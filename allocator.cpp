#include <malloc.h>

using u64 = unsigned long ;


typedef struct{
    u64 capacity;
    u64 current_pos;

}arena_allocator;

arena_allocator *create_arena(u64 capacity);
bool delete_arena(arena_allocator *arena);
bool clear_arena(arena_allocator *arena);

void arena_push_mem(arena_allocator *arena, u64 size, bool assign_zero);
void arena_pop_mem(arena_allocator *arena, u64 size);
void arena_pop_to(arena_allocator *arena, u64 position);

// This is offset, as the arena metadata has size sizeof(arena_allocator)
constexpr u64 ARENA_OFFSET = sizeof(arena_allocator); 

arena_allocator* create_arena(u64 capacity){

    void *initial_offset = (void*)malloc(ARENA_OFFSET);

    capacity -= ARENA_OFFSET;
    arena_allocator *arena = (arena_allocator*)malloc(capacity );   
    arena->capacity = capacity;
    arena->current_pos = 0;

    return arena;
}
