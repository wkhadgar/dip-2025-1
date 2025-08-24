cmake_minimum_required(VERSION 3.11)

project(task07)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task07 task-07-linear-combination.cpp)

target_link_libraries(task07 ${OpenCV_LIBS})
