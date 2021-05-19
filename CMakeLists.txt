cmake_minimum_required(VERSION 3.10)
project(LCS)

set(CMAKE_CXX_STANDARD 17)

add_compile_options( -W  -Wall      -Werror         -pedantic
                     -Wextra        -Wfloat-equal   -pedantic-errors
                     -Wcast-align   -Wpointer-arith -Wformat-security
                     -Wformat       -Wwrite-strings -Wcast-align
                     -Wno-long-long -Wunused        -Wcast-qual
                     -Wmissing-declarations -Wfatal-errors
                     -fstack-protector-all)

include_directories(utils)
add_subdirectory(lib)
add_subdirectory(tests)
