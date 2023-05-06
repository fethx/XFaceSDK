
# 添加全部源码文件
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)

add_executable(xfaceapp ${srcs})

# 设置头文件搜索路径 
target_include_directories(xfaceapp PUBLIC include)

# 链接 xfacesdk 文件
target_link_libraries(xfaceapp PUBLIC xfacesdk)

