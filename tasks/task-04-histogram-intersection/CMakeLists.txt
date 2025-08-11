cmake_minimum_required(VERSION 3.6)

project(task04)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task04 task-04-histogram-intersection.cpp)

target_link_libraries(task04 ${OpenCV_LIBS})
