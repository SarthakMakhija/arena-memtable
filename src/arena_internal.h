#ifndef ARENA_MEMTABLE_ARENA_INTERNAL_H
#define ARENA_MEMTABLE_ARENA_INTERNAL_H

struct arena {
    unsigned char* buffer;
    int32_t size;
    int32_t next_offset;
};

#endif //ARENA_MEMTABLE_ARENA_INTERNAL_H
