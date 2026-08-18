#ifndef ARENA_MEMTABLE_NODE_H
#define ARENA_MEMTABLE_NODE_H

#include <stdint.h>

#include "arena.h"
#include "arena_memtable/slice.h"

struct node {
    struct arena *arena;
    arena_offset offset;
};

struct node new_node(
    struct arena *arena,
    const char *key,
    uint16_t key_length,
    const char *value,
    uint32_t value_length
);

bool is_valid_node(struct node node);

bool is_null_node(struct node node);

bool has_next_node(struct node node);

struct slice key_from(struct node node);

struct slice value_from(struct node node);

#endif //ARENA_MEMTABLE_NODE_H
