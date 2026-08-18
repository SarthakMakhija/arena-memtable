#include <stdlib.h>

#include "arena_memtable/arena.h"

#include "arena_internal.h"

/**
 * @brief Creates a new memory arena.
 *
 * Allocates `capacity` bytes of memory for the backing buffer and the arena structure.
 * Initializes `next_offset` to 1 to reserve offset 0 as a null/invalid offset pointer representation.
 *
 * @param capacity The size of the backing buffer in bytes.
 * @return A pointer to the initialized struct arena, or nullptr on failure.
 */
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

/**
 * @brief Allocates a contiguous block of memory from the arena.
 *
 * Compares the requested size with the remaining space in the arena backing buffer.
 * If enough space remains, returns the offset of the block start and advances `next_offset`
 * by `size` bytes. Otherwise, returns -1 (`ARENA_OFFSET_INVALID`).
 *
 * @param arena Pointer to the arena.
 * @param size The number of bytes requested.
 * @return The start offset of the allocated block, or ARENA_OFFSET_INVALID (-1) on failure.
 */
arena_offset allocate(struct arena *const arena, int32_t const size) {
    if (size > arena->size - arena->next_offset) {
        return -1;
    }
    size_t const next_offset = arena->next_offset;
    arena->next_offset = arena->next_offset + size;

    return (arena_offset) next_offset;
}

/**
 * @brief Destroys the arena and frees all associated memory.
 *
 * Frees the backing byte buffer and the arena struct itself.
 * Safe to call with a nullptr argument.
 *
 * @param arena Pointer to the arena to destroy.
 */
void destroy_arena(struct arena* arena) {
    if (arena == nullptr) {
        return;
    }
    free(arena->buffer);
    free(arena);
}