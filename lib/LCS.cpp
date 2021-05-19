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

Mapper::Mapper(const FloatingSequence& S1, const FloatingSequence& S2)
    : _S1(S1), _S2(S2), _table(S1, S2), _res_metric(0.) {}

mapping_t Mapper::mapping() {
    size_t counter = 0;
    mapping_t mapping;
    mapping.reserve(std::min(_S1.size(), _S2.size()));
    // (i + 1, j) (i + 1, j + 1), (i, j + 1)
    std::array<double, 3> next_elems;

    auto [row_begin, row_end] = std::make_pair(_table.begin(), _table.begin() + _table.ncolumns());
    size_t starting_point = std::min_element(row_begin, row_end) - row_begin;

    for (size_t i = 0, j = starting_point;;) {
        _res_metric += _table.at(i, j);
        counter++;
        mapping.emplace_back(i, j);
        if (i + 1 < _S1.size() && j + 1 < _S2.size()) {
            next_elems = {_table.at(i+1, j), _table.at(i+1, j+1), _table.at(i, j+1)};
            auto pos = std::min_element(next_elems.begin(), next_elems.end());

            if (pos == next_elems.begin()) {
                i++;
            } else if (pos == next_elems.begin() + 2) {
                j++;
            } else {
                i++, j++;
            }
        } else if (i + 1 == _S1.size() && j + 1 < _S2.size()) {
            j++;
        } else if (j + 1 == _S2.size() && i + 1 < _S1.size()) {
            i++;
        } else if (i + 1 == _S1.size() && j + 1 == _S2.size()) {
            break;
        }
    }
    _res_metric /= counter;
    return mapping;
}

const FloatingTable& Mapper::table() const {
    return _table;
}

double Mapper::res_metric() const {
    return _res_metric;
}

}  // namespace lcs
