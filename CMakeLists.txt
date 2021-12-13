cmake_minimum_required(VERSION 3.5)

project(Lab_2_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE( OpenCV REQUIRED ) # 3.4.14
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )
link_directories( ${OpenCV_LIBRARY_DIR} )

add_executable(Lab_2_project main.cpp filters.cpp utilities.cpp)
TARGET_LINK_LIBRARIES (Lab_2_project ${OpenCV_LIBRARIES} )
