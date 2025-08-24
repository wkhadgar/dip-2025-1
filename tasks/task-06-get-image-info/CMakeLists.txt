cmake_minimum_required(VERSION 3.11)

project(task06)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task06 task-06-get-image-info.cpp)

target_link_libraries(task06 ${OpenCV_LIBS})
