#include <iostream>
#include <opencv2/opencv.hpp>

#include "lib/util.hpp"

using namespace std;

namespace util {
uchar* lookup_table(uchar div) {
  static uchar table[256];
  static bool done = false;

  if (!done) {
    for (size_t i = 0; i < 256; ++i) {
      uchar val = div * (static_cast<uchar>(i) / div);
      table[i] = val;
    }
    done = true;
  }
  return table;
}
}  // namespace util

void play(int argc, char* argv[]) {
  cv::Mat A, C;

  if (argc > 1) {
    A = cv::imread(argv[1], cv::IMREAD_COLOR);
  } else {
    cout << "DEBUG ... using sample" << endl;
    A = cv::imread(cv::samples::findFile("messi5.jpg"));
  }

  cv::imshow("IMAGE", A);
  cv::waitKey();

  C = A;
  cv::Mat D(A, cv::Rect(10, 10, 100, 100));
  cv::Mat D2 = A(cv::Range::all(), cv::Range(1, 3));

  cv::Mat M(2, 3, CV_8UC3, cv::Scalar(0, 0, 255));
  cout << "M = " << endl << M << "\n";

  int sz[3] = {2, 2, 2};
  cv::Mat L(3, sz, CV_8UC(1), cv::Scalar::all(0));

  cv::Mat E = cv::Mat::eye(4, 4, CV_64F);
  cout << "E = " << endl << E << endl << endl;
  cv::Mat O = cv::Mat::ones(2, 2, CV_32F);
  cout << "O = " << endl << O << endl << endl;
  cv::Mat Z = cv::Mat::zeros(3, 3, CV_8UC1);
  cout << "Z = " << endl << Z << endl << endl;

  cv::Mat G = (cv::Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
  cout << "G = " << endl << G << endl << endl;

  // somehow doesn't work?
  cout << "G (reshaped) = " << endl
       << (cv::Mat_<double>({0, -1, 0, -1, 5, -1, 0, -1, 0})).reshape(3) << endl
       << endl;

  cout << "G (row 1) = " << endl << (G.row(1).clone()) << endl;
}

void scan_and_reduce(cv::Mat& I, const uchar* const lookup_table) {
  CV_Assert(I.depth() == CV_8U);

  int channels = I.channels();
  int nRows = I.rows;
  int nCols = I.cols * channels;

  cout << "DEBUG ... nRows [" << nRows << "] nCols [" << nCols << "] channels ["
       << channels << "]" << endl;

  if (I.isContinuous()) {
    nCols *= nRows;
    nRows = 1;
    cout << "DEBUG ... image is continuous - flat [" << nCols << "] array"
         << endl;
  }

  int db_cnt = 50;

  for (size_t r = 0; r < nRows; ++r) {
    uchar* p = I.ptr<uchar>(r);
    for (size_t c = 0; c < nCols; ++c) {
      p[c] = lookup_table[p[c]];
    }
  }
}

int main(int argc, char* argv[]) {
  util::timer t("hello world");

  cout << "Hello OpenCV!\n";
  play(argc, argv);

  if (argc == 3) {
    util::timer t2("Reduce RBG");
    cout << "======== processing image [" << argv[1] << "] divisor [" << argv[2]
         << "]" << endl;

    cv::Mat A = cv::imread(argv[1], cv::IMREAD_COLOR);
    // cv::Mat A = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    uchar div = static_cast<uchar>(strtoul(argv[2], NULL, 0));
    scan_and_reduce(A, util::lookup_table(div));

    t2.print_passed_sec();

    // stringstream save_name;
    // save_name << "result" << argv[2] << ".jpg";
    // cv::imwrite(save_name.str(), A);

    cv::namedWindow("my_output", cv::WINDOW_AUTOSIZE);
    cv::imshow("my_output", A);
    cv::waitKey();
  }
}
