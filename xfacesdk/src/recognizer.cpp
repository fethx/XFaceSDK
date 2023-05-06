/*
 * @Description: 
 * @Date: 2023-05-04 02:54:13
 * @LastEditTime: 2023-05-06 03:23:38
 * @FilePath: /XFaceSDK/xfacesdk/src/recognizer.cpp
 */
#include <xfacesdk/recognizer.h>

using json = nlohmann::json;

namespace XFace {

std::pair<cv::Mat, bool> SFaceRecognizer::signUp(cv::Mat image, cv::Mat face, std::unordered_map<int, cv::Mat>& userMap) {
    bool is_exist = false;

    std::ifstream f("xfacesdk/config/config.json");
    json data = json::parse(f);
    json sface = data["recognizer"]["SFace"];

    std::string model_path = sface["model_path"];
    float cosine_threshold = sface["cosine_threshold"];
    float normL2_threshold = sface["normL2_threshold"];

    // 初始化 SFace
    SFace model(model_path, cosine_threshold, normL2_threshold);

    // 在人脸检测部分的基础上, 对齐检测到的首个人脸(faces.row(0)), 保存至aligned_face。
    cv::Mat aligned_face = model.alignCrop(image, face);

    cv::Mat feature = model.feature(aligned_face);

    // TODO: 目前注册条件不够严格，容易出现重复注册。限制条件，只允许正脸注册。
    if(!userMap.empty()) {
        // 比对两张人脸的特征feature1，feature2以确认身份
        for(const auto& [key, value] : userMap) {
            double score = model.match(feature, value);
            if(score <= normL2_threshold) {
                std::cout << "[xfacesdk] User has registered.\n";
                // 返回一个空矩阵
                is_exist = true;
                return std::make_pair(cv::Mat(), is_exist);
            }
        }
    }

    return std::make_pair(feature, is_exist);
}

bool SFaceRecognizer::logIn(cv::Mat image, cv::Mat face, std::unordered_map<int, cv::Mat>& userMap) {
    std::ifstream f("xfacesdk/config/config.json");
    json data = json::parse(f);
    json sface = data["recognizer"]["SFace"];

    std::string model_path = sface["model_path"];
    float cosine_threshold = sface["cosine_threshold"];
    float normL2_threshold = sface["normL2_threshold"];

    // 初始化 SFace
    SFace model(model_path, cosine_threshold, normL2_threshold);

    // 在人脸检测部分的基础上, 对齐检测到的首个人脸(faces.row(0)), 保存至aligned_face。
    cv::Mat aligned_face = model.alignCrop(image, face);

    cv::Mat feature = model.feature(aligned_face);

    // 比对两张人脸的特征feature1，feature2以确认身份
    if(!userMap.empty()) {
        for(const auto& [key, value] : userMap) {
            double score = model.match(feature, value);
            if(score <= normL2_threshold) {
                std::cout << "[xfacesdk] Login Success.\n";
                return true;
            }
        }
    }
    return false;
}

SFace::SFace(const std::string& model_path, float cosine_threshold, float normL2_threshold) :
    model_path_(model_path),cosine_threshold_(cosine_threshold),normL2_threshold_(normL2_threshold) 
{
    model = cv::FaceRecognizerSF::create(model_path, "");
}

cv::Mat SFace::alignCrop(cv::Mat image, cv::Mat face) {
    cv::Mat res;
    model->alignCrop(image, face, res);
    return res;
}

cv::Mat SFace::feature(cv::Mat image) {
    cv::Mat res;
    model->feature(image, res);
    return res;
}

double SFace::match(cv::Mat feature1, cv::Mat feature2) {
    return model->match(feature1, feature2, cv::FaceRecognizerSF::DisType::FR_NORM_L2);
}

} // namespace XFace