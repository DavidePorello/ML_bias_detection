cmake_minimum_required(VERSION 3.17)
project(sdp_project)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

add_executable(sdp_project main.cpp dataset/Dataset.cpp dataset/Dataset.h dataset/RawDataRecord.cpp dataset/RawDataRecord.h dataset/CleanedDataset.cpp dataset/CleanedDataset.h)

target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)