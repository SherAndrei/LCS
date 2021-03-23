#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>

namespace lcs {

class Table {
using node_t = uint16_t;

 public:
    template<typename T>
    Table(const std::vector<T>& S1,
          const std::vector<T>& S2)
        : _rows(S1.size() + 1)
        , _columns(S2.size() + 1)
        , _table(std::make_unique<node_t[]>(_rows * _columns)) {
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

 private:
    node_t& at(size_t i, size_t j) {
        return _table.get()[i * _columns + j];
    }

 public:
    node_t at(size_t i, size_t j) const {
        return _table.get()[i * _columns + j];
    }

 private:
    const size_t _rows;
    const size_t _columns;
    std::unique_ptr<node_t[]> _table;
};

template<typename T>
class LCS_t {
 public:
    std::vector<T> find(const std::vector<T>& S1, const std::vector<T>& S2) const {
        const Table table(S1, S2);
        int index = table.at(S1.size(), S2.size());
        std::vector<T> lcs(index);

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
};




template<typename T,
        std::enable_if_t<std::is_integral<T>::value, bool> = true>
std::vector<T> LCS(const std::vector<T>& S1, const std::vector<T>& S2) {
    std::vector<std::vector<int>> table(S1.size() + 1,
                                    std::vector<int>(S2.size() + 1));
    size_t i, j;

    for (i = 0u; i <= S1.size(); ++i) {
        for (j = 0u; j <= S2.size(); ++j) {
            if (i * j == 0u)
                table[i][j] = 0u;
            else if (S1[i - 1] == S2[j - 1])
                table[i][j] = table[i - 1][j - 1] + 1;
            else
                table[i][j] = std::max(table[i - 1][j], table[i][j - 1]);
        }
    }

    int index = table[S1.size()][S2.size()];
    std::vector<T> lcs(index);

    i = S1.size(), j = S2.size();
    while (i > 0 && j > 0) {
        if (S1[i - 1] == S2[j - 1]) {
          lcs[index - 1] = S1[i - 1];
          i--, j--, index--;
        } else if (table[i - 1][j] > table[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    return lcs;
}

}  // namespace lcs

