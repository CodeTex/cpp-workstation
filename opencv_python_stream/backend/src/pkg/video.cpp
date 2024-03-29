#include <ctime>
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

    TickMeter tm;

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
            tm.start();
            Point brightestPoint = findBrightestPoint(outputFrame, neighbors);
            tm.stop();

            int thickness = 2; // Pixel thickness of the rectangle
            Scalar color(0, 255, 0); // Green rectangle
            drawRectangleAroundPoint(outputFrame, brightestPoint, neighbors, thickness, color);

            // Write timestamp on frame
            time_t now = time(nullptr);
            std::string timestamp = cv::format("Timestamp : %s", ctime(&now));
            putText(outputFrame, timestamp, Point(5, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);

            // Write FPS on frame
            double fpsCount = tm.getFPS();
            std::string fpsString = cv::format("FPS : %.2f", fpsCount);
            putText(outputFrame, fpsString, Point(5, 35), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);

            // Write the brightest point on frame
            std::string pointString = cv::format("Nearest Point : (%d,%d)", brightestPoint.x, brightestPoint.y);
            putText(outputFrame, pointString, Point(5, 55), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
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