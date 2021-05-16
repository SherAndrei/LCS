#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <string_view>

#include "test_runner.h"
#include "profile.h"

using sequence_t = char;
using Sequence = std::vector<sequence_t>;

class IntegralTable final {
 public:
    IntegralTable() = default;
    IntegralTable(const Sequence& S1,
                  const Sequence& S2)
        : _rows(S1.size() + 1)
        , _columns(S2.size() + 1)
        , _table(std::make_unique<uint32_t[]>(_rows * _columns)) {
            fill(S1, S2);
    }

    void fill(const Sequence& S1, const Sequence& S2) {
        _rows = S1.size() + 1;
        _columns = S2.size() + 1;
        _table = std::make_unique<uint32_t[]>(_rows * _columns);

        size_t i, j;
        for (i = 0u; i <= _rows - 1; ++i) {
            for (j = 0u; j <= _columns - 1; ++j) {
                if (i * j == 0u)
                    at(i, j) = 0u;
                else if (S1[i - 1] == S2[j - 1])
                    at(i, j) = at(i - 1, j - 1) + 1;
                else
                    at(i, j) = std::max(at(i - 1, j), at(i, j - 1));
            }
        }
    }

    uint32_t& at(size_t i, size_t j) {
        return _table.get()[i * _columns + j];
    }

    uint32_t at(size_t i, size_t j) const {
        return _table.get()[i * _columns + j];
    }

 protected:
    virtual std::ostream& print(std::ostream& os) const {
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _columns; ++j)
                os << at(i, j) << " \n"[j == _columns - 1];
        }
        return os;
    }

 public:
    friend std::ostream& operator<<(std::ostream& os, const IntegralTable& t) {
        return t.print(os);
    }

 private:
    size_t _rows;
    size_t _columns;
    std::unique_ptr<uint32_t[]> _table;
};

Sequence
find(const Sequence& S1, const Sequence& S2) {
    const IntegralTable table(S1, S2);
    size_t index = table.at(S1.size(), S2.size());
    Sequence lcs(index);

    size_t i = S1.size(), j = S2.size();
    while (i > 0 && j > 0) {
        if (S1[i - 1] == S2[j - 1]) {
            lcs[index - 1] = S1[i - 1];
            i--, j--, index--;
        } else if (table.at(i - 1, j) > table.at(i, j - 1)) {
            i--;
        } else {
            j--;
        }
    }

    return lcs;
}

void do_test(
    const std::vector<sequence_t>& s1,
    const std::vector<sequence_t>& s2,
    const std::vector<sequence_t>& expected,
    bool do_table = false) {
    std::vector<sequence_t> result;
    if (do_table) {
        IntegralTable it;
        {
            LOG_DURATION("table filling");
            it.fill(s1, s2);
        }
        std::cout << it << std::endl;
    }
    {
        LOG_DURATION("finding sequence");
        result = find(s1, s2);
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

int main() {
    TestRunner tr;
    RUN_TEST(tr, Simple);
    RUN_TEST(tr, EqualSequences);
    RUN_TEST(tr, TestSubsequence);
}
