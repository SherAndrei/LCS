#include <iostream>
#include <vector>

#include "test_runner.h"
#include "profile.h"
#include "LCS.hpp"

void TestString();

void TestString() {
    {
        lcs::LCS<char> lsc;
        std::vector<char> s1 = { 'A', 'C', 'B'};
        std::vector<char> s2 = { 'C', 'B', 'A'};
        ASSERT_EQUAL(lsc.find(s1, s2), std::vector<char>({'C', 'B'}));
    }
    {
        lcs::LCS<char> lsc;
        std::vector<char> s1 = { 'A', 'C', 'A', 'D', 'B'};
        std::vector<char> s2 = { 'C', 'Q', 'D', 'B', 'A'};
        ASSERT_EQUAL(lsc.find(s1, s2), std::vector<char>({'C', 'D', 'B'}));
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestString);
}
