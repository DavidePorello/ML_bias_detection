
cmake_minimum_required(VERSION 3.17)
project(sdp_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

add_executable(${PROJECT_NAME} main.cpp dataset/Dataset.cpp dataset/Dataset.h dataset/RawDataRecord.cpp dataset/RawDataRecord.h bin/utilities.cpp bin/LinearRegression.h bin/LinearRegression.cpp
        bin/ModelML.h bin/ModelML.cpp dataset/CleanedDataset.cpp dataset/CleanedDataset.h bin/BiasDetection.cpp bin/BiasDetection.h bin/ThreadPool.cpp bin/ThreadPool.h bin/Task.cpp bin/Task.h) #The name of the cpp file and its path can vary

target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)