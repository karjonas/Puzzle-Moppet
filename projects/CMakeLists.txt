option(BUILD_TESTS "Build the test suite" OFF)

add_subdirectory(Puzzle)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
