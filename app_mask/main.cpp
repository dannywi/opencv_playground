#include <iostream>
#include <opencv2/opencv.hpp>

#include "lib/util.hpp"

using namespace std;

void sharpen(const cv::Mat& source, cv::Mat& result) {
  CV_Assert(source.depth() == CV_8U);

  result.create(source.size(), source.type());

  cout << "DEBUG ... size [" << source.size() << "] type [" << source.type()
       << "]" << endl;

  const int channels = source.channels();
  const int rows = source.rows;
  const int cols = channels * (source.cols - 1);

  for (size_t r = 1; r < source.rows - 1; ++r) {
    const uchar* prev = source.ptr<uchar>(r - 1);
    const uchar* curr = source.ptr<uchar>(r);
    const uchar* next = source.ptr<uchar>(r + 1);

    uchar* output = result.ptr<uchar>(r);

    for (size_t c = channels; c < cols; ++c) {
      *output++ =
          cv::saturate_cast<uchar>(5.2 * curr[c] - curr[c - channels] -
                                   curr[c + channels] - prev[c] - next[c]);
    }
  }

  auto set_0_fn = [](auto tgt) { tgt.setTo(cv::Scalar(0)); };
  set_0_fn(result.row(0));
  set_0_fn(result.row(result.rows - 1));
  set_0_fn(result.col(0));
  set_0_fn(result.col(result.cols - 1));
}

void show(const cv::Mat& one, const cv::Mat& two) {
  cv::namedWindow("ONE", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("TWO", cv::WINDOW_AUTOSIZE);

  cv::imshow("ONE", one);
  cv::imshow("TWO", two);
  cv::waitKey();
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout << "Please pass image file to process ..." << endl;
  }

  cv::Mat source = cv::imread(argv[1], cv::IMREAD_COLOR);

  if (source.empty()) {
    cerr << "Failed to open [" << argv[1] << "]" << endl;
  } else {
    cout << "Processing image [" << argv[1] << "]" << endl;
  }

  {
    util::timer t("mask - use handcrafted mask");
    cv::Mat result;
    sharpen(source, result);
    t.print_passed_sec();
    show(source, result);
  }

  {
    util::timer t("mask - use kernel matrix");
    // clang-format off
    cv::Mat kernel = (cv::Mat_<char>(3, 3) <<
      0, -1,  0,
      -1,  5, -1,
      0, -1, 0
    );
    // clang-format on
    cv::Mat result;
    cv::filter2D(source, result, source.depth(), kernel);
    t.print_passed_sec();
    show(source, result);
  }

  {
    util::timer t("mask - Sobelx");
    cv::Mat sobelx;
    cv::Sobel(source, sobelx, CV_32F, 1, 0);
    double min, max;
    cv::minMaxLoc(sobelx, &min, &max);  // find minimum and maximum intensities
    cv::Mat result;
    double range = max - min;
    sobelx.convertTo(result, CV_8U, 255.0 / range, -min * 255.0 / range);
    t.print_passed_sec();
    show(source, result);
  }
}
