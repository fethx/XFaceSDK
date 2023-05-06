# 添加全部源码文件
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)

# 生成静态库
add_library(xfacesdk SHARED ${srcs})

# 设置头文件搜索路径 
target_include_directories(xfacesdk PUBLIC include)
