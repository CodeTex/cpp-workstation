#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    std::string image_path = "c:\\person.jpg";
    Mat image = imread(image_path, 1);
    
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    std::cout << "Image size: " << image.size() << std::endl;

    return 0;
}