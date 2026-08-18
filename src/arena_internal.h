#ifndef ARENA_MEMTABLE_ARENA_INTERNAL_H
#define ARENA_MEMTABLE_ARENA_INTERNAL_H

#include <stdint.h>

/**
 * @file arena_internal.h
 * @brief Internal definition of the arena allocator structure.
 */

/**
 * @brief Representation of the memory arena allocator.
 */
struct arena {
    unsigned char* buffer; /**< Backing contiguous block of raw memory bytes. */
    int32_t size;          /**< Total capacity of the arena backing buffer in bytes. */
    int32_t next_offset;   /**< Offset indicating the next available byte for bump allocation. */
};

#endif //ARENA_MEMTABLE_ARENA_INTERNAL_H