#include <stdlib.h>
#include <string.h>

#include "arena_memtable/sorted_list.h"
#include "node_internal.h"
#include "arena_memtable/node.h"

/**
 * @brief Representation of the sorted linked list.
 */
struct sorted_list {
    struct arena *arena; /**< Backing memory arena where nodes are stored. */
    struct node head;    /**< Logical node handle representing the start of the list. */
};

/**
 * @brief Compares two keys lexicographically.
 *
 * Helper function that compares key bytes.
 *
 * @param key Pointer to first key.
 * @param first_length Length of first key.
 * @param other_key Pointer to second key.
 * @param other_length Length of second key.
 * @return An integer less than, equal to, or greater than zero if key is found,
 *         respectively, to be less than, to match, or be greater than other_key.
 */
static int compare_keys(const char *key, uint16_t first_length, const char *other_key, uint16_t other_length);

/**
 * @brief Creates a new sorted linked list.
 *
 * Initializes the underlying arena and sets the head node pointer to offset 0 (null node).
 */
struct sorted_list *new_list(int32_t const capacity) {
    struct arena *const arena = new_arena(capacity);
    if (arena == nullptr) {
        return nullptr;
    }
    struct sorted_list *list = malloc(sizeof(struct sorted_list));
    if (list == nullptr) {
        destroy_arena(arena);
        return nullptr;
    }
    list->arena = arena;
    list->head = (struct node){.arena = arena, .offset = 0};
    return list;
}

/**
 * @brief Inserts a key/value pair into the sorted list in lexicographical order.
 *
 * Performs sorted insertion of the new node by handling three insertion cases:
 * - Case A: The list is empty. Set head to the new node.
 * - Case B: The new key belongs before the current head. Insert before head and update head.
 * - Case C: Traverse list to find the correct slot, inserting the node between two existing nodes
 *           or appending at the end of the list.
 */
bool list_put(struct sorted_list *list, const char *key, uint16_t const key_length, const char *value,
         uint32_t const value_length) {
    const struct node node = new_node(list->arena, key, key_length, value, value_length);
    if (!is_valid_node(node)) {
        return false;
    }

    // Case A: List is empty
    if (is_null_node(list->head)) {
        set_next_node_of(node, list->head);
        list->head = node;
        return true;
    }

    struct slice node_key = key_of(list->head);
    // Case B: New key belongs before current head
    if (compare_keys(key, key_length, node_key.data, node_key.length) <= 0) {
        set_next_node_of(node, list->head);
        list->head = node;
        return true;
    }

    // Case C: Traverse the list to find the correct sorted insertion point
    struct node current = list->head;
    while (true) {
        struct node const next = next_node_of(current);
        if (is_null_node(next)) {
            // Reached end of the list; append here
            set_next_node_of(current, node);
            break;
        }

        node_key = key_of(next);
        if (compare_keys(key, key_length, node_key.data, node_key.length) <= 0) {
            // Key lies between current and next.
            set_next_node_of(current, node);
            set_next_node_of(node, next);
            break;
        }
        current = next;
    }
    return true;
}

/**
 * @brief Retrieves the value slice associated with a key.
 *
 * Iterates through the list, comparing keys lexicographically.
 * Since the list is sorted, lookup stops early if a greater key is met.
 */
struct slice list_get_value(struct sorted_list const *list, char const *key, uint16_t const key_length) {
    struct node current = list->head;
    while (!is_null_node(current)) {
        struct slice const node_key = key_of(current);
        //compare node's key and search key
        int const comparison_result = compare_keys(node_key.data, node_key.length, key, key_length);
        if (comparison_result == 0) {
            return value_of(current);
        }
        if (comparison_result > 0) {
            return new_invalid_slice();
        }
        current = next_node_of(current);
    }
    return new_invalid_slice();
}

/**
 * @brief Destroys the list and frees its resources.
 *
 * Destroys the backing memory arena, which frees all node memory in one go,
 * then frees the list wrapper.
 */
void destroy_list(struct sorted_list *list) {
    if (list == nullptr) {
        return;
    }
    destroy_arena(list->arena);
    free(list);
}

/**
 * @brief Compares two keys lexicographically.
 */
static int compare_keys(
    const char *key,
    uint16_t const first_length,
    const char *other_key,
    uint16_t const other_length
) {
    uint16_t const length = first_length < other_length ? first_length : other_length;
    int const result = memcmp(key, other_key, length);

    if (result != 0) {
        return result;
    }
    if (first_length < other_length) {
        return -1;
    }
    if (first_length > other_length) {
        return 1;
    }
    return 0;
}