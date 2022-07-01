
cmake_minimum_required(VERSION 3.17)
project(sdp_project)

set(CMAKE_CXX_STANDARD 17)

find_package (Eigen3 3.4 REQUIRED NO_MODULE)

add_executable(${PROJECT_NAME} main.cpp bin/utilities.cpp) #The name of the cpp file and its path can vary

target_link_libraries (${PROJECT_NAME} Eigen3::Eigen)