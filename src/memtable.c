#include "arena_memtable/memtable.h"

#include <stdlib.h>

#include "arena_memtable/sorted_list.h"

/**
 * @brief Representation of the memtable.
 */
struct memtable {
    struct sorted_list *list; /**< Internal arena-backed sorted list. */
};

/**
 * @brief Creates a new memtable with the given arena capacity.
 */
struct memtable *new_memtable(int32_t const capacity) {
    struct sorted_list *list = new_list(capacity);
    if (list == nullptr) {
        return nullptr;
    }
    struct memtable *memtable = malloc(sizeof(struct memtable));
    if (memtable == nullptr) {
        destroy_list(list);
        return nullptr;
    }
    memtable->list = list;
    return memtable;
}

/**
 * @brief Inserts a key/value pair into the memtable.
 */
bool put(struct memtable const *memtable, const char *key, uint16_t key_length, const char *value, uint32_t value_length) {
    if (memtable == nullptr) {
        return false;
    }
    return list_put(memtable->list, key, key_length, value, value_length);
}

/**
 * @brief Looks up a key in the memtable and returns a view of the value payload.
 */
struct slice get_value(struct memtable const *memtable, char const *key, uint16_t const key_length) {
    if (memtable == nullptr) {
        return new_invalid_slice();
    }
    return list_get_value(memtable->list, key, key_length);
}

/**
 * @brief Destroys the memtable and all its backing memory.
 */
void destroy_memtable(struct memtable *memtable) {
    if (memtable == nullptr) {
        return;
    }
    destroy_list(memtable->list);
    free(memtable);
}