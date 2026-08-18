#ifndef ARENA_MEMTABLE_MEMTABLE_H
#define ARENA_MEMTABLE_MEMTABLE_H
#include <stdint.h>

struct memtable;

struct memtable *new_memtable(int32_t capacity);

bool put(struct memtable const *memtable, const char *key, uint16_t key_length, const char *value, uint32_t value_length);

struct slice get_value(struct memtable const *memtable, char const *key, uint16_t key_length);

void destroy_memtable(struct memtable *memtable);

#endif //ARENA_MEMTABLE_MEMTABLE_H
