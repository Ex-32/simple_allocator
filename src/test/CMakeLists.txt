
add_executable(simple_malloc_test
    "./simple_malloc_test.c"
)
target_link_libraries(simple_malloc_test LINK_PUBLIC simple_malloc)
