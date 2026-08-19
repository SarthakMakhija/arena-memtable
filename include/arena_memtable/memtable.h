#ifndef ARENA_MEMTABLE_MEMTABLE_H
#define ARENA_MEMTABLE_MEMTABLE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file memtable.h
 * @brief Memtable (Memory Table) interface.
 *
 * A memtable is an in-memory data structure used in storage engines (such as LSM-trees)
 * to buffer write operations before they are flushed to disk.
 *
 * This implementation wraps a memory-arena-backed @ref sorted_list, ensuring that
 * all elements are sorted by key and allocated contiguously in pre-allocated memory.
 */

/**
 * @brief Opaque handle representing the memtable.
 */
struct memtable;

/**
 * @brief Creates a new memtable with a given capacity.
 *
 * Allocates the memtable structure and its internal arena-backed sorted list.
 *
 * @param capacity The size in bytes of the backing memory arena.
 * @return A pointer to the newly created memtable, or nullptr if allocation fails.
 */
struct memtable *new_memtable(int32_t capacity);

/**
 * @brief Inserts a key/value pair into the memtable.
 *
 * Delegates insertion to the internal sorted list. If there is sufficient capacity
 * in the backing arena, the node is created and inserted in sorted order.
 *
 * @param memtable Pointer to the memtable.
 * @param key Pointer to the key string.
 * @param key_length Length of the key in bytes.
 * @param value Pointer to the value bytes.
 * @param value_length Length of the value in bytes.
 * @return true on success, false if the memtable is nullptr or the arena is full.
 */
bool put(struct memtable const *memtable, const char key[static 1], uint16_t key_length, const char value[static 1], uint32_t value_length);

/**
 * @brief Looks up a key in the memtable.
 *
 * Searches the internal sorted list for a node matching the specified key.
 *
 * @param memtable Pointer to the memtable.
 * @param key Pointer to the search key string.
 * @param key_length Length of the key in bytes.
 * @return A slice pointing to the associated value in the arena, or an invalid slice if not found.
 */
struct slice get_value(struct memtable const *memtable, char const key[static 1], uint16_t key_length);

/**
 * @brief Destroys the memtable and releases all allocated memory.
 *
 * Destroys the internal sorted list and the backing memory arena, then frees
 * the memtable wrapper.
 *
 * @param memtable Pointer to the memtable to destroy. If nullptr, does nothing.
 */
void destroy_memtable(struct memtable *memtable);

#endif //ARENA_MEMTABLE_MEMTABLE_H