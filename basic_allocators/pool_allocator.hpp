#pragma once


#include <cstdint>
#include <malloc.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;


struct mem_pool{
    u8* data_buffer;
    u64 max_chunks;
    u64 chunk_count;
    u64 chunk_size;
};

constexpr u64 POOL_OFFSET = sizeof(mem_pool);

mem_pool* create_memory_pool(u64 num_elements, u64 element_size);
void destroy_pool(mem_pool* pool);

u8* add_elements(mem_pool* pool, u64 num_elements);
void remove_elements(mem_pool* pool, u64 num_elements);
void clear_pool(mem_pool* pool);