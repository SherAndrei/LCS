#include <vector>
#include <algorithm>

namespace lcs {

template<typename T,
        std::enable_if_t<std::is_integral<T>::value, bool> = true>
std::vector<T> LCS(const std::vector<T>& S1, const std::vector<T>& S2) {
    std::vector<std::vector<int>> table(S1.size() + 1,
                                    std::vector<int>(S2.size() + 1));
    size_t i, j;

    for (i = 0u; i <= S1.size(); ++i) {
        for (j = 0u; j <= S2.size(); ++j) {
            if (i * j == 0u)
                table[i][j] = 0u;
            else if (S1[i - 1] == S2[j - 1])
                table[i][j] = table[i - 1][j - 1] + 1;
            else
                table[i][j] = std::max(table[i - 1][j], table[i][j - 1]);
        }
    }

    int index = table[S1.size()][S2.size()];
    std::vector<T> lcs(index);

    i = S1.size(), j = S2.size();
    while (i > 0 && j > 0) {
        if (S1[i - 1] == S2[j - 1]) {
          lcs[index - 1] = S1[i - 1];
          i--, j--, index--;
        } else if (table[i - 1][j] > table[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    return lcs;
}

}  // namespace lcs

