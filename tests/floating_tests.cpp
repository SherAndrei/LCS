#include <iostream>
#include <fstream>
#include <vector>

#include "table.hpp"
#include "LCS.hpp"

#include "test_runner.h"
#include "profile.h"

using mapping_t = lcs::Mapper::mapping_t;
using sequence_t = lcs::FloatingTable::sequence_t;
using Sequence = lcs::FloatingTable::Sequence;

struct Point {
    sequence_t x, y;
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << p.x << ' ' << p.y;
    }
};

namespace {

inline std::vector<Point> readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Cannot open file " + filename);
    std::vector<Point> points;
    sequence_t x, y;
    while (file >> x >> y) {
        points.push_back({x, y});
    }
    return points;
}

inline Sequence getSequence(const std::vector<Point>& points) {
    Sequence s;
    s.reserve(points.size());
    for (auto&& p : points) {
        s.emplace_back(p.y);
    }
    return s;
}

inline void PlotMapping(
    std::vector<Point> s1_points,
    std::vector<Point> s2_points,
    const mapping_t& mapping
) {
    // auto comp = [](const Point& lhs, const Point& rhs) { return lhs.y < rhs.y; };
    // auto [min_pos_s1, max_pos_s1] = std::minmax_element(s1_points.begin(), s1_points.end(), comp);
    // auto [min_pos_s2, max_pos_s2] = std::minmax_element(s2_points.begin(), s2_points.end(), comp);
    //  (min_pos_s1->y - max_pos_s2->y) / 2;
    // diff *= 2;
    sequence_t diff = 5;

    std::ofstream seq1("seq1.txt");
    for (auto&& p : s1_points) {
        p.y += diff;
        seq1 << p << '\n';
    }

    std::ofstream seq2("seq2.txt");
    for (auto&& p : s2_points) {
        seq2 << p << '\n';
    }

    std::ofstream map("mapping.txt");
    for (auto&& [idx_s1, idx_s2] : mapping) {
        map << s1_points[idx_s1] << '\n' << s2_points[idx_s2] << "\n      \n";
    }
}

void doTest(
    const std::string& first_filename,
    const std::string& second_filename,
    double metric_upper_boudary = 0.,
    const mapping_t& assert_mapping = {},
    bool do_plot = false
) {
    std::vector<Point> s1_points = readFile(first_filename);
    std::vector<Point> s2_points = readFile(second_filename);

    auto s1 = getSequence(s1_points);
    auto s2 = getSequence(s2_points);

    const lcs::FloatingTable table(s1, s2);

    lcs::Mapper mp(table);
    auto mapping = mp.mapping();

    if (do_plot) {
        {
            std::ofstream mpf("map_function.txt");
            auto values = mp.map_function();
            for (auto&& val : values)
                mpf << val << '\n';
        }
        {
            std::ofstream map("map.txt");
            map << table << '\n';
            map << mapping << '\n';
        }

        PlotMapping(s1_points, s2_points, mapping);
    }
    if (metric_upper_boudary > 0)
        ASSERT(mp.res_metric() < metric_upper_boudary);
    if (!assert_mapping.empty())
        ASSERT(assert_mapping == mapping);
}

void testTextFiles() {
    doTest("tests/simple/seq1.txt",
           "tests/simple/seq2.txt",
           0.02,
           {{0, 0}, {1, 0}, {2, 1}, {2, 2}, {2, 3}, {3, 4}, {4, 4}},
           false);
    doTest("tests/unequal/seq1.txt",
           "tests/unequal/seq2.txt",
           10,
           {},
           false);
    // doTest("tests/funcs/seq1.txt", "tests/funcs/seq2.txt",
    //         0., {}, true);
}

}  // namespace

int main() {
    TestRunner tr;
    RUN_TEST(tr, testTextFiles);
}
