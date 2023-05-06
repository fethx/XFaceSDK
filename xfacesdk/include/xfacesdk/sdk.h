/*
 * @Description: 
 * @Date: 2023-04-24 01:28:33
 * @LastEditTime: 2023-05-06 01:11:41
 * @FilePath: /XFaceSDK/xfacesdk/include/xfacesdk/sdk.h
 */
#pragma once

#include <xfacesdk/detector.h>
#include <xfacesdk/recognizer.h>

namespace XFace {

class SDK {
public:
    // 根据配置信息初始化
    SDK();
    ~SDK();

    // 获取SDK版本
    static std::string getVersion();

    // 人脸检测
    cv::Mat faceDetect(cv::Mat image);

    // 注册，（检测人脸，图片预处理，特征值提取, 比对）
    // 返回值, 如何不存在该用户, 返回特征值; bool 为是否已存在
    std::pair<cv::Mat, bool> signUp(cv::Mat image, std::unordered_map<int, cv::Mat>& userMap);

    // 登录，（检测人脸，图片预处理，特征值提取，比对）
    bool logIn(cv::Mat image, std::unordered_map<int, cv::Mat>& userMap);

private:
    std::string detectorConfig;
    std::unique_ptr<detectorFactory> dfactory;
    std::unique_ptr<detector> detector_;

    std::string recognizerConfig;
    std::unique_ptr<recognizerFactory> rfactory;
    std::unique_ptr<recognizer> recognizer_;
};

} // namespace XFace