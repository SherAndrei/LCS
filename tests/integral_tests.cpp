#include <vector>

#include "table.hpp"
#include "LCS.hpp"

#include "test_runner.h"
#include "profile.h"


namespace {

using sequence_t = typename lcs::IntegralTable::sequence_t;
using Sequence = typename lcs::IntegralTable::Sequence;

void do_test(
    const std::vector<sequence_t>& s1,
    const std::vector<sequence_t>& s2,
    const std::vector<sequence_t>& expected,
    bool do_table = false) {
    std::vector<sequence_t> result;
    if (do_table) {
        LogDuration timer("table filling");
        lcs::IntegralTable it(s1, s2);
        timer.~LogDuration();

        std::cout << it << std::endl;
    }
    {
        LOG_DURATION("finding sequence");
        result = lcs::find(s1, s2);
    }
    ASSERT_EQUAL(result, expected);
}

void Simple() {
    {
        do_test({ 'A', 'C', 'B'}, { 'C', 'B', 'A'}, {'C', 'B'}, true);
    }
}

void EqualSequences() {
    {
        std::string str1(2048, 'a');
        std::string str2(2048, 'a');

        do_test(std::vector<sequence_t>(str1.begin(), str1.end()),
                std::vector<sequence_t>(str2.begin(), str2.end()),
                std::vector<sequence_t>(2048, 'a'));
    }
}

void TestSubsequence() {
    {
        std::string str1(2048, 'a');
        std::string str2(1000, 'b');
        str2 += std::string(48, 'a');
        str2 += std::string(1000, 'b');

        do_test(std::vector<sequence_t>(str1.begin(), str1.end()),
                std::vector<sequence_t>(str2.begin(), str2.end()),
                std::vector<sequence_t>(48, 'a'));
    }
    {
        std::string str1(2048, 'a');
        std::string str2(2048, '\0');
        for (int i = 0; i < 2048; i++) {
            str2[i] = (i%2) ? 'a' : 'b';
        }
        do_test(
            std::vector<sequence_t>(str2.begin(), str2.end()),
            std::vector<sequence_t>(str1.begin(), str1.end()),
            std::vector<sequence_t>(1024, 'a')
        );
    }
    {
        std::string str1(2048, 'a');
        std::string str2(2048, '\0');
        for (int i = 0; i < 2048; i++) {
            str2[i] = (i%2) ? 'a' : 'b';
        }
        do_test(
            std::vector<sequence_t>(str2.begin(), str2.end()),
            std::vector<sequence_t>(str1.begin(), str1.end()),
            std::vector<sequence_t>(1024, 'a'));
    }
}

}  // namespace

int main() {
    TestRunner tr;
    RUN_TEST(tr, Simple);
    RUN_TEST(tr, EqualSequences);
    RUN_TEST(tr, TestSubsequence);
}
