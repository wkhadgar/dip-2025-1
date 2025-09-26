cmake_minimum_required(VERSION 3.11)

project(task09)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(task09 task-09-geometric-transformations.cpp)

target_link_libraries(task09 ${OpenCV_LIBS})
