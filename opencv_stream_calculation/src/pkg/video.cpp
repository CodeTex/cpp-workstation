#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "calculate.hpp"

using namespace cv;
using namespace std;

void drawRectangleAroundPoint(Mat& frame, const Point& center, int depth, int thickness, const Scalar& color)
{
    int radius = depth * 2 + 1; // Convert depth to radius
    
    Point topLeft(center.x - radius, center.y - radius);
    Point bottomRight(center.x + radius, center.y + radius);

    rectangle(frame, topLeft, bottomRight, color, thickness);
}

void drawWindow(VideoCapture& cap, bool mirrorFlag, int neighbors)
{
    namedWindow("Webcam", WINDOW_AUTOSIZE);

    while (true) {
        Mat frame, outputFrame;

        cap >> frame;

        if (frame.empty()) {
            cerr << "ERROR: Couldn't grab a camera frame." << endl;
            break;
        }

        if (mirrorFlag) {
            flip(frame, outputFrame, 1); // Flip the frame horizontally
        } else {
            outputFrame = frame;
        }

        if (neighbors > 0) {
            Point brightestPoint = findBrightestPoint(outputFrame, neighbors);

            int thickness = 2; // Pixel thickness of the rectangle
            Scalar color(0, 255, 0); // Green rectangle
            drawRectangleAroundPoint(outputFrame, brightestPoint, neighbors, thickness, color);
        }

        imshow("Webcam", outputFrame);

        if (waitKey(30) == 27) {
            cout << "ESC key pressed by user. Exiting..." << endl;
            break;
        }
    }

    cap.release();
    destroyAllWindows();
}