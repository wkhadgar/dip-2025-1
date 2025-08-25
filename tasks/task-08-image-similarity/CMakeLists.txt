cmake_minimum_required(VERSION 3.11)

project(task08)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task08 task-08-image-similarity.cpp)

target_link_libraries(task08 ${OpenCV_LIBS})
