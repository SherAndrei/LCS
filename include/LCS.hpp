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
    using mapping_t = std::vector<std::pair<std::size_t, std::size_t>>;

 public:
    explicit Mapper(const FloatingTable& table);
    const mapping_t& mapping() const;
    std::vector<double> map_function() const;

 public:
    const FloatingTable& table() const;
    double res_metric() const;

 private:
    const FloatingTable& _table;
    mapping_t _mapping = {};
    std::vector<double> _map_func = {};
    double _res_metric = 0.;
};

}  // namespace lcs


#endif  // LCS_HPP
