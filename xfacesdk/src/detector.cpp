/*
 * @Description: 
 * @Date: 2023-05-02 06:39:20
 * @LastEditTime: 2023-05-06 04:11:12
 * @FilePath: /XFaceSDK/xfacesdk/src/detector.cpp
 */
#include <xfacesdk/detector.h>
#include <xfacesdk/imageutils.h>

using json = nlohmann::json;

namespace XFace {

YuNet::YuNet(const std::string& model_path,
        const cv::Size& input_size,
        float conf_threshold,
        float nms_threshold,
        int top_k,
        int backend_id,
        int target_id)
    : model_path_(model_path), input_size_(input_size),
        conf_threshold_(conf_threshold), nms_threshold_(nms_threshold),
        top_k_(top_k), backend_id_(backend_id), target_id_(target_id)
{
    model = cv::FaceDetectorYN::create(model_path_, "", input_size_, conf_threshold_, nms_threshold_, top_k_, backend_id_, target_id_);
}

void YuNet::setBackendAndTarget(int backend_id, int target_id) {
    backend_id_ = backend_id;
    target_id_ = target_id;
    model = cv::FaceDetectorYN::create(model_path_, "", input_size_, conf_threshold_, nms_threshold_, top_k_, backend_id_, target_id_);
}

void YuNet::setInputSize(const cv::Size& input_size) {
    input_size_ = input_size;
    model->setInputSize(input_size_);
}

cv::Mat YuNet::infer(const cv::Mat image) {
    cv::Mat res;
    model->detect(image, res);
    return res;
}

cv::Mat YuNetDetector::detect(cv::Mat image) {
    const std::map<std::string, int> str2backend {
        {"opencv", cv::dnn::DNN_BACKEND_OPENCV}, {"cuda", cv::dnn::DNN_BACKEND_CUDA},
        {"timvx",  cv::dnn::DNN_BACKEND_TIMVX},  {"cann", cv::dnn::DNN_BACKEND_CANN}
    };
    const std::map<std::string, int> str2target {
        {"cpu", cv::dnn::DNN_TARGET_CPU}, {"cuda", cv::dnn::DNN_TARGET_CUDA},
        {"npu", cv::dnn::DNN_TARGET_NPU}, {"cuda_fp16", cv::dnn::DNN_TARGET_CUDA_FP16}
    };

    std::ifstream f("xfacesdk/config/config.json");
    json data = json::parse(f);
    json yunet = data["detector"]["YuNet"];

    std::string model_path = yunet["model_path"];
    std::string backend = yunet["backend"];
    std::string target = yunet["target"];
    bool save_flag = yunet["save"];
    bool vis_flag = yunet["vis"];

    // model params
    float conf_threshold = yunet["conf_threshold"];
    float nms_threshold = yunet["nms_threshold"];
    int top_k = yunet["top_k"];
    const int backend_id = str2backend.at(backend);
    const int target_id = str2target.at(target);

    // Instantiate YuNet
    YuNet model(model_path, cv::Size(320, 320), conf_threshold, nms_threshold, top_k, backend_id, target_id);

    // Inference
    model.setInputSize(image.size());
    auto faces = model.infer(image);

    // Print faces
    std::cout << cv::format("[xfacesdk] %d faces detected!\n", faces.rows);
    for (int i = 0; i < faces.rows; ++i) {
        int x1 = static_cast<int>(faces.at<float>(i, 0));
        int y1 = static_cast<int>(faces.at<float>(i, 1));
        int w = static_cast<int>(faces.at<float>(i, 2));
        int h = static_cast<int>(faces.at<float>(i, 3));
        float conf = faces.at<float>(i, 14);
        // std::cout << cv::format("%d: x1=%d, y1=%d, w=%d, h=%d, conf=%.4f\n", i, x1, y1, w, h, conf);
    }

    // Draw reults on the input image
    if (save_flag || vis_flag) {
        auto res_image = visualize(image, faces);
        if (save_flag) {
            std::cout << "[xfacesdk] Detection results are saved to result.jpg\n";
            cv::imwrite("result.jpg", res_image);
        }
        if (vis_flag) {
            cv::namedWindow("XFaceSDK", cv::WINDOW_AUTOSIZE);
            cv::imshow("XFaceSDK", res_image);
            cv::waitKey(1);
        }
    }
    
    if(faces.empty()) {
        return cv::Mat();
    } else {
        return faces.row(0); // 返回最大脸
    }
}

cv::Mat LBPDetector::detect(cv::Mat image) {
    std::ifstream f("xfacesdk/config/config.json");
    json data = json::parse(f);
    json lbp = data["detector"]["LBP"];

    std::string model_path = lbp["model_path"];

    cv::CascadeClassifier face_cascade;
    face_cascade.load(model_path);

    // 转换成灰度图片
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);  
    
    // 检测人脸
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 5);

    // 绘制人脸矩形框
    for (cv::Rect face : faces) {
        cv::rectangle(image, face, cv::Scalar(0, 255, 0), 2);
    }

    // 显示图片
    cv::namedWindow("XFaceSDK", cv::WINDOW_AUTOSIZE);
    cv::imshow("XFaceSDK", image); 
    
    return image;

}

} // namespace XFace