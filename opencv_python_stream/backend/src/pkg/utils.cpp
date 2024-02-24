#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

void listDevices() {
    VideoCapture capture;
    int deviceCount = 0;
    const int maxDeviceCount = 10;

    while (deviceCount < maxDeviceCount) {
        capture.open(deviceCount);
        if (!capture.isOpened()) {
            break;
        }
        cout << "Device " << deviceCount << " is working." << endl;
        deviceCount++;
        capture.release();
    }
}

bool checkBooleanFlag(int argc, char** argv, const std::string& flag) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == flag) {
            return true;
        }
    }
    return false;
}

int checkIntFlag(int argc, char** argv, const std::string& flag, int defaultValue) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == flag) {
            if (i + 1 < argc) {
                return std::stoi(argv[i + 1]);
            }
        }
    }
    return defaultValue;
}
