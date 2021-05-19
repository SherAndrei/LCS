#ifndef PROFILE_H
#define PROFILE_H

#include <chrono>
#include <iostream>
#include <string>

namespace chr = std::chrono;

class LogDuration {
 public:
    explicit LogDuration(const std::string& msg = "")
        : message(msg + ": ")
        , start(chr::steady_clock::now()) {
    }

    ~LogDuration() {
        auto finish = chr::steady_clock::now();
        auto dur = finish - start;
        std::cerr << message
            << chr::duration_cast<chr::microseconds>(dur).count()
            << " us ("
            << chr::duration_cast<chr::milliseconds>(dur).count() << " ms)\n";
    }
 private:
    std::string message;
    chr::steady_clock::time_point start;
};


#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) \
  LogDuration UNIQ_ID(__LINE__){message};

#endif  // PROFILE_H
