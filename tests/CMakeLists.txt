link_libraries(LCS)

enable_testing()
find_package(GTest CONFIG REQUIRED)

include(GoogleTest)
link_libraries(GTest::gtest GTest::gmock_main)

add_executable(integral_tests.out integral_tests.cpp)
gtest_discover_tests(integral_tests.out)

add_executable(floating_tests.out floating_tests.cpp)
