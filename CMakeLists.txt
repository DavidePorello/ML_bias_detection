
cmake_minimum_required(VERSION 3.17)
project(sdp_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# include and download sciplot
include(FetchContent)
FetchContent_Declare(sciplot_content
        GIT_REPOSITORY https://github.com/sciplot/sciplot.git
        GIT_TAG master)

FetchContent_GetProperties(sciplot_content)
if(NOT sciplot_content_POPULATED)
    FetchContent_Populate(sciplot_content)
endif()

include_directories(${sciplot_content_SOURCE_DIR})

# include Eigen
find_package (Eigen3 3.4 REQUIRED NO_MODULE)

add_executable(${PROJECT_NAME} main.cpp dataset/Dataset.cpp dataset/Dataset.h dataset/RawDataRecord.cpp dataset/RawDataRecord.h bin/utilities.cpp
         dataset/CleanedDataset.cpp dataset/CleanedDataset.h bin/BiasDetection.cpp bin/BiasDetection.h bin/ThreadPool.cpp bin/ThreadPool.h bin/AlternationTask.cpp bin/KFoldTask.cpp bin/Task.h bin/KFold.cpp bin/KFold.h
        bin/utilities.cpp bin/LinearRegression.h bin/LinearRegression.cpp
        bin/PolynomialRegression.h bin/PolynomialRegression.cpp bin/ModelML.h bin/plot_main.cpp bin/PlotML.h
        bin/PlotML.cpp) #The name of the cpp file and its path can vary

target_link_libraries (${PROJECT_NAME} Eigen3::Eigen)