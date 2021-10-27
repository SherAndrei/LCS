#include "LCS.hpp"

namespace lcs {

IntegralSequence
Find(const IntegralSequence& S1, const IntegralSequence& S2) {
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

#define PROBLEM_ONE 1
#define PROBLEM_TWO 1
#define PROBLEM_THREE 1

Mapper::Mapper(const FloatingTable& table)
    : _table(table) {
    size_t counter = 0;
#if PROBLEM_TWO
    size_t row_penalty = 1, column_penalty = 1;
#endif
    _mapping.reserve(_table.nrows() + _table.ncolumns());
    _map_func.reserve(_table.nrows() + _table.ncolumns());
    // (i + 1, j + 1), (i + 1, j), , (i, j + 1)
    std::array<double, 3> next_elems;

#if PROBLEM_THREE
    auto pos = std::min_element(
        table.begin(), table.begin() + table.ncolumns()
    );
    size_t i = 0, j = (pos - table.begin());
#else
    size_t i = 0, j = 0;
#endif
    for (; i < table.nrows() && j < table.ncolumns();) {
        _res_metric += _table.at(i, j);
        counter++;
        _map_func.emplace_back(_table.at(i, j));
        _mapping.emplace_back(i, j);
        if (i+1 == table.nrows()) {
#if PROBLEM_ONE
            break;
#else
            j++;
            continue;
#endif
        }
        if (j+1 == table.ncolumns()) {
#if PROBLEM_ONE
            break;
#else
            i++;
            continue;
#endif
        }
        next_elems = {
            table.at(i+1, j+1),
#if PROBLEM_TWO
            row_penalty * table.at(i+1, j),
            column_penalty * table.at(i, j+1)
#else
            table.at(i+1, j),
            table.at(i, j+1)
#endif
        };
        auto min_pos = std::min_element(next_elems.begin(), next_elems.end());
        if (min_pos == next_elems.begin()) {
#if PROBLEM_TWO
            row_penalty = column_penalty = 1;
#endif
            i++, j++;
        } else if (min_pos == std::next(next_elems.begin())) {
#if PROBLEM_TWO
            row_penalty++;
#endif
            i++;
        } else {
#if PROBLEM_TWO
            column_penalty++;
#endif
            j++;
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
