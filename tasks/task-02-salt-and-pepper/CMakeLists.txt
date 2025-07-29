cmake_minimum_required(VERSION 2.8)

project(task02)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task02 task-02-salt-and-pepper.cpp)

target_link_libraries(task02 ${OpenCV_LIBS})
