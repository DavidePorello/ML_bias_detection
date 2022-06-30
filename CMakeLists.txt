cmake_minimum_required(VERSION 3.17)
project(sdp_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(sdp_project main.cpp dataset/Dataset.cpp dataset/Dataset.h dataset/RawDataRecord.cpp dataset/RawDataRecord.h dataset/CleanedDataset.cpp dataset/CleanedDataset.h)

# add MLpack library
include_directories("C:/Program Files/armadillo-11.2.1/include")

# add MLpack library
include_directories("C:/Program Files/mlpack")
target_link_libraries(sdp_project "C:/Program Files/mlpack/lib/mlpack.lib")