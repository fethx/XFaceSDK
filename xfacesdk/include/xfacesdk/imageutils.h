/*
 * @Description: 
 * @Date: 2023-05-03 00:44:50
 * @LastEditTime: 2023-05-09 03:20:25
 * @FilePath: /XFaceSDK/xfacesdk/include/xfacesdk/imageutils.h
 */
#pragma once

#include <opencv2/opencv.hpp>

namespace XFace {

cv::Mat visualize(const cv::Mat& image, const cv::Mat& faces, float fps = -1.f);

} // namespace XFace