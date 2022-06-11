cmake_minimum_required(VERSION 3.11)
set(C_STANDARD_REQUIRED c11)
project(simple_malloc VERSION 1.0.0 DESCRIPTION "A simple memory allocator like `malloc()` and `free()`")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory("./src/lib")
add_subdirectory("./src/test")
