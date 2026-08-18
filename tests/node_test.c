#include <stdlib.h>
#include <string.h>

#include "unity.h"

#include "arena_memtable/node.h"

#include "../src/node_internal.h"


void setUp() {
}

void tearDown() {
}

static void is_a_null_node() {
    struct node const node = {
        .arena = nullptr,
        .offset = 0,
    };

    TEST_ASSERT_TRUE(is_null_node(node));
}

static void creates_new_node() {
    struct arena *arena = new_arena(100);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));

    TEST_ASSERT_NOT_EQUAL(ARENA_OFFSET_INVALID, node.offset);

    destroy_arena(arena);
}

static void creates_new_node_without_next_node() {
    struct arena *arena = new_arena(100);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));

    TEST_ASSERT_FALSE(has_next_node(node));

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
    struct slice const retrieved_key = key_of(node);

    TEST_ASSERT_EQUAL(strlen(key), retrieved_key.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        key,
        retrieved_key.data,
        retrieved_key.length
    );

    destroy_arena(arena);
}

static void gets_an_invalid_key_slice_from_an_invalid_node() {
    struct arena *arena = new_arena(100);

    struct node const node = {
        .arena = arena,
        .offset = ARENA_OFFSET_INVALID,
    };
    struct slice const retrieved_key = key_of(node);

    TEST_ASSERT_FALSE(is_valid_slice(retrieved_key));

    destroy_arena(arena);
}

static void gets_a_value() {
    struct arena *arena = new_arena(100);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));
    struct slice const retrieved_value = value_of(node);

    TEST_ASSERT_EQUAL(3, retrieved_value.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        value,
        retrieved_value.data,
        retrieved_value.length
    );

    destroy_arena(arena);
}

static void gets_an_invalid_value_slice_from_an_invalid_node() {
    struct arena *arena = new_arena(100);

    struct node const node = {
        .arena = arena,
        .offset = ARENA_OFFSET_INVALID,
    };
    struct slice const retrieved_value = value_of(node);

    TEST_ASSERT_FALSE(is_valid_slice(retrieved_value));

    destroy_arena(arena);
}

static void gets_next_node() {
    struct arena *arena = new_arena(100);
    const char *const key = "Storage";
    const char *const value = "LSM";

    struct node const node = new_node(arena, key, strlen(key), value, strlen(value));
    struct node const next_node = next_node_of(node);

    TEST_ASSERT_EQUAL(0, next_node.offset);

    destroy_arena(arena);
}

static void sets_the_next_node() {
    struct arena *arena = new_arena(100);

    const char *const first_key = "one";
    const char *const first_value = "one";
    struct node const first = new_node(arena, first_key, strlen(first_key), first_value, strlen(first_value));

    const char *const second_key = "one";
    const char *const second_value = "one";
    struct node const second = new_node(arena, second_key, strlen(second_key), second_value, strlen(second_value));

    set_next_node_of(first, second);

    struct node const next_node = next_node_of(first);
    TEST_ASSERT_EQUAL(second.offset, next_node.offset);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(second_key, key_of(second).data, key_of(second).length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(second_value, value_of(second).data, value_of(second).length);

    destroy_arena(arena);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(is_a_null_node);
    RUN_TEST(creates_new_node);
    RUN_TEST(creates_new_node_without_next_node);
    RUN_TEST(get_an_invalid_node);
    RUN_TEST(gets_a_valid_node);
    RUN_TEST(gets_a_key);
    RUN_TEST(gets_an_invalid_key_slice_from_an_invalid_node);
    RUN_TEST(gets_a_value);
    RUN_TEST(gets_an_invalid_value_slice_from_an_invalid_node);
    RUN_TEST(gets_next_node);
    RUN_TEST(sets_the_next_node);

    return UNITY_END();
}
