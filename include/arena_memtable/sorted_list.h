#ifndef ARENA_MEMTABLE_LIST_H
#define ARENA_MEMTABLE_LIST_H

#include <stdint.h>
#include <stdbool.h>

#include "slice.h"

/**
 * @file sorted_list.h
 * @brief Arena-backed sorted single linked list interface.
 *
 * This header defines a sorted single linked list where all node headers, keys,
 * and values are stored in a pre-allocated contiguous memory arena.
 *
 * The list is sorted in ascending lexicographical order of its keys.
 * All memory allocations for the list nodes are managed by the underlying arena.
 */

/**
 * @brief Opaque handle representing the sorted linked list.
 */
struct sorted_list;

/**
 * @brief Creates a new sorted linked list with a backing memory arena.
 *
 * Allocates the sorted_list container structure and initializes its backing arena
 * with the specified byte capacity.
 *
 * @param capacity The capacity in bytes of the backing arena.
 * @return A pointer to the newly created sorted_list, or nullptr if allocation fails.
 */
struct sorted_list *new_list(int32_t capacity);

/**
 * @brief Inserts a key/value pair into the sorted list in lexicographical order.
 *
 * Allocates and serializes a new node into the backing arena, then traverses the list
 * to find the correct sorted insertion point. If list insertion succeeds, returns true.
 * If the arena has insufficient capacity to allocate the node, returns false.
 *
 * @param list Pointer to the sorted list.
 * @param key Pointer to the key payload string.
 * @param key_length Length of the key in bytes.
 * @param value Pointer to the value payload bytes.
 * @param value_length Length of the value in bytes.
 * @return true on success, false if the node cannot be allocated or list is invalid.
 */
bool list_put(struct sorted_list *list, const char key[static 1], uint16_t key_length, const char value[static 1], uint32_t value_length);

/**
 * @brief Retrieves the value associated with a key from the sorted list.
 *
 * Traverses the sorted list, comparing each node's key to the search key.
 * Because the list is sorted, searching stops early if a node with a key greater than
 * the search key is encountered (indicating the key is not present).
 *
 * @param list Pointer to the sorted list.
 * @param key Pointer to the key string to look up.
 * @param key_length Length of the key in bytes.
 * @return A slice pointing to the value payload in the arena if found, or an invalid slice if not found.
 */
struct slice list_get_value(struct sorted_list const *list, char const key[static 1], uint16_t key_length);

/**
 * @brief Destroys the sorted list and its backing arena.
 *
 * Frees the backing arena (which releases all memory used by list nodes) and the
 * list structure itself.
 *
 * @param list Pointer to the sorted list to destroy. If nullptr, does nothing.
 */
void destroy_list(struct sorted_list *list);

#endif //ARENA_MEMTABLE_LIST_H