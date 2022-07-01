cmake_minimum_required(VERSION 3.17)
project(sdp_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )

add_executable(sdp_project main.cpp dataset/Dataset.cpp dataset/Dataset.h dataset/RawDataRecord.cpp dataset/RawDataRecord.h dataset/CleanedDataset.cpp dataset/CleanedDataset.h bin/BiasDetection.cpp bin/BiasDetection.h bin/ThreadPool.cpp bin/ThreadPool.h)

# add Armadillo library
include_directories("C:/Program Files/armadillo-11.2.1/include")
