cmake_minimum_required(VERSION 3.17)
project(sdp_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(sdp_project main.cpp dataset/Dataset.cpp dataset/Dataset.h dataset/DataRecord.cpp dataset/DataRecord.h dataset/RawDataRecord.cpp dataset/RawDataRecord.h)