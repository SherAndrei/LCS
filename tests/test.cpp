#include <iostream>
#include <vector>

#include "test_runner.h"
#include "LCS.hpp"

void TestString();

void TestString() {
    std::vector<char> s1 = { 'A', 'C', 'A', 'D', 'B'};
    std::vector<char> s2 = { 'C', 'Q', 'D', 'B', 'A'};
    ASSERT_EQUAL(lcs::LCS<char>(s1, s2), std::vector<char>({'C', 'D', 'B'}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestString);
}
