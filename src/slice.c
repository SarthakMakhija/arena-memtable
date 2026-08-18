#include "arena_memtable/slice.h"

/**
 * @brief Creates an invalid slice.
 *
 * Initializes the slice structure with a nullptr data pointer and zero length.
 *
 * @return An invalid struct slice.
 */
struct slice new_invalid_slice() {
    return (struct slice) {
        .data = nullptr,
        .length = 0,
    };
}

/**
 * @brief Checks if a slice is valid.
 *
 * Verifies that the slice's data pointer is not null (nullptr).
 *
 * @param slice The slice to validate.
 * @return true if the data pointer is non-null, false otherwise.
 */
bool is_valid_slice(struct slice const slice) {
    return slice.data != nullptr;
}