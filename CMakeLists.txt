cmake_minimum_required(VERSION 3.5)
project(fileParse)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(opencv_DIR "/home/yunzhongke/work/ws_opencv/share/OpenCV")
include(${opencv_DIR}/OpenCVConfig.cmake)
include_directories(${OpenCV_INCLUDE_DIR})
set(cmake_include_path ${CMAKE_INCLUDE_PATH} "/home/yunzhongke/work/ws_boost/include")
set(cmake_library_path ${CMAKE_LIBRARY_PATH} "/home/yunzhongke/work/ws_boost/lib")
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} boost_thread boost_system rt pthread boost_atomic boost_filesystem boost_regex ${OpenCV_LIBRARIES})
