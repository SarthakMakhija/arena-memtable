#ifndef ARENA_MEMTABLE_SLICE_H
#define ARENA_MEMTABLE_SLICE_H
#include <stdint.h>

struct slice {
    char const* data;
    uint32_t length;
};

struct slice new_invalid_slice();

bool is_valid_slice(struct slice slice);

#endif //ARENA_MEMTABLE_SLICE_H
