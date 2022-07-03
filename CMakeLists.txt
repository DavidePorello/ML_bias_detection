
cmake_minimum_required(VERSION 3.17)
project(sdp_project)

set(CMAKE_CXX_STANDARD 17)

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

add_executable(${PROJECT_NAME} main.cpp bin/utilities.cpp bin/LinearRegression.h bin/LinearRegression.cpp
        bin/PolynomialRegression.h bin/PolynomialRegression.cpp bin/ModelML.h bin/plot_main.cpp bin/PlotML.h
        bin/PlotML.cpp)

target_link_libraries (${PROJECT_NAME} Eigen3::Eigen)