#include "unity.h"

#include "arena_memtable/slice.h"

#include <stdlib.h>
#include <string.h>

#include "../cmake-build-debug/_deps/unity-src/src/unity.h"


void setUp() {
}

void tearDown() {
}

static void creates_an_invalid_slice() {
    struct slice const slice = new_invalid_slice();

    TEST_ASSERT_NULL(slice.data);
    TEST_ASSERT_EQUAL(0, slice.length);
}

static void is_an_invalid_slice() {
    struct slice const slice = new_invalid_slice();

    TEST_ASSERT_FALSE(is_valid_slice(slice));
}

static void is_a_valid_slice() {
    struct slice const slice = (struct slice){
        .data = "LSM",
        .length = 3,
    };

    TEST_ASSERT_TRUE(is_valid_slice(slice));
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(creates_an_invalid_slice);
    RUN_TEST(is_an_invalid_slice);
    RUN_TEST(is_a_valid_slice);

    UNITY_END();
}
