#include <array>
#include <iostream>
#include <limits>
#include <sstream>

#include "lib/util.hpp"
#include "opencv2/opencv.hpp"

using namespace std;

namespace lookup_table {

// TODO: T must be integer smaller than size_t. Do static assertion.

template <typename T, size_t N = numeric_limits<T>::max() + 1>
array<T, N> alpha_beta(double alpha, double beta) {
  array<T, N> ret;
  for (size_t i = 0; i < N; ++i)
    ret[i] = cv::saturate_cast<T>(alpha * i + beta);
  return ret;
}

template <typename T, size_t N = numeric_limits<T>::max() + 1>
array<T, N> gamma(double gamma) {
  array<T, N> ret;
  double max_val = static_cast<double>(N - 1);
  for (size_t i = 0; i < N; ++i)
    ret[i] = cv::saturate_cast<T>(pow(i / max_val, gamma) * max_val);
  return ret;
}

}  // namespace lookup_table

template <typename T, size_t N = numeric_limits<T>::max() + 1>
cv::Mat transform_image(const cv::Mat& image, const array<T, N>& lookup_table) {
  cv::Mat result = cv::Mat::zeros(image.size(), image.type());

  for (size_t y = 0; y < image.rows; ++y) {
    for (size_t x = 0; x < image.cols; ++x) {
      for (size_t c = 0; c < image.channels(); ++c) {
        result.at<cv::Vec3b>(y, x)[c] =
            lookup_table[image.at<cv::Vec3b>(y, x)[c]];
      }
    }
  }

  return result;
}

void show(const cv::Mat& result, const string& title) {
  cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
  cv::imshow(title, result);
  cv::waitKey();
}

int main(int argc, char* argv[]) {
  cv::CommandLineParser parser(
      argc, argv,
      "{help h        |     | print this message    }"
      "{@image        |     | image to process      }"
      "{alpha a       |  1.0| 0.0 to 3.0            }"
      "{beta b        |  0.0| -100.0 to 100.0       }"
      "{gamma g       |  1.0| -100.0 to 100.0       }");

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  cv::Mat image = cv::imread(parser.get<cv::String>(0), cv::IMREAD_COLOR);

  {
    double alpha = parser.get<double>("alpha");
    double beta = parser.get<double>("beta");

    util::timer t("brightness and constrast - alpha + beta");
    cv::Mat result = transform_image<uchar>(
        image, lookup_table::alpha_beta<uchar>(alpha, beta));
    t.print_passed_sec();
    stringstream title;
    title << "alpha [" << alpha << "] beta [" << beta << "]" << endl;
    show(result, title.str());
  }

  {
    double gamma = parser.get<double>("gamma");

    util::timer t("brightness and constrast - gamma");
    cv::Mat result =
        transform_image<uchar>(image, lookup_table::gamma<uchar>(gamma));
    t.print_passed_sec();
    stringstream title;
    title << "gamma [" << gamma << "]" << endl;
    show(result, title.str());
  }
}
