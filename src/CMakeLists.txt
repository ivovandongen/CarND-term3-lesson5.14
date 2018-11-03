add_library(core STATIC
    ${CMAKE_SOURCE_DIR}/src/hybrid_breadth_first.h
    ${CMAKE_SOURCE_DIR}/src/hybrid_breadth_first.cpp
    )

target_include_directories(core
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    )

if (DO_CLANG_TIDY)
    set_target_properties(core PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif ()