#ifndef ARENA_MEMTABLE_ARENA_H
#define ARENA_MEMTABLE_ARENA_H

#define ARENA_OFFSET_INVALID (-1)

typedef int64_t arena_offset;

struct arena;

struct arena* new_arena(size_t capacity);

arena_offset allocate(struct arena* arena, size_t size);

#endif //ARENA_MEMTABLE_ARENA_H
