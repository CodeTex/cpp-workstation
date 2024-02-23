#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

void listDevices() {
    VideoCapture capture;
    int deviceCount = 0;
    const int maxDeviceCount = 10;

    while (deviceCount < maxDeviceCount) {
        capture.open(deviceCount);
        if (!capture.isOpened()) {
            break;
        }
        std::cout << "Device " << deviceCount << " is working." << std::endl;
        deviceCount++;
        capture.release();
    }
}

bool checkFlag(int argc, char** argv, const std::string& flag) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == flag) {
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " --list-devices or <DeviceIndex> [-m]" << std::endl;
        return 1;
    }

    std::string firstArg(argv[1]);

    if (firstArg == "--list-devices") {
        listDevices();
        return 0; // Exit after listing devices
    }

    int deviceIndex = std::stoi(firstArg);
    bool mirrorFlag = checkFlag(argc, argv, "-m");

    VideoCapture cap(deviceIndex);
    if (!cap.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    namedWindow("Webcam", WINDOW_AUTOSIZE);

    while (true) {
        Mat frame, outputFrame;
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "ERROR: Couldn't grab a camera frame." << std::endl;
            break;
        }

        if (mirrorFlag) {
            flip(frame, outputFrame, 1); // Flip the frame horizontally
        } else {
            outputFrame = frame;
        }

        imshow("Webcam", outputFrame);

        if (waitKey(30) == 27) {
            std::cout << "ESC key pressed by user. Exiting..." << std::endl;
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}