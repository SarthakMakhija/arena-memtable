#ifndef ARENA_MEMTABLE_NODE_INTERNAL_H
#define ARENA_MEMTABLE_NODE_INTERNAL_H

#include "arena_memtable/node.h"

/**
 * @file node_internal.h
 * @brief Internal node functions for mutating node links in the arena.
 */

/**
 * @brief Updates the next node pointer link of a node in the arena.
 *
 * Directly writes the offset of the `next` node into the Next Node Offset field
 * of the `node` header in the arena.
 *
 * @param node The node whose link is being updated.
 * @param next The node to link to as the next node.
 */
void set_next_node_of(struct node node, struct node next);

#endif //ARENA_MEMTABLE_NODE_INTERNAL_H