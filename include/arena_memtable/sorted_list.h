#ifndef ARENA_MEMTABLE_LIST_H
#define ARENA_MEMTABLE_LIST_H

#include <stdint.h>

#include "slice.h"

struct sorted_list;

struct sorted_list *new_list(int32_t capacity);

bool put(struct sorted_list *list, const char *key, uint16_t key_length, const char *value, uint32_t value_length);

struct slice get_value(struct sorted_list const *list, char const *key, uint16_t key_length);

void destroy_list(struct sorted_list *list);

#endif //ARENA_MEMTABLE_LIST_H
