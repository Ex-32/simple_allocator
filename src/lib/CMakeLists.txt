add_library(simple_malloc
    "simple_malloc.c"
)
set_target_properties(simple_malloc PROPERTIES
    VERSION "${PROJECT_VERSION}"
    SOVERSION 1
    PUBLIC_HEADER "simple_malloc.h"
)
target_include_directories(simple_malloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
