cmake_minimum_required(VERSION 2.8)

project(task01)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task01 task-01-generate-an-image.cpp)

target_link_libraries(task01 ${OpenCV_LIBS})
