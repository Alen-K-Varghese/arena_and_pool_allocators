#pragma once

#include "arena_allocator.hpp"
#include <cassert>
namespace container {
namespace internal {

template <typename dtype>
class const_iterator{
    public:
    using value_type      = dtype;
    using pointer         = value_type *;
    using const_pointer   = const value_type *;
    using void_pointer    = memory::internal::arena_impl<>::void_pointer;
    using size_type       = memory::internal::arena_impl<>::size_type;
    using const_size_type = memory::internal::arena_impl<>::const_size_type;
    using difference_type = memory::internal::arena_impl<>::difference_type;

    public:
    const_iterator(pointer ptr);

    pointer operator++()   { return ptr_++; }
    pointer operator++(int){ return ++ptr_; }
    pointer operator--()   { return ptr_--; }
    pointer operator--(int){ return --ptr_; }

    pointer operator+(size_type size){ return ptr_ + size; }
    pointer operator-(size_type size){ return ptr_ - size; }
    
    pointer operator*(){ return *ptr_; }
    pointer operator->(){ return ptr_; }

    pointer get() { return ptr_; }

    private:
    pointer ptr_;
};


template <typename dtype>
class array_base{
    public:
    using value_type      = dtype;
    using pointer         = value_type *;
    using iterator        = const_iterator<value_type>;
    using const_pointer   = const value_type *;
    using void_pointer    = memory::internal::arena_impl<>::void_pointer;
    using size_type       = memory::internal::arena_impl<>::size_type;
    using const_size_type = memory::internal::arena_impl<>::const_size_type;
    using difference_type = memory::internal::arena_impl<>::difference_type;

    public:
    array_base() = delete;
    array_base (pointer begin, pointer capacity)
    : begin_(begin)
    , end_(begin + 1)
    , capacity_(capacity)
    {}

    value_type operator[](size_type idx){ return begin_[idx]; }

    iterator begin(){ return const_iterator<value_type>(begin_); }
    iterator end  (){ return const_iterator<value_type>(end_);   }

    protected:
    pointer begin_, end_;
    pointer capacity_;

};

} // namespace inetrnal


template <
            typename dtype, 
            memory::internal::arena_impl<>::size_type buffer_size
        >
class static_array : public internal::array_base<dtype>{
    public:
    using base            = internal::array_base<dtype>; 
    using value_type      = dtype;
    using pointer         = value_type *;
    using iterator        = internal::const_iterator<value_type>;
    using const_pointer   = const value_type *;
    using void_pointer    = memory::internal::arena_impl<>::void_pointer;
    using size_type       = memory::internal::arena_impl<>::size_type;
    using const_size_type = memory::internal::arena_impl<>::const_size_type;
    using difference_type = memory::internal::arena_impl<>::difference_type;
    
    public:
    static_array()
    : base (buffer, buffer_size)
    {}

    public:
    void push_back(value_type value){
        assert(this->end_ < this->capacity_ );
        
        new (this->end_) value_type(value);
        this->end_++;
    }

    void pop_back(){
        assert(this->end_ != this->begin());

        this->end_--;
        this->end_->~value_type();
    }

    private:
    value_type buffer[buffer_size];
};
}