cmake_minimum_required(VERSION 4.1)
project(arena_memtable C)

set(CMAKE_C_STANDARD 23)

add_library(arena_memtable STATIC
        src/arena.c
        src/arena_internal.h
        src/node.c
        src/slice.c)

target_include_directories(arena_memtable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


# ---- Tests ----

include(FetchContent)

FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.6.1
)

FetchContent_MakeAvailable(unity)

enable_testing()

add_executable(arena_tests tests/arena_test.c)
add_executable(node_tests tests/node_test.c)

target_link_libraries(arena_tests
        PRIVATE
        arena_memtable
        unity
)
target_link_libraries(node_tests
        PRIVATE
        arena_memtable
        unity
)

add_test(
        NAME arena_tests
        COMMAND arena_tests
)

add_test(
        NAME node_tests
        COMMAND node_tests
)