#ifndef VIS_H
#define VIS_H

#include <opencv2/core.hpp>

void visualize(cv::Mat& input, int frame, cv::Mat& faces, double fps, int thickness = 2);

#endif // VIS_H