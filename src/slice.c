#include "arena_memtable/slice.h"

struct slice new_invalid_slice() {
    return (struct slice) {
        .data = nullptr,
        .length = 0,
    };
}

bool is_valid_slice(struct slice const slice) {
    return slice.data != nullptr;
}
