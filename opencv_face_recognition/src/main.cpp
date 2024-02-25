#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "pkg/vis.hpp"

using namespace cv;

CommandLineParser parseArguments(int argc, char** argv) {
    CommandLineParser parser(argc, argv,
        "{help  h         |       | Print this message}"
        "{fd_model fd     |       | Path to the face detection model. Download yunet.onnx in https://github.com/opencv/opencv_zoo/tree/master/models/face_detection_yunet}"
        "{video v         | 0     | Path to the input video}"
        "{scale sc        | 1.0   | Scale factor used to resize input video frames}"
        "{score_threshold | 0.9   | Filter out faces of score < score_threshold}"
        "{nms_threshold   | 0.3   | Suppress bounding boxes of iou >= nms_threshold}"
        "{top_k           | 5000  | Keep top_k bounding boxes before NMS}"
        "{save s          | false | Set true to save results. This flag is invalid when using camera}"
    );
    if (parser.has("help")) {
        parser.printMessage();
        exit(0);
    }
    return parser;
}

std::tuple<VideoCapture, int, int> initializeVideoCapture(const CommandLineParser& parser) {
    VideoCapture capture;
    std::string video = parser.get<std::string>("video");
    float scale = parser.get<float>("scale");
    int frameWidth, frameHeight;

    if (video.size() == 1 && isdigit(video[0])) {
        capture.open(parser.get<int>("video"));
    } else {
        capture.open(samples::findFileOrKeep(video));
    }

    if (!capture.isOpened()) {
        std::cerr << "Could not initialize video capturing: " << video << "\n";
        exit(1);
    }

    frameWidth = int(capture.get(CAP_PROP_FRAME_WIDTH) * scale);
    frameHeight = int(capture.get(CAP_PROP_FRAME_HEIGHT) * scale);
    return {capture, frameWidth, frameHeight};
}

Ptr<FaceDetectorYN> initializeFaceDetector(const String& modelPath, float scoreThreshold, float nmsThreshold, int topK, int width, int height) {
    auto detector = FaceDetectorYN::create(modelPath, "", Size(320, 320), scoreThreshold, nmsThreshold, topK);
    detector->setInputSize(Size(width, height));
    return detector;
}

void processVideoFrames(VideoCapture& capture, Ptr<FaceDetectorYN>& detector, bool save, float scale) {
    TickMeter tm;
    Mat frame;
    int nFrame = 0;

    const int borderWidth = 2;

    for (;;) {
        if (!capture.read(frame)) {
            std::cerr << "Can't grab frame! Stop\n";
            break;
        }
        resize(frame, frame, Size(int(frame.cols * scale), int(frame.rows * scale)));
        // Inference
        Mat faces;
        tm.start();
        detector->detect(frame, faces);
        tm.stop();
        Mat result = frame.clone();
        // Draw results on the input image
        visualize(result, nFrame, faces, tm.getFPS(), borderWidth);
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
}

int main(int argc, char** argv) {
    auto parser = parseArguments(argc, argv);
    auto [capture, frameWidth, frameHeight] = initializeVideoCapture(parser);
    auto detector = initializeFaceDetector(parser.get<String>("fd_model"), parser.get<float>("score_threshold"), parser.get<float>("nms_threshold"), parser.get<int>("top_k"), frameWidth, frameHeight);
    processVideoFrames(capture, detector, parser.get<bool>("save"), parser.get<float>("scale"));
    std::cout << "Done." << std::endl;
    return 0;
}