#include <malloc.h>

using u64 = unsigned long ;

typedef struct{
    u64 capacity;
    u64 current_pos;

}arena_state;

// This is offset, as the arena metadata has size sizeof(arena_allocator)
constexpr u64 ARENA_OFFSET = sizeof(arena_state); 

class construct_arena
{
private:
    void *initial_offset_;
    arena_state *state_;

public:
    construct_arena(u64 capacity){
        capacity -= ARENA_OFFSET;

        initial_offset_ = (void*)malloc(ARENA_OFFSET);
        state_ = (arena_state*)malloc(capacity);

        state_->capacity = capacity;
    }

};