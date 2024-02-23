#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "pkg/vis.hpp"

using namespace cv;

int main(int argc, char** argv) {
        CommandLineParser parser(argc, argv,
        "{help  h           |         | Print this message}"
        "{fd_model fd       |         | Path to the face detection model. Download yunet.onnx in https://github.com/opencv/opencv_zoo/tree/master/models/face_detection_yunet}"
        "{video v           | 0       | Path to the input video}"
        "{scale sc          | 1.0     | Scale factor used to resize input video frames}"
        "{score_threshold   | 0.9     | Filter out faces of score < score_threshold}"
        "{nms_threshold     | 0.3     | Suppress bounding boxes of iou >= nms_threshold}"
        "{top_k             | 5000    | Keep top_k bounding boxes before NMS}"
        "{save s            | false   | Set true to save results. This flag is invalid when using camera}"
    );
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    std::cout << "FD Model flag provided: " << parser.has("fd_model") << std::endl;
    String fd_modelPath = parser.get<String>("fd_model");

    float scoreThreshold = parser.get<float>("score_threshold");
    float nmsThreshold = parser.get<float>("nms_threshold");
    int topK = parser.get<int>("top_k");
    bool save = parser.get<bool>("save");
    float scale = parser.get<float>("scale");

    int frameWidth, frameHeight;
    VideoCapture capture;
    std::string video = parser.get<std::string>("video");

    if (video.size() == 1 && isdigit(video[0]))
    {
        capture.open(parser.get<int>("video"));
    }
    else
    {
        capture.open(samples::findFileOrKeep(video));  // keep GStreamer pipelines
    }

    if (capture.isOpened())
    {
        frameWidth = int(capture.get(CAP_PROP_FRAME_WIDTH) * scale);
        frameHeight = int(capture.get(CAP_PROP_FRAME_HEIGHT) * scale);
        std::cout << "Video " << video
            << ": width=" << frameWidth
            << ", height=" << frameHeight
            << std::endl;
    }
    else
    {
        std::cout << "Could not initialize video capturing: " << video << "\n";
        return 1;
    }

    // Initialize FaceDetectorYN
    std::cout << fd_modelPath << std::endl;
    Ptr<FaceDetectorYN> detector = FaceDetectorYN::create(fd_modelPath, "", Size(320, 320), scoreThreshold, nmsThreshold, topK);
    TickMeter tm;
    detector->setInputSize(Size(frameWidth, frameHeight));

    std::cout << "Press 'SPACE' to save frame, any other key to exit..." << std::endl;
    int nFrame = 0;
    for (;;)
    {
        // Get frame
        Mat frame;
        if (!capture.read(frame))
        {
            std::cerr << "Can't grab frame! Stop\n";
            break;
        }
        resize(frame, frame, Size(frameWidth, frameHeight));
        // Inference
        Mat faces;
        tm.start();
        detector->detect(frame, faces);
        tm.stop();
        Mat result = frame.clone();
        // Draw results on the input image
        visualize(result, nFrame, faces, tm.getFPS());
        // Visualize results
        imshow("Live", result);
        int key = waitKey(1);
        bool saveFrame = save;
        if (key == ' ')
        {
            saveFrame = true;
            key = 0;  // handled
        }
        if (saveFrame)
        {
            std::string frame_name = cv::format("frame_%05d.png", nFrame);
            std::string result_name = cv::format("result_%05d.jpg", nFrame);
            std::cout << "Saving '" << frame_name << "' and '" << result_name << "' ...\n";
            imwrite(frame_name, frame);
            imwrite(result_name, result);
        }
        ++nFrame;
        if (key > 0)
            break;
    }
    std::cout << "Processed " << nFrame << " frames" << std::endl;
    
    std::cout << "Done." << std::endl;
    return 0;
}