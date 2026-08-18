#ifndef ARENA_MEMTABLE_LIST_H
#define ARENA_MEMTABLE_LIST_H

#include <stdint.h>

#include "slice.h"

struct list;

struct list *new_list(int32_t capacity);

bool put(struct list *list, const char *key, uint16_t key_length, const char *value, uint32_t value_length);

struct slice get_value(struct list const *list, char const *key, uint16_t key_length);

void destroy_list(struct list *list);

#endif //ARENA_MEMTABLE_LIST_H
