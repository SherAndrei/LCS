#include <vector>

#include "table.hpp"

namespace lcs {

template<typename T>
class LCS {
 public:
    std::vector<T> find(const std::vector<T>& S1, const std::vector<T>& S2) const {
        const Table table(S1, S2);
        int index = table.at(S1.size(), S2.size());
        std::vector<T> lcs(index);

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

