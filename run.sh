###
 # @Description: 
 # @Date: 2023-04-24 10:08:08
 # @LastEditTime: 2023-05-07 04:29:45
 # @FilePath: /XFaceSDK/run.sh
### 

cmake -B build
cmake --build build --target xfaceapp -- -j8
build/xfaceapp/xfaceapp
