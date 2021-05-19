#ifndef LCS_HPP
#define LCS_HPP

#include <vector>
#include <utility>

#include "table.hpp"

namespace lcs {

using IntegralSequence = IntegralTable::Sequence;

IntegralSequence
find(const IntegralSequence& S1, const IntegralSequence& S2);

using FloatingSequence = FloatingTable::Sequence;

class Mapper {
 public:
    using mapping_t = std::vector<std::pair<size_t, size_t>>;

 public:
    Mapper(const FloatingSequence& S1, const FloatingSequence& S2);

    mapping_t mapping();

 public:
    const FloatingTable& table() const;
    double res_metric() const;

 private:
    // TODO remove s1 and s2 references
    const FloatingSequence& _S1;
    const FloatingSequence& _S2;
    const FloatingTable _table;
    double _res_metric;
};

}  // namespace lcs


#endif  // LCS_HPP
