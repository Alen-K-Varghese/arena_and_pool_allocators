
#include "pool_allocator.hpp"
#include <new>



mem_pool* create_memory_pool(u64 num_elements, u64 element_size){
    mem_pool* pool = (mem_pool*)malloc(POOL_OFFSET);
    if(!pool) throw std::bad_alloc();

    u8* data_ptr = (u8*)malloc(num_elements*element_size);
    if(!data_ptr){
        free(pool);
        throw std::bad_alloc();
}
    pool->chunk_count = 0;
    pool->max_chunks = num_elements;
    pool->data_buffer = data_ptr;
    pool->chunk_size = element_size;

    return pool;
}
void destroy_pool(mem_pool* pool){
    free(pool->data_buffer);
    free(pool);
}

u8* add_elements(mem_pool* pool, u64 num_elements){
    if(num_elements > (pool->max_chunks - pool->chunk_count)) throw std::bad_alloc();

    else{
        u8* data_ptr = pool->data_buffer + (pool->chunk_size * pool->chunk_count);
        pool->chunk_count += num_elements;

        return data_ptr;
    }
}
void remove_elements(mem_pool* pool, u64 num_elements){
    if(num_elements > pool->chunk_count) throw std::bad_alloc();
    else{
        pool->chunk_count -= num_elements;
    }
}
void clear_pool(mem_pool* pool){
    pool->chunk_count = 0;
}