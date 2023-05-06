/*
 * @Description: 
 * @Date: 2023-04-24 01:15:11
 * @LastEditTime: 2023-05-06 03:31:36
 * @FilePath: /XFaceSDK/xfacesdk/src/sdk.cpp
 */
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

#include <xfacesdk/sdk.h>

using json = nlohmann::json;

namespace XFace {

SDK::SDK() {
    std::ifstream f("xfacesdk/config/config.json");
    json data = json::parse(f);
    json sdk = data["SDK"];
    detectorConfig = sdk["detector"];
    recognizerConfig = sdk["recognizer"];
}

// 析构函数
SDK::~SDK() {

}

std::string SDK::getVersion() { return "1.0.0"; }

cv::Mat SDK::faceDetect(cv::Mat image) {
    if(detectorConfig.compare("YuNet") == 0) {
        // 使用 YuNet, 如果使用另一个算法，就更换 Factory
        dfactory = std::make_unique<YuNetDetectorFactory>();
    } else if(detectorConfig.compare("LBP") == 0) {
        dfactory = std::make_unique<LBPDetectorFactory>();
    } else {
        std::cout << "[xfacesdk] Wrong detect algorithm!" << "\n";
        return cv::Mat();
    }

    detector_ = dfactory->createDetector();

    return detector_->detect(image);
}

std::pair<cv::Mat, bool> SDK::signUp(cv::Mat image, std::unordered_map<int, cv::Mat>& userMap) {
    cv::Mat face = faceDetect(image);
    bool is_exist = false;
    if(face.empty()) {
        std::cout << "[xfacesdk] No face detected!" << "\n";
        return std::make_pair(cv::Mat(), is_exist);
    }

    if(recognizerConfig.compare("SFace") == 0) {
        rfactory = std::make_unique<SFaceRecognizerFactory>();
    } else {
        std::cout << "[xfacesdk] Wrong recognize algorithm!" << "\n";
        return {};
    }

    recognizer_ = rfactory->createRecognizer();
    
    return recognizer_->signUp(image, face, userMap);
}

bool SDK::logIn(cv::Mat image, std::unordered_map<int, cv::Mat>& userMap) {
    cv::Mat face = faceDetect(image);

    if(face.empty()) {
        std::cout << "[xfacesdk] No face detected!" << "\n";
        return false;
    }

    if(recognizerConfig.compare("SFace") == 0) {
        rfactory = std::make_unique<SFaceRecognizerFactory>();
    } else {
        std::cout << "[xfacesdk] Wrong recognize algorithm!" << "\n";
        return false;
    }

    recognizer_ = rfactory->createRecognizer();

    return recognizer_->logIn(image, face, userMap);
}

} // namespace XFace