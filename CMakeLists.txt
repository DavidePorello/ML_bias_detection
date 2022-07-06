
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

add_executable(${PROJECT_NAME} main.cpp dataset/Dataset.cpp dataset/Dataset.h dataset/CleanedDataset.cpp dataset/CleanedDataset.h dataset/RawDataRecord.cpp dataset/RawDataRecord.h bin/ml/utilities.cpp
         dataset/CleanedDataset.cpp dataset/CleanedDataset.h bin/dataset_alternator/DatasetAlternator.cpp bin/dataset_alternator/DatasetAlternator.h bin/thread_pool/ThreadPool.cpp bin/thread_pool/ThreadPool.h bin/dataset_alternator/AlternationTask.cpp bin/kfold/KFoldTask.cpp bin/thread_pool/Task.h bin/kfold/KFold.cpp bin/kfold/KFold.h
        bin/ml/utilities.cpp bin/ml/LinearRegression.h bin/ml/LinearRegression.cpp
        bin/ml/PolynomialRegression.h bin/ml/PolynomialRegression.cpp bin/ml/ModelML.h bin/plot/plot_main.cpp bin/plot/PlotML.h
        bin/plot/PlotML.cpp bin/post_process.cpp bin/post_process.h bin/dataset_alternator/AlternationTask.h bin/kfold/KFoldTask.h) #The name of the cpp file and its path can vary

target_link_libraries (${PROJECT_NAME} Eigen3::Eigen)