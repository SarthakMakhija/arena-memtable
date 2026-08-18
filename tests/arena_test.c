#include "unity.h"

#include "arena_memtable/arena.h"

#include <stdlib.h>

void setUp() {
}

void tearDown() {
}

static void creates_new_arena() {
    struct arena *arena = new_arena(100);

    TEST_ASSERT_NOT_NULL(arena);

    destroy_arena(arena);
}

static void first_allocation_returns_non_zero_offset() {
    struct arena *arena = new_arena(100);

    arena_offset const offset = allocate(arena, 10);

    TEST_ASSERT_EQUAL_INT64(1, offset);

    destroy_arena(arena);
}

static void multiple_allocations_return_unique_offsets() {
    struct arena *arena = new_arena(100);

    TEST_ASSERT_EQUAL_INT64(1, allocate(arena, 10));
    TEST_ASSERT_EQUAL_INT64(11, allocate(arena, 20));
    TEST_ASSERT_EQUAL_INT64(31, allocate(arena, 30));

    destroy_arena(arena);
}

static void allocation_beyond_arena_capacity_returns_invalid_offset() {
    struct arena *arena = new_arena(10);

    TEST_ASSERT_EQUAL_INT64(ARENA_OFFSET_INVALID, allocate(arena, 11));

    destroy_arena(arena);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(creates_new_arena);
    RUN_TEST(first_allocation_returns_non_zero_offset);
    RUN_TEST(multiple_allocations_return_unique_offsets);
    RUN_TEST(allocation_beyond_arena_capacity_returns_invalid_offset);

    return UNITY_END();
}
