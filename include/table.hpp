#ifndef TABLES_HPP
#define TABLES_HPP

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace lcs {

template<typename sequence_t, typename node_t>
class BaseTable {
 public:
    using Sequence = std::vector<sequence_t>;
    using Sequence_t = sequence_t;

 protected:
    BaseTable() = default;

    BaseTable(const Sequence& S1,
              const Sequence& S2)
        : _rows(S1.size())
        , _columns(S2.size())
        , _table(std::make_unique<node_t[]>(_rows * _columns)) {}

    virtual void fill(const Sequence& S1,
                      const Sequence& S2) = 0;

    virtual ~BaseTable() = default;

 protected:
    node_t& at(size_t i, size_t j) {
        return _table.get()[i * _columns + j];
    }

 public:
    node_t at(size_t i, size_t j) const {
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
    friend std::ostream& operator<<(std::ostream& os, const BaseTable& t) {
        return t.print(os);
    }

 protected:
    size_t _rows;
    size_t _columns;
    std::unique_ptr<node_t[]> _table;
};


class IntegralTable final : public BaseTable<char, uint32_t>{
 public:
    using BaseTable::Sequence;
    using BaseTable::Sequence_t;

 public:
    IntegralTable() = default;
    IntegralTable(const Sequence& S1,
                  const Sequence& S2)
        : BaseTable(S1, S2) {
            fill(S1, S2);
    }

    void fill(const Sequence& S1, const Sequence& S2) override {
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
};

}  // namespace lcs


#endif  // TABLES_HPP
