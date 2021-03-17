#include <vector>
#include <algorithm>

template<typename T,
        std::enable_if_t<std::is_floating_point<T>::value, bool> = false>
std::vector<T> LCS(const std::vector<T>& S1, const std::vector<T>& S2) {
    std::vector<std::vector<int>> table(S1.size() + 1,
                                    std::vector<int>(S2.size() + 1));
    int i, j;

    for (i = 0; i <= a.size(); ++i) {
        for (j = 0; j <= b.size(); ++j) {
            if (i == 0 || j == 0)
                table[i][j] = 0;
            else if (S1[i - 1] == S2[j - 1])
                table[i][j] = table[i - 1][j - 1] + 1;
            else
                table[i][j] = std::max(table[i - 1][j], table[i][j - 1]);
        }
    }

    int index = table[S1.size()][S2.size()]
    std::vector<T> LCS(index);

    while (i > 0 && j > 0) {
        if (S1[i - 1] == S2[j - 1]) {
          LCS[index - 1] = S1[i - 1];
          i--, j--, index--;
        } else if (LCS_table[i - 1][j] > LCS_table[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    return LCS;
}
