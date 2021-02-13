#pragma once

#include <chrono>
#include <iostream>
#include <string>


class LogDuration {
public:
  explicit LogDuration(const std::string& msg = "")
    : message(msg + ": "), start(std::chrono::steady_clock::now()) {}

  ~LogDuration() {
    auto finish = std::chrono::steady_clock::now();
    std::cerr << message
      << std::chrono::duration_cast<std::chrono::milliseconds>
                                    (finish - start).count()
      << " ms." << std::endl;
  }
private:
  std::string message;
  std::chrono::steady_clock::time_point start;
};

#define UNIQ_ID_MAKER__(line) __loc_var_there__##line
#define UNIQ_ID__(line) UNIQ_ID_MAKER__(line)

#define LOG_DURATION(message) LogDuration UNIQ_ID__(__LINE__)(message);
