#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>

namespace lcs {

template<typename node_type>
class BaseTable {
 protected:
    template<typename T>
    BaseTable(const std::vector<T>& S1,
              const std::vector<T>& S2)
        : _rows(S1.size() + 1)
        , _columns(S2.size() + 1)
        , _table(std::make_unique<node_type[]>(_rows * _columns)) {}

 protected:
    node_type& at(size_t i, size_t j) {
        return _table.get()[i * _columns + j];
    }

 public:
    node_type at(size_t i, size_t j) const {
        return _table.get()[i * _columns + j];
    }

 protected:
    const size_t _rows;
    const size_t _columns;
    std::unique_ptr<node_type[]> _table;
};

class IntegralTable final : public BaseTable<uint16_t> {
 public:
    template<typename T,
         std::enable_if_t<std::is_integral_v<T>, bool> = true>
    IntegralTable(const std::vector<T>& S1,
                  const std::vector<T>& S2)
        : BaseTable(S1, S2) {
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
};

// class FloatingTable

}  // namespace lcs
