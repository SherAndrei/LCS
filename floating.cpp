#include <iostream>
#include <memory>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>

#include "test_runner.h"
#include "profile.h"

using sequence_t = double;
using Sequence = std::vector<sequence_t>;

class FloatingTable final {
 public:
    FloatingTable() = default;
    FloatingTable(const Sequence& S1,
                  const Sequence& S2)
        : _rows(S1.size())
        , _columns(S2.size())
        , _table(std::make_unique<double[]>(_rows * _columns)) {
            fill(S1, S2);
    }

    static double metric(double x_i, double y_j) {
        return std::pow(x_i - y_j, 2);
    }

    void fill(const Sequence& S1, const Sequence& S2) {
        _rows = S1.size();
        _columns = S2.size();
        _table = std::make_unique<double[]>(_rows * _columns);

        size_t i, j;
        for (i = 0u; i < _rows; ++i) {
            for (j = 0u; j < _columns; ++j) {
                at(i, j) = metric(S1[i], S2[j]);
            }
        }
    }

    double& at(size_t i, size_t j) {
        return _table.get()[i * _columns + j];
    }

    double at(size_t i, size_t j) const {
        return _table.get()[i * _columns + j];
    }

 protected:
    virtual std::ostream& print(std::ostream& os) const {
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _columns; ++j)
                os << std::setw(4) << at(i, j) << " \n"[j == _columns - 1];
        }
        return os;
    }

 public:
    friend std::ostream& operator<<(std::ostream& os, const FloatingTable& t) {
        return t.print(os);
    }

 private:
    size_t _rows;
    size_t _columns;
    std::unique_ptr<double[]> _table;
};


class Mapper {
 private:
    const Sequence& _S1;
    const Sequence& _S2;
    const FloatingTable _table;
    double _res_metric;
 public:
    Mapper(const Sequence& S1, const Sequence& S2)
        : _S1(S1), _S2(S2), _table(S1, S2) {}

    std::vector<std::pair<double, double>>
    mapping() {
        
    }
};

double
estimate(const Sequence& S1, const Sequence& S2) {
    const FloatingTable table(S1, S2);
    double res_metric = 0.;
    size_t counter = 0;

    // (i + 1, j) (i + 1, j + 1), (i, j + 1)
    std::array<double, 3> next_elems;
    double cur = 0.;
    for (size_t i = 0, j = 0;;) {
        cur = table.at(i, j);
        res_metric += table.at(i, j);
        counter++;
        if (i + 1 < S1.size() && j + 1 < S2.size()) {
            next_elems = {table.at(i+1, j), table.at(i+1, j+1), table.at(i, j+1)};
            auto pos = std::min_element(next_elems.begin(), next_elems.end());

            if (pos == next_elems.begin())
                i++;
            else if (pos == next_elems.begin() + 2)
                j++;
            else
                i++, j++;
        } else if (i + 1 == S1.size() && j + 1 < S2.size()) {
            j++;
        } else if (j + 1 == S2.size() && i + 1 < S1.size()) {
            i++;
        } else if (i + 1 == S1.size() && j + 1 == S2.size()) {
            break;
        }
    }

    return res_metric / counter;
}

void Simple() {
    {
        FloatingTable f;
        Sequence S1 = {0.9, 1.1, 2.5, 3.9, 4.1};
        Sequence S2 = {1, 2.4, 2.5, 2.6, 4};
        f.fill(S1, S2);
        std::cout << f << std::endl;
        std::cout << "Metric: " << estimate(S1, S2) << std::endl;
    }
    {
        FloatingTable f;
        Sequence S1 = {3.5, 9, 2.8};
        Sequence S2 = {1, 3.4, 8, 3.6, 1};
        f.fill(S1, S2);
        std::cout << f << std::endl;
        std::cout << estimate(S1, S2) << std::endl;
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Simple);
}
