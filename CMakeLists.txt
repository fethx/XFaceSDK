cmake_minimum_required(VERSION 3.16)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O3")  

enable_language(CXX)
# 查找 pthread 库
find_package(Threads REQUIRED)

# 添加 opencv 动态库文件
file(GLOB opencv_libs 3rdparty/opencv/lib/*.so)
# 添加 spdlog 静态库文件
file(GLOB spdlog_libs 3rdparty/spdlog/lib/libspdlog.a)

project(XFace LANGUAGES CXX)

add_subdirectory(xfacesdk)
add_subdirectory(xfaceapp)

# opencv 头文件路径
target_include_directories(xfacesdk PUBLIC 3rdparty/opencv/include/opencv4)
# spdlog 头文件
target_include_directories(xfacesdk PUBLIC 3rdparty/spdlog/include)
# json 头文件
target_include_directories(xfacesdk PUBLIC 3rdparty/json/include)

# 链接 opencv 库文件
target_link_libraries(xfacesdk PUBLIC ${opencv_libs})
# 链接 spdlog 库文件
target_link_libraries(xfacesdk PUBLIC ${spdlog_libs})
# spdlog 异步日志需要 pthread 库
target_link_libraries(xfaceapp PUBLIC Threads::Threads)

