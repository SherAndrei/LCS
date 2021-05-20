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
        map << s1_points[idx_s1] << '\n' << s2_points[idx_s2] << "\n\n";
    }
}

void doTest(
    const std::string& first_filename,
    const std::string& second_filename,
    double metric_lower_bound = 0.,
    double metric_upper_bound = 0.,
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
            map << mp.res_metric() << '\n';
            map << table << '\n';
            map << mapping << '\n';
        }

        PlotMapping(s1_points, s2_points, mapping);
    }
    if (metric_upper_bound > 0) {
        ASSERT(metric_lower_bound < mp.res_metric());
        ASSERT(mp.res_metric() < metric_upper_bound);
    }
    if (!assert_mapping.empty())
        ASSERT_EQUAL(assert_mapping, mapping);
}

void testSmall() {
    doTest(
        "tests/00.one_to_one/seq1.txt",
        "tests/00.one_to_one/seq2.txt",
        0.9, 1.1,
        {{0, 0}});
    doTest(
        "tests/01.all_to_one/seq1.txt",
        "tests/01.all_to_one/seq2.txt",
        9.9, 10.1,
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0},
         {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}});
    doTest(
        "tests/02.one_to_all/seq1.txt",
        "tests/02.one_to_all/seq2.txt",
        9.9, 10.1,
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},
         {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}});
}

void testConstants() {
    for (const std::string& test_name : {
            "03.constants", "05.increasing", "06.decreasing"
            }) {
        doTest(
            "tests/" + test_name + "/seq1.txt",
            "tests/" + test_name + "/seq2.txt",
            -0.1, 0.1,
            {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}});
    }
    doTest(
        "tests/04.diff_constants/seq1.txt",
        "tests/04.diff_constants/seq2.txt",
        0.9, 1.1,
        {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}});
}

void testVarious() {
    doTest(
        "tests/07.simple/seq1.txt",
        "tests/07.simple/seq2.txt",
        0., 0.2,
        {{0, 0}, {1, 0}, {2, 1}, {2, 2}, {2, 3}, {3, 4}, {4, 4}});
    doTest(
        "tests/08.unequal/seq1.txt",
        "tests/08.unequal/seq2.txt",
        0, 10,
        {{0, 0}, {1, 0}, {2, 1}, {3, 2}, {4, 2}});
}

void testFuncs() {
    doTest("tests/funcs/sin/seq1.txt", "tests/funcs/sin/seq2.txt",
            0., 4e-5, {});
    doTest("tests/funcs/cont/seq1.txt", "tests/funcs/cont/seq2.txt",
            0., 0, {}, true);
}

}  // namespace

int main() {
    TestRunner tr;
    RUN_TEST(tr, testSmall);
    RUN_TEST(tr, testConstants);
    RUN_TEST(tr, testVarious);
    RUN_TEST(tr, testFuncs);
}
