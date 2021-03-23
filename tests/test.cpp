#include <iostream>
#include <vector>

#include "test_runner.h"
#include "profile.h"
#include "LCS.hpp"

void TestLoad();
void TestString();

void TestString() {
    // {
    //     std::vector<char> s1 = { 'A', 'C', 'B'};
    //     std::vector<char> s2 = { 'C', 'B', 'A'};
    //     ASSERT_EQUAL(lcs::LCS(s1, s2), std::vector<char>({'C', 'D', 'B'}));
    // }
    {
        lcs::LCS_t<char> lsc;
        std::vector<char> s1 = { 'A', 'C', 'B'};
        std::vector<char> s2 = { 'C', 'B', 'A'};
        ASSERT_EQUAL(lsc.find(s1, s2), std::vector<char>({'C', 'B'}));
    }
    {
        lcs::LCS_t<char> lsc;
        std::vector<char> s1 = { 'A', 'C', 'A', 'D', 'B'};
        std::vector<char> s2 = { 'C', 'Q', 'D', 'B', 'A'};
        ASSERT_EQUAL(lsc.find(s1, s2), std::vector<char>({'C', 'D', 'B'}));
    }
}

void TestLoad() {
    std::vector<char> s1(7000, 'A');
    std::vector<char> s2(7000);
    for (int i = 0; i < 7000; i++) {
        s2[i] = i % 2 ? 'A': 'B';
    }
    std::vector<char> expected(3500, 'A');
    std::vector<char> result;
    {
        LOG_DURATION("standart_alg");
        result = lcs::LCS<char>(s1, s2);
    }
    ASSERT_EQUAL(result, expected);
    {
        LOG_DURATION("class_alg");
        lcs::LCS_t<char> lcs;
        result = lcs.find(s1, s2);
    }
    ASSERT_EQUAL(result, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestString);
    RUN_TEST(tr, TestLoad);
}
