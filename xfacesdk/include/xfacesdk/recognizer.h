/*
 * @Description: 
 * @Date: 2023-05-04 02:54:22
 * @LastEditTime: 2023-05-06 01:15:43
 * @FilePath: /XFaceSDK/xfacesdk/include/xfacesdk/recognizer.h
 */
#pragma once

#include <fstream>

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

namespace XFace {

// 抽象类
class recognizer {
public:
    // 注册
    virtual std::pair<cv::Mat, bool> signUp(cv::Mat image, cv::Mat face, std::unordered_map<int, cv::Mat>& userMap)=0;
    // 登录
    virtual bool logIn(cv::Mat image, cv::Mat face, std::unordered_map<int, cv::Mat>& userMap)=0;
    // virtual ~detector();
};

// 工厂基类
class recognizerFactory {
public:
    virtual std::unique_ptr<recognizer> createRecognizer()=0;
    // virtual ~detectorFactory();
};

// 具体类 SFace
class SFaceRecognizer : public recognizer {
public:
    std::pair<cv::Mat, bool> signUp(cv::Mat image, cv::Mat face, std::unordered_map<int, cv::Mat>& userMap) override;
    bool logIn(cv::Mat image, cv::Mat face, std::unordered_map<int, cv::Mat>& userMap) override;
};

// 具体工厂 SFace
class SFaceRecognizerFactory : public recognizerFactory {
public:
    std::unique_ptr<recognizer> createRecognizer() {
        return std::make_unique<SFaceRecognizer>();
    }
};

class SFace {
public:
    SFace(const std::string& model_path, float cosine_threshold_, float normL2_threshold_);
    
    // 图片预处理, 人脸对齐
    cv::Mat alignCrop(cv::Mat image, cv::Mat face);
    // 特征值提取
    cv::Mat feature(cv::Mat image);
    // 比对
    double match(cv::Mat feature1, cv::Mat feature2);

private:
    cv::Ptr<cv::FaceRecognizerSF> model;
    std::string model_path_;
    float cosine_threshold_;
    float normL2_threshold_;
};

} // namespace XFace