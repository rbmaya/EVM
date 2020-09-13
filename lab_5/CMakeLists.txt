cmake_minimum_required(VERSION 3.1)
project(lab5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV 4.1.2 REQUIRED )

add_executable( lab_5 main.cpp)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( lab_5 ${OpenCV_LIBS} )
