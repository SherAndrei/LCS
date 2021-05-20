#include "LCS.hpp"

namespace lcs {

IntegralSequence
find(const IntegralSequence& S1, const IntegralSequence& S2) {
    const IntegralTable table(S1, S2);
    size_t index = table.at(S1.size(), S2.size());
    IntegralSequence lcs(index);

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

using mapping_t = Mapper::mapping_t;

Mapper::Mapper(const FloatingTable& table)
    : _table(table) {
    size_t counter = 0;
    _mapping.reserve(_table.nrows() + _table.ncolumns());
    _map_func.reserve(_table.nrows() + _table.ncolumns());
    // (i + 1, j + 1), (i + 1, j), , (i, j + 1)
    std::array<double, 3> next_elems;

    // auto [row_begin, row_end] = std::make_pair(_table.begin(), _table.begin() + _table.ncolumns());
    // size_t starting_point = std::min_element(row_begin, row_end) - row_begin;

    for (size_t i = 0, j = 0;;) {
        _res_metric += _table.at(i, j);
        counter++;
        _map_func.emplace_back(_table.at(i, j));
        _mapping.emplace_back(i, j);
        if (i + 1 < _table.nrows() && j + 1 < _table.ncolumns()) {
            next_elems = { _table.at(i+1, j+1), _table.at(i+1, j), _table.at(i, j+1)};
            auto pos = std::min_element(next_elems.begin(), next_elems.end());

            if (pos == next_elems.begin()) {
                i++, j++;
            } else if (pos == next_elems.begin() + 2) {
                j++;
            } else {
                i++;
            }
        } else if (i + 1 == _table.nrows() && j + 1 < _table.ncolumns()) {
            j++;
        } else if (i + 1 < _table.nrows() && j + 1 == _table.ncolumns()) {
            i++;
        } else if (i + 1 == _table.nrows() && j + 1 == _table.ncolumns()) {
            break;
        }
    }
    _res_metric /= counter;
}

std::vector<double> Mapper::map_function() const {
    return _map_func;
}

const mapping_t& Mapper::mapping() const {
    return _mapping;
}

const FloatingTable& Mapper::table() const {
    return _table;
}

double Mapper::res_metric() const {
    return _res_metric;
}

}  // namespace lcs
