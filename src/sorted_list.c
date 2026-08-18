#include <stdlib.h>
#include <string.h>

#include "arena_memtable/sorted_list.h"
#include "node_internal.h"
#include "arena_memtable/node.h"

struct sorted_list {
    struct arena *arena;
    struct node head;
};

static int compare_keys(const char *key, uint16_t first_length, const char *other_key, uint16_t other_length);

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

void destroy_list(struct sorted_list *list) {
    if (list == nullptr) {
        return;
    }
    destroy_arena(list->arena);
    free(list);
}

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
