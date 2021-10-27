#include "LCS.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

struct IntegralSequenceTest : ::testing::Test
{
    void TearDown() override
    {
        const auto result = lcs::Find(lhs, rhs);
        ASSERT_THAT(result, ::testing::SizeIs(expected.size()));
        EXPECT_EQ(result, expected);
    }

    lcs::IntegralSequence lhs;
    lcs::IntegralSequence rhs;
    lcs::IntegralSequence expected;
};

TEST_F(IntegralSequenceTest, Chars)
{
    lhs = lcs::IntegralSequence{'A', 'C', 'B'};
    rhs = lcs::IntegralSequence{'C', 'B', 'A'};
    expected = lcs::IntegralSequence{'C', 'B'};
}

TEST_F(IntegralSequenceTest, EqualSequence)
{
    lhs = lcs::IntegralSequence(2048, 'a');
    rhs = lhs;
    expected = lhs;
}

TEST_F(IntegralSequenceTest, Subsequence)
{
    expected = lcs::IntegralSequence(48, 'a');
    lhs = lcs::IntegralSequence(2048, 'a');
    lcs::IntegralSequence nonmathcing_seq(1000, 'b');
    rhs.reserve(expected.size() + nonmathcing_seq.size() * 2);
    rhs.insert(rhs.end(), nonmathcing_seq.begin(), nonmathcing_seq.end());
    rhs.insert(rhs.end(), expected.begin(), expected.end());
    rhs.insert(rhs.end(), nonmathcing_seq.begin(), nonmathcing_seq.end());
}

TEST_F(IntegralSequenceTest, AlternatingSubsequence)
{
    lhs = lcs::IntegralSequence(2048, 'a');
    rhs = lcs::IntegralSequence(lhs.size());
    for (lcs::IntegralSequence::size_type i = 0; i < rhs.size(); i++)
        rhs[i] = (i % 2) ? 'a' : 'b';
    expected = lcs::IntegralSequence(1024, 'a');
}
