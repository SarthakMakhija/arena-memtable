#ifndef ARENA_MEMTABLE_ARENA_H
#define ARENA_MEMTABLE_ARENA_H

#define ARENA_OFFSET_INVALID (-1)

#include <stdint.h>

typedef int32_t arena_offset;

struct arena;

struct arena *new_arena(int32_t capacity);

arena_offset allocate(struct arena *arena, int32_t size);

void destroy_arena(struct arena *arena);

#endif //ARENA_MEMTABLE_ARENA_H
