cmake_minimum_required(VERSION 3.6)

project(task05)

find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})

add_executable(task05 task-05-load-image-from-url.cpp)

target_link_libraries(task05 ${OpenCV_LIBS} ${CURL_LIBRARIES})
