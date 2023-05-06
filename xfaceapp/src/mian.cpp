/*
 * @Description: 
 * @Date: 2023-04-24 02:20:40
 * @LastEditTime: 2023-05-06 04:19:27
 * @FilePath: /XFaceSDK/xfaceapp/src/mian.cpp
 */
#include <iostream>
#include <opencv2/opencv.hpp>

#include <xfaceapp/log.h>
#include <xfacesdk/sdk.h>

using json = nlohmann::json;

// 注册
bool signUp(std::string userFilePath);

// 登录
bool logIn(std::string userFilePath, int try_time);

// 序列化
void mat2Json(std::unordered_map<int, cv::Mat> userMap, std::string userFilePath);
// 反序列化
std::unordered_map<int, cv::Mat> json2Mat(std::string userFilePath);


int main() {
    log::initLogger();

    std::string version = XFace::SDK::getVersion();
    LOG_INFO("SDK version: {}", version);
    
    std::string userFilePath = "doc/user.json";

    std::string configFilePath = "xfaceapp/config/config.json";

    std::ifstream f(configFilePath);
    json data = json::parse(f);
    std::string function = data["function"];

    if(function.compare("signUp") == 0) {
        signUp(userFilePath);
    } else if(function.compare("logIn") == 0) {
        int try_time = data["logIn"]["try_time"];
        logIn(userFilePath, try_time);
    } else {
        LOG_ERROR("Wrong function! Please modify the configuration file: {}", configFilePath);
    }
}

bool signUp(std::string userFilePath) {
    bool res = false;
    cv::Mat feature;
    std::pair<cv::Mat, bool> sdk_res;
    XFace::SDK sdk;
    std::unordered_map<int, cv::Mat> userMap;

    // 读取用户数据
    userMap = json2Mat(userFilePath);

    // 打开默认的网络摄像头
    int device_id = 0;
    auto cap = cv::VideoCapture(device_id);

    cv::TickMeter tm; 
    cv::Mat frame;
    while (!res) {
        bool has_frame = cap.read(frame);
        if (!has_frame) {
            LOG_ERROR("No frames grabbed! Exiting ...");
            break;
        }

        tm.start();
        sdk_res = sdk.signUp(frame, userMap);
        tm.stop();

        if(sdk_res.second) {
            LOG_INFO("Sign up fail, user already exist!");
            return false;
        }

        feature = sdk_res.first;
        if(!feature.empty()) {
            res = true;
            userMap[userMap.size()+1] = feature;
            // std::cout << feature <<"\n";
            
            LOG_INFO("Sign up success!");

            long long time_ms = tm.getTimeMilli();
            LOG_INFO("SDK sign up time spent: {} ms", time_ms);
        }

        tm.reset();
    }
    // 保存用户到本地
    mat2Json(userMap, userFilePath);
    return res;
}

bool logIn(std::string userFilePath, int try_time) {
    int try_time_ = try_time;
    bool sdk_res;
    XFace::SDK sdk;
    std::unordered_map<int, cv::Mat> userMap;

    // 读取用户数据
    userMap = json2Mat(userFilePath);

    // 打开默认的网络摄像头
    int device_id = 0;
    auto cap = cv::VideoCapture(device_id);

    cv::TickMeter tm; 
    cv::Mat frame;
  
    while (try_time_--) {
        bool has_frame = cap.read(frame);
        if (!has_frame) {
            LOG_ERROR("No frames grabbed! Exiting ...");
            break;
        }

        tm.start();
        sdk_res = sdk.logIn(frame, userMap);
        tm.stop();

        // 登录成功
        if(sdk_res) {
            LOG_INFO("Log in success!");

            long long time_ms = tm.getTimeMilli();
            LOG_INFO("SDK log in time spent: {} ms", time_ms);
            return true;
        }

        tm.reset();
    }
    LOG_INFO("Log in failed after {} times attempts!", try_time);
    return false;
}

void mat2Json(std::unordered_map<int, cv::Mat> userMap, std::string userFilePath) {
    cv::FileStorage fs(userFilePath, cv::FileStorage::WRITE);

    // 若文件打开失败
    if (!fs.isOpened()) {
        LOG_ERROR("Failed to open {}", userFilePath);
        return;
    }

    for(const auto& [key, value] : userMap) { 
        std::string str = std::to_string(key);
        fs << "usr_" + str << value;
    }
    fs.release(); 
}

std::unordered_map<int, cv::Mat> json2Mat(std::string userFilePath) {
    std::unordered_map<int, cv::Mat> um;
    cv::Mat feature;

    cv::FileStorage fs(userFilePath, cv::FileStorage::READ);
    
    // 若文件打开失败
    if (!fs.isOpened()) {
        LOG_ERROR("Failed to open {}", userFilePath);
        return um;
    }

    int i = 1;
    cv::FileNode fnode = fs.root();  
    if (fnode.begin() == fnode.end()) {
        LOG_INFO("No user data exist!");
        return um;
    } else {
        // 非空,可以访问子节点
        for (cv::FileNodeIterator it = fnode.begin(); it != fnode.end(); ++it) {
            std::string key = (*it).name();
            fs[key] >>  feature;
            um[i++] = feature;
        }
    }

    return um; 
}
