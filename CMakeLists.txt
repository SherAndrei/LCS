project(LCS CXX)
cmake_minimum_required(VERSION 3.12)

include(CMakeDependentOption)
cmake_policy(SET CMP0054 NEW)

set(CMAKE_CXX_STANDARD 17)

add_compile_options( -W  -Wall      -Werror         -pedantic
                     -Wextra        -Wfloat-equal   -pedantic-errors
                     -Wmissing-declarations -Wfatal-errors)

include_directories(utils)
add_subdirectory(lib)

find_package(GTest)
if (GTest_FOUND)
  include_directories(${GTest_INCLUDE_DIRS})
endif (GTest_FOUND)

# Add an option to exclude tests executable from build
# This is useful, because it allows to remove GTest dependency if a user is interested only in the
# LCS library itself and not in the tests
cmake_dependent_option(ENABLE_TESTS "Whether or not to build tests" ON "GTest_FOUND" OFF)

if (ENABLE_TESTS)
    add_subdirectory(tests)
endif(ENABLE_TESTS)
