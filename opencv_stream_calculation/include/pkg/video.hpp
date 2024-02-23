#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

void drawRectangleAroundPoint(cv::Mat& frame, const cv::Point& center, int depth, int thickness, const cv::Scalar& color);
void drawWindow(cv::VideoCapture& cap, bool mirrorFlag, int neighbors);

#endif // VIDEO_H 