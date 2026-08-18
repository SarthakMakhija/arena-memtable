#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "arena_memtable/node.h"
#include "node_internal.h"
#include "arena_internal.h"

#define RESERVED_KEY_SIZE                  (sizeof(uint16_t))
#define RESERVED_VALUE_SIZE                (sizeof(uint32_t))
#define RESERVED_NEXT_OFFSET_SIZE          (sizeof(uint32_t))
#define NODE_HEADER_SIZE                   (RESERVED_KEY_SIZE + RESERVED_VALUE_SIZE + RESERVED_NEXT_OFFSET_SIZE)
#define NULL_NODE_OFFSET                   0

#define KEY_LENGTH_OFFSET(offset) \
    (offset)

#define VALUE_LENGTH_OFFSET(offset) \
    (offset + RESERVED_KEY_SIZE)

#define NEXT_OFFSET(offset) \
    (offset + RESERVED_KEY_SIZE + RESERVED_VALUE_SIZE)

#define KEY_OFFSET(offset) \
    (offset + NODE_HEADER_SIZE)

#define VALUE_OFFSET(offset, key_length) \
    (offset + NODE_HEADER_SIZE + key_length)

static void set_key_length(struct arena const *arena, uint16_t key_length, struct node node);

static void set_value_length(struct arena const *arena, uint32_t value_length, struct node node);

static void set_next_node_offset(struct arena const *arena, arena_offset next_offset, struct node node);

static void set_key(struct arena const *arena, char const *key, uint16_t key_length, struct node node);

static void set_value(struct arena const *arena, char const *value, uint32_t value_length,
                      uint16_t key_length, struct node node);

static struct node fill_node_with(struct arena *arena, char const *key, uint16_t key_length, char const *value,
                                  uint32_t value_length,
                                  arena_offset offset);

struct node new_node(
    struct arena *arena,
    char const *key,
    uint16_t const key_length,
    char const *value,
    uint32_t const value_length
) {
    if (value_length > INT32_MAX - NODE_HEADER_SIZE - key_length) {
        return (struct node){
            .arena = arena,
            .offset = ARENA_OFFSET_INVALID
        };
    }

    int32_t const node_size = NODE_HEADER_SIZE + key_length + value_length;
    arena_offset const offset = allocate(arena, node_size);

    if (offset == ARENA_OFFSET_INVALID) {
        return (struct node){
            .arena = arena,
            .offset = ARENA_OFFSET_INVALID
        };
    }
    return fill_node_with(arena, key, key_length, value, value_length, offset);
}

bool is_valid_node(struct node const node) {
    return node.offset != ARENA_OFFSET_INVALID;
}

bool is_null_node(struct node const node) {
    return node.offset == NULL_NODE_OFFSET;
}

bool has_next_node(struct node const node) {
    arena_offset next_offset;
    memcpy(&next_offset, node.arena->buffer + NEXT_OFFSET(node.offset), sizeof(next_offset));

    return next_offset != NULL_NODE_OFFSET;
}

struct slice key_of(struct node const node) {
    if (!is_valid_node(node)) {
        return new_invalid_slice();
    }

    uint16_t key_length;
    memcpy(&key_length, node.arena->buffer + KEY_LENGTH_OFFSET(node.offset), sizeof(key_length));

    return (struct slice){
        .data = (char const *) (node.arena->buffer + KEY_OFFSET(node.offset)),
        .length = key_length
    };
}

struct slice value_of(struct node const node) {
    struct slice const key = key_of(node);
    if (!is_valid_slice(key)) {
        return key;
    }

    uint32_t value_length;
    memcpy(&value_length, node.arena->buffer + VALUE_LENGTH_OFFSET(node.offset), sizeof(value_length));

    return (struct slice){
        .data = (char const *) (node.arena->buffer + VALUE_OFFSET(node.offset, key.length)),
        .length = value_length
    };
}

struct node next_node_of(struct node const node) {
    arena_offset next_offset;
    memcpy(&next_offset, node.arena->buffer + NEXT_OFFSET(node.offset), sizeof(next_offset));

    return (struct node){.arena = node.arena, .offset = next_offset};
}

void set_next_node_of(struct node const node, struct node const next) {
    set_next_node_offset(node.arena, next.offset, node);
}

static void set_key_length(struct arena const *arena, uint16_t const key_length, struct node const node) {
    memcpy(arena->buffer + KEY_LENGTH_OFFSET(node.offset), &key_length, sizeof(key_length));
}

static void set_value_length(struct arena const *arena,
                             uint32_t const value_length,
                             struct node const node) {
    memcpy(arena->buffer + VALUE_LENGTH_OFFSET(node.offset), &value_length, sizeof(value_length));
}

static void set_next_node_offset(struct arena const *arena,
                                 arena_offset const next_offset,
                                 struct node const node) {
    memcpy(arena->buffer + NEXT_OFFSET(node.offset), &next_offset, sizeof(next_offset));
}

static void set_key(struct arena const *arena,
                    char const *key,
                    uint16_t const key_length,
                    struct node const node) {
    memcpy(arena->buffer + KEY_OFFSET(node.offset), key, key_length);
}

static void set_value(struct arena const *arena,
                      char const *value,
                      uint32_t const value_length,
                      uint16_t const key_length,
                      struct node const node) {
    memcpy(arena->buffer + VALUE_OFFSET(node.offset, key_length), value, value_length);
}

static struct node fill_node_with(struct arena *arena,
                                  char const *key,
                                  uint16_t const key_length,
                                  char const *value,
                                  uint32_t const value_length,
                                  arena_offset const offset) {
    struct node const node = {
        .arena = arena,
        .offset = offset,
    };

    set_key_length(arena, key_length, node);
    set_value_length(arena, value_length, node);
    set_next_node_offset(arena, NULL_NODE_OFFSET, node);
    set_key(arena, key, key_length, node);
    set_value(arena, value, value_length, key_length, node);

    return node;
}
