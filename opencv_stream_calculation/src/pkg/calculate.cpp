#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

Point findBrightestPoint(const Mat& frame, int neighbors) {
    CV_Assert(frame.channels() == 1 || frame.channels() == 3); // Ensure frame is either grayscale or BGR

    Mat grayFrame;
    if (frame.channels() == 3) {
        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
    } else {
        grayFrame = frame;
    }

    Mat blurredFrame;
    int kernelSize = 1 + 2 * neighbors; // Kernel size must be odd
    blur(grayFrame, blurredFrame, Size(kernelSize, kernelSize));

    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(blurredFrame, &minVal, &maxVal, &minLoc, &maxLoc);

    return maxLoc;
}
