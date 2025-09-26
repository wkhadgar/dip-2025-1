cmake_minimum_required(VERSION 3.11)

project(task10)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task10 task-10-histogram-matching.cpp)

target_link_libraries(task10 ${OpenCV_LIBS})
