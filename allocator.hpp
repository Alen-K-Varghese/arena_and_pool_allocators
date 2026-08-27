#pragma once

#include <err.h>
#include <cstdint>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

constexpr u64 SIZE_ALLIGN = sizeof(void*); // 1 Byte
constexpr u64 PAGE_SIZE = 8*1024;

namespace internal{

    struct page_header{
        u64 page_size;
        bool is_free;
        bool is_rewritable;
        page_header *next;
        page_header *prev;
    }; // 32 BYTES

    struct Allocator{
        u64 capacity;
        u64 free_block_largest;
        page_header *free_list_head;  
        page_header *current_page;

        Allocator();
        ~Allocator() = default;

        Allocator( Allocator const &) = delete;
        Allocator( Allocator &) = delete;

        page_header *allocate_memoy(u64 capacity, bool is_rewritable);
        void deallocate_memory(page_header *hdr);
        page_header *new_node();
    }; // 32 BYTES

    struct Arena{
        u64 capacity;
        u64 current_pos;
        page_header *hdr;
        void* data_ptr;
    };

    struct Stack{
        u64 capacity;
        u64 current_pos;
        page_header *hdr;
        void *data_ptr;
    };

    struct Node;
    struct Pool{
        u64 max_nodes;
        u64 used_node_count;
        u64 node_size;
        page_header *hdr;
        Node* head_node;
    };
    struct Node{
        Node* next;
    };
} // Internal


internal::Allocator *generate_allocator(u64 size);

void *allocate_arena(internal::Allocator *allocator, u64 size); 