#include <malloc.h>
#include <new>

#include "arena_allocator.hpp"




mem_arena* create_arena(u64 capacity){

    mem_arena* arena = (mem_arena*)malloc(ARENA_OFFSET);
    if(!arena) throw std::bad_alloc();

    u8* data_ptr = (u8*)malloc(capacity);
    if(!data_ptr) {
        free(arena);
        throw std::bad_alloc();
    }

    arena->capacity = capacity;
    arena->current_pos = 0;
    arena->buffer_ptr = data_ptr;

    return arena;
}

void destroy_arena(mem_arena *arena){
    free(arena->buffer_ptr);
    free(arena);
}

void clear_arena(mem_arena *arena){
    arena->current_pos = 0;
}

u8* arena_push_mem(mem_arena *arena, u64 size){
    u64 position = arena->current_pos + size;
    
    if(position > arena->capacity) throw std::bad_alloc();
    
    else{
        u8* head_ptr = arena->buffer_ptr + arena->current_pos;
        arena->current_pos += size;
        
        return head_ptr;
    }

}
void arena_pop_mem(mem_arena *arena, u64 size){

    if(size > arena->current_pos) throw std::bad_alloc();
    else{
        arena->current_pos -= size;
    }
}
void arena_pop_to(mem_arena *arena, u64 position){
    if(position > 0 && position < arena->capacity) arena->current_pos = position;
    
    else throw std::bad_alloc();
}
