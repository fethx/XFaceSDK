/*
 * @Description: 使用工厂模式创建不同的 detector
 * @Date: 2023-05-02 06:39:09
 * @LastEditTime: 2023-05-06 00:08:34
 * @FilePath: /XFaceSDK/xfacesdk/include/xfacesdk/detector.h
 */
// #include <memory>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

namespace XFace {

// 抽象类
class detector {
public:
    // 推理函数
    virtual cv::Mat detect(cv::Mat image) = 0;
    // virtual ~detector();
};

// 工厂基类
class detectorFactory {
public:
    virtual std::unique_ptr<detector> createDetector()=0;
    // virtual ~detectorFactory();
};

// 具体类 YuNet
class YuNetDetector : public detector {
public:
    cv::Mat detect(cv::Mat image);
};
// 具体类 LBP
class LBPDetector : public detector {
public:
    cv::Mat detect(cv::Mat image);
};

// 具体工厂 YuNet
class YuNetDetectorFactory : public detectorFactory {
public:
    std::unique_ptr<detector> createDetector() {
        return std::make_unique<YuNetDetector>();
    }
};
// 具体工厂 LBP
class LBPDetectorFactory : public detectorFactory {
public:
    std::unique_ptr<detector> createDetector() {
        return std::make_unique<LBPDetector>();
    }
};

class YuNet {
public:
    YuNet(const std::string& model_path,
        const cv::Size& input_size = cv::Size(320, 320),
        float conf_threshold = 0.6f,
        float nms_threshold = 0.3f,
        int top_k = 5000, // 检测人脸数量
        int backend_id = 0,
        int target_id = 0);

    void setBackendAndTarget(int backend_id, int target_id);

    // Overwrite the input size when creating the model. Size format: [Width, Height].
    void setInputSize(const cv::Size& input_size);

    cv::Mat infer(const cv::Mat image);

private:
    cv::Ptr<cv::FaceDetectorYN> model;

    std::string model_path_;
    cv::Size input_size_;
    float conf_threshold_;
    float nms_threshold_;
    int top_k_;
    int backend_id_;
    int target_id_;
};

} // namespace XFace