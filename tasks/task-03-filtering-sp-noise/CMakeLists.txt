cmake_minimum_required(VERSION 3.6)

project(task03)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task03 task-03-filtering-sp-noise.cpp)

target_link_libraries(task03 ${OpenCV_LIBS})
