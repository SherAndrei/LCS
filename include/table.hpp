#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>

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

}  // namespace lcs
