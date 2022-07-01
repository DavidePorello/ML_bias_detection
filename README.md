# sdp-project


### Dataset

The [_Census-Income Data Set_](https://archive.ics.uci.edu/ml/machine-learning-databases/census-income-mld/census.tar.gz) was downloaded and extracted as-is inside the _dataset_ folder

### Installing Eigen

[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.  
In order to build and use this project is necessary to download and install it (procedure tested on Windows).  
Requires [CMake](https://cmake.org/download/) and Visual Studio.
1. Download Eigen from [Eigen website](https://eigen.tuxfamily.org/index.php?title=Main_Page)
2. Extract Eigen in `C:/eigen`
3. Create the subdirectory `C:/eigen/build`
4. Open the command prompt with **admin privileges**, and navigate to such directory with `cd C:/eigen/build`
5. Run `cmake ..`
6. Run `cmake --build . --target install`. If it fails due to missing authorization, try to close and re-open the command prompt with **admin privileges**
