#pragma once


#include "arena_allocator.hpp"
#include "base.hpp"
namespace memory {

template< internal::arena_impl<>::const_size_type node_size, 
          internal::arena_impl<>::const_size_type pool_size = MiB_1,
          internal::arena_impl<>::const_size_type alignment = _align
        >
class pool_alloc{
    public:
    using value_type      = internal::arena_impl<>::value_type;
    using pointer         = internal::arena_impl<>::pointer;
    using const_pointer   = internal::arena_impl<>::const_pointer;
    using void_pointer    = internal::arena_impl<>::void_pointer;
    using size_type       = internal::arena_impl<>::size_type;
    using const_size_type = internal::arena_impl<>::const_size_type;
    using difference_type = internal::arena_impl<>::difference_type;

    public:
    struct free_list{
        free_list *next; 
    };

    static_assert(node_size < pool_size, "incorrect initalization");
    static_assert(node_size >= 8       , "too small initiaization");

    public:
    pool_alloc() = default;

    void_pointer allocate (size_type, size_type){
        pointer ret = m_head.next;

        if(ret){ m_head.next = m_head.next->next; }
        else   { ret = m_arena.allocate(node_size, alignment); }
        
        return ret;
    };


    ~pool_alloc(){
        m_arena.destroy();
    }

    private:
    arena_alloc<pool_size> m_arena;
    free_list m_head{ nullptr };

};

}