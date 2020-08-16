cmake_minimum_required(VERSION 3.10)

project(timer)

# Library

set(SRC
    src/timer.c)

add_library(timer ${SRC})

target_include_directories(timer PUBLIC include)

target_compile_options(timer PRIVATE -Wall -Wextra)

# Test

add_executable(timer_test
  test/timer_test.c)

target_link_libraries(timer_test timer)

target_compile_options(timer_test PRIVATE -DUNIT_TEST -DNDEBUG -Wall -Wextra)
