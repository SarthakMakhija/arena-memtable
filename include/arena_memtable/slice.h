#ifndef ARENA_MEMTABLE_SLICE_H
#define ARENA_MEMTABLE_SLICE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file slice.h
 * @brief Read-only view into a sequence of contiguous bytes.
 *
 * A slice encapsulates a pointer to constant data and its length.
 * It is commonly used to return read-only views of key and value payloads
 * stored inside the memory arena, avoiding expensive memory copies.
 */

/**
 * @brief Represents a read-only contiguous sequence of bytes.
 */
struct slice {
    char const* data;  /**< Pointer to the start of the read-only byte sequence. */
    uint32_t length;   /**< Length of the byte sequence in bytes. */
};

/**
 * @brief Creates an invalid slice.
 *
 * An invalid slice has a null data pointer and a length of 0. It is typically
 * used to signify that a key lookup failed or that an error occurred.
 *
 * @return An invalid struct slice instance.
 */
struct slice new_invalid_slice();

/**
 * @brief Checks if a slice is valid.
 *
 * A slice is considered valid if its data pointer is not null.
 *
 * @param slice The slice to validate.
 * @return true if the slice points to valid data (non-null), false otherwise.
 */
bool is_valid_slice(struct slice slice);

#endif //ARENA_MEMTABLE_SLICE_H