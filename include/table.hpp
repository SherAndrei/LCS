#ifndef TABLES_HPP
#define TABLES_HPP

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace lcs {

template<typename S, typename N>
class BaseTable {
 public:
    using Sequence   = std::vector<S>;
    using sequence_t = S;
    using node_t     = N;

 public:
    using iterator = node_t*;
    using const_iterator = const node_t*;

 protected:
    BaseTable(size_t nrows, size_t ncolumns)
        : _rows(nrows)
        , _columns(ncolumns)
        , _table(std::make_unique<node_t[]>(_rows * _columns)) {
            if (nrows == 0 || ncolumns == 0)
                throw std::runtime_error("Invalid table size");
        }

    virtual ~BaseTable() = default;

 protected:
    node_t& at(size_t i, size_t j) {
        return _table.get()[i * _columns + j];
    }

 public:
    iterator begin() { return _table.get(); }
    iterator end()   { return _table.get() + _rows * _columns; }
    const_iterator begin() const { return _table.get(); }
    const_iterator end()   const { return _table.get() + _rows * _columns; }

    size_t nrows()    const { return _rows; }
    size_t ncolumns() const { return _columns; }

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
    using BaseTable::sequence_t;
    using BaseTable::node_t;

 public:
    IntegralTable(const Sequence& S1, const Sequence& S2)
        : BaseTable(S1.size() + 1, S2.size() + 1) {
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

class FloatingTable final : public BaseTable<double, double> {
 public:
    using BaseTable::Sequence;
    using BaseTable::sequence_t;
    using BaseTable::node_t;

 public:
    FloatingTable(const Sequence& BiggerSequence,
                  const Sequence& SmallerSequence)
        : BaseTable(SmallerSequence.size(), BiggerSequence.size()) {
        if (BiggerSequence.size() < SmallerSequence.size())
            throw std::runtime_error("S1 size must be greater");
        size_t i, j;
        for (i = 0u; i < _rows; ++i) {
            for (j = 0u; j < _columns; ++j) {
                at(i, j) = metric(SmallerSequence[i], BiggerSequence[j]);
            }
        }
    }

    static double metric(double x_i, double y_j) {
        return std::pow(x_i - y_j, 2);
    }
};


}  // namespace lcs


#endif  // TABLES_HPP
