#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"

namespace util {

class timer {
 public:
  timer(const char* title)
      : tick_start_(cv::getTickCount()),
        tick_freq_(cv::getTickFrequency()),
        title_(title) {
    std::cout << "[timer] [" << title_ << "] START" << std::endl;
  }
  ~timer() {
    if (!printed_) print_passed_sec();
  }

  double get_passed_sec() const {
    return (cv::getTickCount() - tick_start_) / tick_freq_;
  }

  void print_passed_sec() {
    std::cout << "[timer] [" << title_
              << "] END -- elapsed secs: " << get_passed_sec() << std::endl;
    printed_ = true;
  }

 private:
  const double tick_start_;
  const double tick_freq_;
  const char* title_;
  bool printed_ = false;
};

}  // namespace util