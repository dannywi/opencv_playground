#include <iostream>
#include <opencv2/opencv.hpp>

#include "lib/util.hpp"

using namespace std;

cv::Mat blend(const cv::Mat& image1, const cv::Mat& image2, double alpha) {
  CV_Assert(image1.depth() == CV_8U);
  CV_Assert(image1.depth() == image2.depth());
  CV_Assert(image1.rows == image2.rows);
  CV_Assert(image1.cols == image2.cols);
  CV_Assert(image1.channels() == image2.channels());
  CV_Assert(image1.type() == image2.type());
  CV_Assert(image1.size() == image2.size());

  cv::Mat result;
  result.create(image1.size(), image1.type());

  const int channels = image1.channels();
  const int rows = image1.rows;
  const int cols = channels * (image1.cols - 1);

  for (size_t r = 0; r < rows; ++r) {
    const uchar* row1 = image1.ptr<uchar>(r);
    const uchar* row2 = image2.ptr<uchar>(r);
    uchar* row_res = result.ptr<uchar>(r);

    for (size_t c = 0; c < cols; ++c) {
      row_res[c] =
          cv::saturate_cast<uchar>((row1[c] * alpha) + row2[c] * (1 - alpha));
    }
  }

  return result;
}

void show(const cv::Mat& result) {
  cv::namedWindow("RESULT", cv::WINDOW_AUTOSIZE);

  cv::imshow("RESULT", result);
  cv::waitKey();
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
    cout << "Pass <image 1> <image 2> <0 to 1> ..." << endl;
  }

  cv::Mat image1 = cv::imread(argv[1], cv::IMREAD_COLOR);
  cv::Mat image2 = cv::imread(argv[2], cv::IMREAD_COLOR);
  double alpha = stof(argv[3]);

  if (image1.empty()) {
    cerr << "Failed to open [" << argv[1] << "]" << endl;
  } else if (image2.empty()) {
    cerr << "Failed to open [" << argv[2] << "]" << endl;
  } else if (alpha < 0 || alpha > 1) {
    cerr << "alpha [" << argv[3] << "] not between 0 and 1" << endl;
  }

  {
    util::timer t("blend - manual blending");
    cv::Mat result = blend(image1, image2, alpha);
    t.print_passed_sec();
    show(result);
  }

  {
    util::timer t("blend - using addWeighted");
    cv::Mat result;
    cv::addWeighted(image1, alpha, image2, 1 - alpha, 0.0, result);
    t.print_passed_sec();
    show(result);
  }
}
