#ifndef ARENA_MEMTABLE_NODE_H
#define ARENA_MEMTABLE_NODE_H

#include <stdint.h>
#include <stdbool.h>

#include "arena.h"
#include "arena_memtable/slice.h"

/**
 * @file node.h
 * @brief Logical node interface representing a key/value pair stored in the arena.
 *
 * A node is a purely logical concept. Instead of allocating separate C structs
 * on the heap, nodes are serialized directly into the arena's contiguous byte buffer.
 *
 * Each `struct node` is a lightweight handle wrapping:
 * 1. A pointer to the @ref arena in which the node's data is stored.
 * 2. An @ref arena_offset pointing to the start of the node's byte payload in the arena.
 *
 * @section node_layout Binary Layout of a Node in Arena
 *
 *  0             2             6                   10               10 + KeyLen        Total Size
 * +-------------+-------------+-------------------+-----------------+------------------+
 * | Key Length  | Val Length  | Next Node Offset  | Key Payload     | Value Payload    |
 * | (uint16_t)  | (uint32_t)  | (arena_offset)    | (KeyLen bytes)  | (ValLen bytes)   |
 * | [ 2 Bytes ] | [ 4 Bytes ] | [ 4 Bytes ]       |                 |                  |
 * +-------------+-------------+-------------------+-----------------+------------------+
 * |<---------- Fixed Header (10 Bytes) ---------->|
 */

/**
 * @brief Lightweight handle representing a logical node inside the arena.
 */
struct node {
    struct arena *arena;  /**< Pointer to the backing arena. */
    arena_offset offset;  /**< Byte offset of the node within the arena buffer. */
};

/**
 * @brief Creates and serializes a new node into the arena.
 *
 * Calculates the required size for the node (10 bytes header + key length + value length),
 * allocates space in the arena, and serializes the header and payloads into the allocated space.
 *
 * @param arena Pointer to the arena where the node will be stored.
 * @param key Pointer to the key payload string.
 * @param key_length Length of the key in bytes.
 * @param value Pointer to the value payload string.
 * @param value_length Length of the value in bytes.
 * @return A struct node handle representing the newly created node. If allocation fails or
 *         size limits are exceeded, returns a node with offset `ARENA_OFFSET_INVALID`.
 */
struct node new_node(
    struct arena *arena,
    const char *key,
    uint16_t key_length,
    const char *value,
    uint32_t value_length
);

/**
 * @brief Checks if the node is valid.
 *
 * A node is valid if its offset is not `ARENA_OFFSET_INVALID` (-1).
 *
 * @param node The node to check.
 * @return true if valid, false otherwise.
 */
bool is_valid_node(struct node node);

/**
 * @brief Checks if the node is a null node pointer representation.
 *
 * A null node represents the end of a linked list or an uninitialized link.
 * It has an offset of `0` (`NULL_NODE_OFFSET`).
 *
 * @param node The node to check.
 * @return true if it is a null node (offset is 0), false otherwise.
 */
bool is_null_node(struct node node);

/**
 * @brief Checks if the node points to a next node.
 *
 * Reads the Next Node Offset field from the node's header in the arena to see
 * if it is non-zero (i.e. not null).
 *
 * @param node The node to check.
 * @return true if the next node offset is not 0, false otherwise.
 */
bool has_next_node(struct node node);

/**
 * @brief Retrieves the key payload of the node.
 *
 * Reads the key length from the header and returns a slice pointing directly
 * to the key payload within the arena buffer.
 *
 * @param node The node to retrieve the key from.
 * @return A slice pointing to the key data and its length. Returns an invalid slice if the node is invalid.
 */
struct slice key_of(struct node node);

/**
 * @brief Retrieves the value payload of the node.
 *
 * Reads the value length and key length from the header and returns a slice
 * pointing directly to the value payload within the arena buffer.
 *
 * @param node The node to retrieve the value from.
 * @return A slice pointing to the value data and its length. Returns an invalid slice if the node/key is invalid.
 */
struct slice value_of(struct node node);

/**
 * @brief Returns the next node in the list.
 *
 * Reads the Next Node Offset field from the node's header in the arena and returns
 * a new node handle pointing to that offset.
 *
 * @param node The current node.
 * @return A node handle representing the next node.
 */
struct node next_node_of(struct node node);

#endif //ARENA_MEMTABLE_NODE_H