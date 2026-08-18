#include "unity.h"

#include "arena_memtable/memtable.h"
#include "arena_memtable/slice.h"

#include <string.h>

void setUp() {
}

void tearDown() {
}

static void puts_a_single_key_value_pair_and_gets_the_value() {
    struct memtable *memtable = new_memtable(2 << 10);
    const char *const key = "Storage";
    const char *const value = "LSM";

    TEST_ASSERT_TRUE(put(memtable, key, strlen(key), value, strlen(value)));
    struct slice const retrieved_value = get_value(memtable, key, strlen(key));

    TEST_ASSERT_EQUAL(3, retrieved_value.length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        value,
        retrieved_value.data,
        retrieved_value.length
    );

    destroy_memtable(memtable);
}

static void puts_a_few_key_value_pairs_with_overlapping_keys_and_gets_the_first_key() {
    struct memtable *memtable = new_memtable(2 << 10);

    TEST_ASSERT_TRUE(put(memtable, "Store", strlen("Store"), "Permanent", strlen("Permanent")));
    TEST_ASSERT_TRUE(put(memtable, "Storage", strlen("Storage"), "Disk", strlen("Disk")));
    TEST_ASSERT_TRUE(put(memtable, "StorageEngine", strlen("StorageEngine"), "LSM", strlen("LSM")));

    TEST_ASSERT_EQUAL_UINT8_ARRAY(
       "Permanent",
       get_value(memtable, "Store", strlen("Store")).data,
       get_value(memtable, "Store", strlen("Store")).length
   );

    destroy_memtable(memtable);
}

static void puts_a_few_key_value_pairs_with_overlapping_keys_and_gets_the_second_key() {
    struct memtable *memtable = new_memtable(2 << 10);

    TEST_ASSERT_TRUE(put(memtable, "Store", strlen("Store"), "Permanent", strlen("Permanent")));
    TEST_ASSERT_TRUE(put(memtable, "Storage", strlen("Storage"), "Disk", strlen("Disk")));
    TEST_ASSERT_TRUE(put(memtable, "StorageEngine", strlen("StorageEngine"), "LSM", strlen("LSM")));

    TEST_ASSERT_EQUAL_UINT8_ARRAY(
       "Disk",
       get_value(memtable, "Storage", strlen("Storage")).data,
       get_value(memtable, "Storage", strlen("Storage")).length
   );

    destroy_memtable(memtable);
}

static void puts_a_few_key_value_pairs_with_overlapping_keys_and_gets_the_third_key() {
    struct memtable *memtable = new_memtable(2 << 10);

    TEST_ASSERT_TRUE(put(memtable, "Store", strlen("Store"), "Permanent", strlen("Permanent")));
    TEST_ASSERT_TRUE(put(memtable, "Storage", strlen("Storage"), "Disk", strlen("Disk")));
    TEST_ASSERT_TRUE(put(memtable, "StorageEngine", strlen("StorageEngine"), "LSM", strlen("LSM")));

    TEST_ASSERT_EQUAL_UINT8_ARRAY(
       "LSM",
       get_value(memtable, "StorageEngine", strlen("StorageEngine")).data,
       get_value(memtable, "StorageEngine", strlen("StorageEngine")).length
   );

    destroy_memtable(memtable);
}

static void attempts_to_get_a_non_existent_key() {
    struct memtable *memtable = new_memtable(2 << 10);

    struct slice const retrieved_value = get_value(memtable, "NonExistent", strlen("NonExistent"));

    TEST_ASSERT_EQUAL(0, retrieved_value.length);

    destroy_memtable(memtable);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(puts_a_single_key_value_pair_and_gets_the_value);
    RUN_TEST(puts_a_few_key_value_pairs_with_overlapping_keys_and_gets_the_first_key);
    RUN_TEST(puts_a_few_key_value_pairs_with_overlapping_keys_and_gets_the_second_key);
    RUN_TEST(puts_a_few_key_value_pairs_with_overlapping_keys_and_gets_the_third_key);
    RUN_TEST(attempts_to_get_a_non_existent_key);

    return UNITY_END();
}
