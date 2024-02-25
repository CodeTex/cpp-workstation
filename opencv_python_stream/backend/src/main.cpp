#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "pkg/utils.hpp"
#include "pkg/video.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " --list-devices or <DeviceIndex> [-m] [-n <neighbours>]" << endl;
        return 1;
    }

    string firstArg(argv[1]);

    if (firstArg == "--list-devices") {
        listDevices();
        return 0; // Exit after listing devices
    }

    int deviceIndex = stoi(firstArg);
    bool mirrorFlag = checkBooleanFlag(argc, argv, "-m");
    int neighbors = checkIntFlag(argc, argv, "-n", 1);

    VideoCapture cap(deviceIndex);
    if (!cap.isOpened()) {
        cerr << "ERROR: Could not open camera" << endl;
        return 1;
    }

    drawWindow(cap, mirrorFlag, neighbors);

    return 0;
}