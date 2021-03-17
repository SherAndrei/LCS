#include <iostream>
#include <fstream>
#include <cmath>

double func(double x) {
#ifdef FIRST
    if (x < -3)
        return -x;
    if (x > 3)
        return x;
    return 3;
#elif SECOND
    return 3.1;
#endif
}

int main(int argc, char* argv[]) {
    if (argc != 5 || argv[3] <= 0) {
        std::cerr << "Usage: ./a.out a b n filename\n"
                  << "\ta - left bound\n"
                  << "\tb - right bound\n"
                  << "\tn - number of points\n"
                  << "\tfilename - output filename\n";
        return 1;
    }
    double left = std::stod(argv[1]);
    double right = std::stod(argv[2]);
    size_t n = std::stoul(argv[3]);
    double cur = left;
    std::ofstream out(argv[4]);
    for (size_t i = 0; i < n; i++) {
        cur = left + (1. * i * (right - left) / n);
        out << cur << ' ' << func(cur) << '\n';
    }
}
