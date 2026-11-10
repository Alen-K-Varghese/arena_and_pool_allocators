#pragma once

#include "base.hpp"
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <sys/mman.h>
namespace memory {
namespace internal {

template<u64 arena_size = MiB_1>
class arena_impl{
    public:
    using value_type      = std::byte;
    using pointer         = value_type *;
    using const_pointer   = const value_type *;
    using void_pointer    = void *;
    using size_type       = u32;
    using const_size_type = const u32;
    using difference_type = std::ptrdiff_t;

    public:
    arena_impl(){
        void_pointer ptr = mmap( NULL, (arena_size) + (KiB_4), 
                            PROT_READ | PROT_WRITE , 
                            MAP_ANON | MAP_POPULATE , 
                            -1, 0);

        if(ptr == MAP_FAILED){ throw; }

        buffer_head_ = static_cast<pointer>(ptr);
        current_     = buffer_head_;
        buffer_end_  = buffer_head_ + arena_size;

        mprotect(buffer_end_ , KiB_4, PROT_NONE);
    }


    arena_impl           (arena_impl const& other) = delete;
    arena_impl operator= (arena_impl const& other) = delete;

    arena_impl          (arena_impl&& other) = delete;
    arena_impl operator=(arena_impl&& other) = delete;

    ~arena_impl(){ destroy(); }

    void_pointer allocate(const size_type size, const size_type alignment = _align){
        void_pointer ret = nullptr;

        if(!is_pow2(alignment)){ return ret; }
        align_as(alignment);

        const difference_type diff = buffer_end_ - current_;
        if( diff < size ) { return ret; }

        ret = current_;
        current_ = current_ + size;

        return static_cast<void_pointer>(ret);
    }

    _inline void_pointer 
    unsafe_alloc(const size_type size, const size_type alignment = _align){
        align_as(alignment);

        pointer ret = current_;
        current_ = current_ + size;

        return static_cast<void_pointer>(ret);
    }


    void deallocate(const void_pointer ptr, const size_type size){
        pointer predicted = static_cast<pointer>(ptr) + size;

        if(predicted == current_){ current_ -= size; }

    }

    _inline void clear(){ current_ = buffer_head_; }

    void destroy(){
        munmap(static_cast<void_pointer>(buffer_head_), (buffer_end_ - buffer_head_) + (KiB_4));
    }

    private:
    _inline void 
    align_as(const size_type alignment){
        current_ = reinterpret_cast<pointer>(
            (reinterpret_cast<uintptr_t>(current_) + alignment - 1) & 
            ~static_cast<uintptr_t>(alignment - 1)
        );
    }

    _inline bool 
    is_pow2(const size_type value){ return (((value) & (value - 1)) == 0); }

    private:
    _inline void incr_ref() { m_ref_count.fetch_add(1); }
    _inline void decr_ref() { m_ref_count.fetch_sub(1); }
    _inline bool zero_ref() { return m_ref_count.load() == 0; }

    private:
    pointer buffer_head_;
    pointer buffer_end_;
    pointer current_;
    std::atomic<size_type> m_ref_count{0};
    
};
} // namespace internal

template<typename dtype, internal::arena_impl<>::size_type arena_size>
class arena_alloc{
    public:
    using value_type      = dtype;
    using pointer         = value_type *;
    using const_pointer   = const value_type *;
    using void_pointer    = internal::arena_impl<>::void_pointer;
    using size_type       = internal::arena_impl<>::size_type;
    using const_size_type = internal::arena_impl<>::const_size_type;
    using difference_type = internal::arena_impl<>::difference_type;

    public:
    arena_alloc(){
        m_arena = new internal::arena_impl<arena_size>;
        m_arena->incr_ref();
    }

    arena_alloc (arena_alloc const& other){
        m_arena = other.m_arena;
        if(m_arena == nullptr){ throw "null init error"; }

        m_arena->incr_ref();
    }

    arena_alloc& operator= (arena_alloc const& other){

        if(&other == this){ return *this; }

        if(other.m_arena == nullptr){ throw "null init error"; }

        if(m_arena != nullptr){ m_arena->decr_ref(); }
        
        m_arena = other.m_arena;
        m_arena->incr_ref();

        return *this;
    }
    
    arena_alloc           (arena_alloc&& other) = delete;
    arena_alloc operator= (arena_alloc&& other) = delete;


    _inline pointer allocate(const size_type size, const size_type){
        return static_cast<pointer>( m_arena->allocate(size*sizeof(dtype), alignof(dtype)) );
    }

    _inline pointer unsafe_alloc(const size_type size, const size_type){
        return static_cast<pointer>( m_arena->unsafe_alloc(size*sizeof(dtype), alignof(dtype)) );
    }

    _inline void deallocate(const pointer ptr, const size_type size){
        m_arena->deallocate(static_cast<void_pointer>(ptr), size);
    }

    ~arena_alloc(){ destroy(); }

    void destroy(){
        m_arena->decr_ref();

        if(m_arena->zero_ref()){
            delete m_arena;
        }
    }

    private:
    internal::arena_impl<arena_size> *m_arena{nullptr};
    
};


}