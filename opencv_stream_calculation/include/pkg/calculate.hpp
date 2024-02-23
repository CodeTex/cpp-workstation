#ifndef CALCULATE_H
#define CALCULATE_H

#include <opencv2/core.hpp>

cv::Point findBrightestPoint(const cv::Mat& frame, int neighbors);

#endif // CALCULATE_H
