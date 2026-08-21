#pragma once


#include <cstdint>
#include <malloc.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;



struct mem_arena{
    u64 capacity;
    u64 current_pos;
    u8* buffer_ptr;
};

constexpr u64 ARENA_OFFSET = sizeof(mem_arena);

mem_arena* create_arena(u64 capacity);
void destroy_arena(mem_arena *arena);
void clear_arena(mem_arena *arena);

void arena_push_mem(mem_arena *arena, u64 size, bool assign_zero);
void arena_pop_mem(mem_arena *arena, u64 size);
void arena_pop_to(mem_arena *arena, u64 position);

