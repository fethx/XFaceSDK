<!--
 * @Description: 
 * @Date: 2023-05-06 04:33:27
 * @LastEditTime: 2023-05-06 12:26:19
 * @FilePath: /XFaceSDK/README.md
-->
# XFaceSDK

基于 OpenCV 开发，其中人脸检测算法为 YuNet，人脸识别算法为 SFace。

整个项目分为两部分：
1. [SDK](xfacesdk): 提供注册和登录接口
2. [APP](xfaceapp): 调用 SDK 实现了注册和登录功能


|   开发环境   |  版本         |
| ----------- | -----------   |
| Linux       | Ubuntu 20.04.6|
| C++         | C++17         |
| CMake       | 3.16.3        |
| [OpenCV](https://github.com/opencv/opencv/releases/)| 4.7.0 |
| [json](https://github.com/nlohmann/json) | 3.11.2 |
| [spdlog](https://github.com/gabime/spdlog) | 1.11.0 |
