#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

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

inline void Plot(
    std::vector<Point> s1_points,
    const std::vector<Point>& s2_points,
    const lcs::FloatingTable& table,
    const lcs::Mapper& mapper
) {
    const std::filesystem::path to_plot = "./to_plot";
    std::filesystem::create_directory(to_plot);

    {
        std::ofstream tfile(to_plot/"table.txt");
        tfile << table << '\n';
    }
    {
        sequence_t diff = 2;
        std::ofstream o_seq1(to_plot/"seq1.txt");
        std::ofstream s_seq1(to_plot/"shifted_seq1.txt");
        for (auto&& p : s1_points) {
            o_seq1 << p << '\n';
            p.y += diff;
            s_seq1 << p << '\n';
        }
    }
    {
        std::ofstream o_seq2(to_plot/"seq2.txt");
        for (auto&& p : s2_points) {
            o_seq2 << p << '\n';
        }
    }
    {
        std::ofstream mpf(to_plot/"map_function.txt");
        auto values = mapper.map_function();
        for (auto&& val : values)
            mpf << val << '\n';
    }
    auto mapping = mapper.mapping();
    {
        std::ofstream metric(to_plot/"metric.txt");
        metric << mapper.res_metric() << '\n'
               << mapping << '\n';
    }
    {
        std::ofstream map(to_plot/"mapping.txt");
        for (auto&& [idx_s1, idx_s2] : mapping) {
            map << s2_points[idx_s1] << '\n' << s1_points[idx_s2] << "\n\n";
        }
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
        Plot(s1_points, s2_points, table, mp);
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
        0.9, 1.1, {{0, 0}});
    doTest(
        "tests/01.all_to_one/seq1.txt",
        "tests/01.all_to_one/seq2.txt",
        -0.1, 0.1, {{0, 5}});
    try {
        doTest(
            "tests/02.one_to_all/seq2.txt",
            "tests/02.one_to_all/seq1.txt",
            0, 0, {});
        ASSERT(false);
    } catch (const std::exception&) {
        ASSERT(true);
    }
}

void testConstants() {
    for (const std::string& test_name : {
            "02.constants", "04.increasing", "05.decreasing"
    }) {
        doTest(
            "tests/" + test_name + "/seq1.txt",
            "tests/" + test_name + "/seq2.txt",
            -0.1, 0.1,
            {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}});
    }
    doTest(
        "tests/03.diff_constants/seq1.txt",
        "tests/03.diff_constants/seq2.txt",
        0.9, 1.1,
        {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}});
}

void testVarious() {
    doTest(
        "tests/06.simple/seq1.txt",
        "tests/06.simple/seq2.txt",
        0.008, 0.009,
        {{0, 0}, {0, 1}, {1, 2}, {2, 2}, {3, 2}, {4, 3}});
    doTest(
        "tests/07.unequal/seq1.txt",
        "tests/07.unequal/seq2.txt",
        0.19, 0.2,
        {{0, 1}, {1, 2}, {2, 3}}, true);
}

void testMisc() {
    doTest("tests/08.misc/seq1.txt", "tests/08.misc/seq2.txt",
            -0.001, 0.001, {{0, 1}, {1, 2}});
    doTest("tests/08.misc/seq1.txt", "tests/08.misc/seq3.txt",
            -0.001, 0.001, {{0, 1}, {0, 2}, {1, 3}});
    doTest("tests/08.misc/seq1.txt", "tests/08.misc/seq4.txt",
            -0.001, 0.001, {{0, 0}, {1, 1}});
}

void testFuncs() {
    doTest("tests/funcs/sin/seq1.txt", "tests/funcs/sin/seq2.txt",
            0., 3e-05, {});
    doTest("tests/funcs/cont/seq1.txt", "tests/funcs/cont/seq2.txt",
            0., 0.2, {});
}

}  // namespace

int main() {
    TestRunner tr;
    RUN_TEST(tr, testSmall);
    RUN_TEST(tr, testConstants);
    RUN_TEST(tr, testVarious);
    RUN_TEST(tr, testMisc);
    RUN_TEST(tr, testFuncs);
}
