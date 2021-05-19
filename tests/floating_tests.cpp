#include <vector>

#include "table.hpp"
#include "LCS.hpp"

#include "test_runner.h"
#include "profile.h"

namespace {

using Sequence = lcs::FloatingTable::Sequence;

void testSimple() {
    {
        Sequence S1 = {0.9, 1.1, 2.5, 3.9, 4.1};
        Sequence S2 = {1, 2.4, 2.5, 2.6, 4};
        lcs::Mapper mp(S1, S2);
        std::cout << mp.table() << std::endl;
        std::cout << "Metric: " << mp.mapping() << std::endl;
        std::cout << "Metric: " << mp.res_metric() << std::endl;
        ASSERT(mp.res_metric() < 0.01);
    }
}

}  // namespace

int main() {
    TestRunner tr;
    RUN_TEST(tr, testSimple);
}
