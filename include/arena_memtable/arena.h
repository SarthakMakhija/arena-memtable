#ifndef ARENA_MEMTABLE_ARENA_H
#define ARENA_MEMTABLE_ARENA_H

/**
 * @file arena.h
 * @brief Memory arena allocator interface.
 *
 * This header defines the public interface for the arena memory allocator.
 * The arena allocator allocates a single large contiguous block of memory
 * and serves subsequent allocation requests by advancing a simple offset pointer
 * (bump allocation).
 *
 * Instead of returning raw pointers, the allocator returns relative offsets
 * (`arena_offset`) from the start of the arena buffer. This allows serialized
 * structures to be relocatable.
 *
 * - Offset `0` is reserved as a null node offset.
 * - Offset `-1` (`ARENA_OFFSET_INVALID`) represents an allocation failure or error.
 */

/**
 * @brief Value indicating an invalid arena offset.
 */
#define ARENA_OFFSET_INVALID (-1)

#include <stdint.h>

/**
 * @brief Represents an offset within the arena.
 *
 * It is a relative index/offset from the start of the arena buffer rather than
 * a direct memory pointer.
 * - `0` is reserved as the null offset (equivalent to a NULL pointer).
 * - `-1` (`ARENA_OFFSET_INVALID`) represents an invalid offset or allocation failure.
 */
typedef int32_t arena_offset;

/**
 * @brief Opaque handle representing the memory arena.
 */
struct arena;

/**
 * @brief Creates a new memory arena.
 *
 * Allocates a contiguous buffer of the specified capacity to serve as the arena's memory pool,
 * along with the internal management structure.
 *
 * @param capacity The total size of the arena memory pool in bytes.
 * @return A pointer to the newly created arena, or nullptr if memory allocation fails.
 */
struct arena *new_arena(int32_t capacity);

/**
 * @brief Allocates a contiguous block of memory from the arena.
 *
 * Bump-allocates `size` bytes of memory within the arena's contiguous buffer.
 * If there is sufficient capacity, it returns the start offset of the allocated block
 * and advances the arena's internal offset.
 *
 * @param arena Pointer to the arena from which to allocate.
 * @param size The number of bytes to allocate.
 * @return The offset of the allocated block (`arena_offset`) on success,
 *         or `ARENA_OFFSET_INVALID` (-1) if there is insufficient space in the arena.
 */
arena_offset allocate(struct arena *arena, int32_t size);

/**
 * @brief Destroys the arena and frees all associated memory.
 *
 * Releases the contiguous memory buffer and the arena management structure itself.
 * All offsets returned by the allocator become invalid after this call.
 *
 * @param arena Pointer to the arena to destroy. If nullptr, the function does nothing.
 */
void destroy_arena(struct arena *arena);

#endif //ARENA_MEMTABLE_ARENA_H

