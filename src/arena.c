#include <stdlib.h>

#include "arena_memtable/arena.h"

#include "arena_internal.h"

struct arena *new_arena(int32_t const capacity) {
    unsigned char *buffer = malloc(capacity);
    if (!buffer) {
        return nullptr;
    }
    struct arena *arena = malloc(sizeof(struct arena));
    if (!arena) {
        free(buffer);
        return nullptr;
    }
    arena->buffer = buffer;
    arena->size = capacity;
    arena->next_offset = 1;

    return arena;
}

arena_offset allocate(struct arena *const arena, int32_t const size) {
    if (size > arena->size - arena->next_offset) {
        return -1;
    }
    size_t const next_offset = arena->next_offset;
    arena->next_offset = arena->next_offset + size;

    return (arena_offset) next_offset;
}
