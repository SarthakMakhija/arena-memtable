#include "unity.h"

#include "arena_memtable/sorted_list.h"

#include <string.h>

void setUp() {
}

void tearDown() {
}

static void puts_a_single_key_value_pair() {
    struct sorted_list *list = new_list(2 << 10);
    const char *const key = "Storage";
    const char *const value = "LSM";

    TEST_ASSERT_TRUE(put(list, key, strlen(key), value, strlen(value)));

    destroy_list(list);
}

static void puts_a_single_key_value_pair_and_gets_the_value() {
    struct sorted_list *list = new_list(2 << 10);
    const char *const key = "Storage";
    const char *const value = "LSM";

    TEST_ASSERT_TRUE(put(list, key, strlen(key), value, strlen(value)));
    struct slice const retrieved_value = get_value(list, key, strlen(key));

    TEST_ASSERT_EQUAL(3, retrieved_value.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        value,
        retrieved_value.data,
        retrieved_value.length
    );

    destroy_list(list);
}

static void puts_a_couple_of_key_value_pairs() {
    struct sorted_list *list = new_list(2 << 10);

    TEST_ASSERT_TRUE(put(list, "Storage", strlen("Storage"), "LSM", strlen("LSM")));
    TEST_ASSERT_TRUE(put(list, "Consensus", strlen("Consensus"), "Raft", strlen("Raft")));

    struct slice const retrieved_value = get_value(list, "Consensus", strlen("Consensus"));

    TEST_ASSERT_EQUAL(4, retrieved_value.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        "Raft",
        retrieved_value.data,
        retrieved_value.length
    );

    destroy_list(list);
}

static void puts_a_few_key_value_pairs_with_overlapping_keys() {
    struct sorted_list *list = new_list(2 << 10);

    TEST_ASSERT_TRUE(put(list, "Store", strlen("Store"), "Permanent", strlen("Permanent")));
    TEST_ASSERT_TRUE(put(list, "Storage", strlen("Storage"), "Disk", strlen("Disk")));
    TEST_ASSERT_TRUE(put(list, "StorageEngine", strlen("StorageEngine"), "LSM", strlen("LSM")));

    struct slice const retrieved_value = get_value(list, "StorageEngine", strlen("StorageEngine"));

    TEST_ASSERT_EQUAL(3, retrieved_value.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        "LSM",
        retrieved_value.data,
        retrieved_value.length
    );

    destroy_list(list);
}

static void attempts_to_get_a_non_existent_key() {
    struct sorted_list *list = new_list(2 << 10);

    struct slice const retrieved_value = get_value(list, "NonExistent", strlen("NonExistent"));

    TEST_ASSERT_EQUAL(0, retrieved_value.length);

    destroy_list(list);
}

static void puts_a_few_key_value_pairs() {
    struct sorted_list *list = new_list(2 << 10);

    for (int index = 1; index < 10; index++) {
        char key[32];
        char value[32];

        snprintf(key, sizeof(key), "Key_%d", index);
        snprintf(value, sizeof(value), "Value_%d", index);

        TEST_ASSERT_TRUE(put(list, key, strlen(key), value, strlen(value)));
    }

    for (int index = 1; index < 10; index++) {
        char key[32];
        snprintf(key, sizeof(key), "Key_%d", index);

        struct slice const retrieved_value = get_value(list, key, strlen(key));

        char expected_value[32];
        snprintf(expected_value, sizeof(expected_value), "Value_%d", index);

        TEST_ASSERT_EQUAL_UINT8_ARRAY(
            expected_value,
            retrieved_value.data,
            retrieved_value.length
        );
    }

    destroy_list(list);
}

static void puts_keys_in_reverse_order_and_gets_all_values() {
    struct sorted_list *list = new_list(2 << 10);

    for (int index = 9; index >= 1; index--) {
        char key[32];
        char value[32];

        snprintf(key, sizeof(key), "Key_%d", index);
        snprintf(value, sizeof(value), "Value_%d", index);

        TEST_ASSERT_TRUE(
            put(list, key, strlen(key), value, strlen(value))
        );
    }

    for (int index = 1; index <= 9; index++) {
        char key[32];
        snprintf(key, sizeof(key), "Key_%d", index);

        char expected_value[32];
        snprintf(expected_value, sizeof(expected_value), "Value_%d", index);

        struct slice const retrieved_value =
            get_value(list, key, strlen(key));

        TEST_ASSERT_EQUAL(strlen(expected_value), retrieved_value.length);

        TEST_ASSERT_EQUAL_UINT8_ARRAY(
            expected_value,
            retrieved_value.data,
            retrieved_value.length
        );
    }

    destroy_list(list);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(puts_a_single_key_value_pair);
    RUN_TEST(puts_a_single_key_value_pair_and_gets_the_value);
    RUN_TEST(puts_a_couple_of_key_value_pairs);
    RUN_TEST(puts_a_few_key_value_pairs_with_overlapping_keys);
    RUN_TEST(attempts_to_get_a_non_existent_key);
    RUN_TEST(puts_a_few_key_value_pairs);
    RUN_TEST(puts_keys_in_reverse_order_and_gets_all_values);

    return UNITY_END();
}
