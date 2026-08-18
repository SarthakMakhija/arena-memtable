#include "unity.h"

#include "arena_memtable/node.h"

#include <stdlib.h>
#include <string.h>


void setUp() {
}

void tearDown() {
}

static void creates_new_node() {
    struct arena *arena = new_arena(100);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));

    TEST_ASSERT_NOT_EQUAL(ARENA_OFFSET_INVALID, node.offset);

    destroy_arena(arena);
}

static void get_an_invalid_node() {
    struct arena *arena = new_arena(5);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));

    TEST_ASSERT_EQUAL(ARENA_OFFSET_INVALID, node.offset);

    destroy_arena(arena);
}

static void gets_a_valid_node() {
    struct arena *arena = new_arena(100);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));
    TEST_ASSERT_TRUE(is_valid_node(node));

    destroy_arena(arena);
}

static void gets_a_key() {
    struct arena *arena = new_arena(100);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));
    struct slice const retrieved = key_from(node);

    TEST_ASSERT_EQUAL(7, retrieved.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        key,
        retrieved.data,
        retrieved.length
    );

    destroy_arena(arena);
}

static void gets_an_invalid_key_slice_from_an_invalid_node() {
    struct arena *arena = new_arena(100);

    struct node const node = {
        .arena = arena,
        .offset = ARENA_OFFSET_INVALID,
    };
    struct slice const retrieved = key_from(node);

    TEST_ASSERT_FALSE(is_valid_slice(retrieved));

    destroy_arena(arena);
}

static void gets_a_value() {
    struct arena *arena = new_arena(100);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));
    struct slice const retrieved = value_from(node);

    TEST_ASSERT_EQUAL(3, retrieved.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        value,
        retrieved.data,
        retrieved.length
    );

    destroy_arena(arena);
}

static void gets_an_invalid_value_slice_from_an_invalid_node() {
    struct arena *arena = new_arena(100);

    struct node const node = {
        .arena = arena,
        .offset = ARENA_OFFSET_INVALID,
    };
    struct slice const retrieved = value_from(node);

    TEST_ASSERT_FALSE(is_valid_slice(retrieved));

    destroy_arena(arena);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(creates_new_node);
    RUN_TEST(get_an_invalid_node);
    RUN_TEST(gets_a_valid_node);
    RUN_TEST(gets_a_key);
    RUN_TEST(gets_an_invalid_key_slice_from_an_invalid_node);
    RUN_TEST(gets_a_value);
    RUN_TEST(gets_an_invalid_value_slice_from_an_invalid_node);

    return UNITY_END();
}
