#include <vector>

#include "table.hpp"

namespace lcs {

template<typename T>
using Sequence = std::vector<T>;


class LCS {
 public:
    template<typename T,
    std::enable_if_t<std::is_integral_v<T>, bool> = true>
    std::vector<T>
    find(const Sequence<T>& S1, const Sequence<T>& S2) const {
        const IntegralTable table(S1, S2);
        // Length of the longest subsequence
        size_t index = table.at(S1.size(), S2.size());
        Sequence<T> lcs(index);

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

}  // namespace lcs

