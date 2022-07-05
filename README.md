# sdp-project

### Dataset

The [_Census-Income Data Set_](https://archive.ics.uci.edu/ml/machine-learning-databases/census-income-mld/census.tar.gz) was downloaded and extracted as-is inside the _dataset_ folder

### Installing Eigen

[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.  
In order to build and use this project is necessary to download and install it

- ##### Windows
    Requires [CMake](https://cmake.org/download/) and Visual Studio.
  1. Download Eigen from [Eigen website](https://eigen.tuxfamily.org/index.php?title=Main_Page)
  2. Extract Eigen in `C:/eigen`
  3. Create the subdirectory `C:/eigen/build`
  4. Open the command prompt with **admin privileges**, and navigate to such directory with `cd C:/eigen/build`
  5. Run `cmake ..`
  6. Run `cmake --build . --target install`. If it fails due to missing authorization, try to close and re-open the command prompt with **admin privileges**

- ##### Linux
  Requires CMake (`sudo apt install cmake`)
    1. Download Eigen from [Eigen website](https://eigen.tuxfamily.org/index.php?title=Main_Page)
    2. Extract Eigen in any folder `[path]/eigen`
    3. Create the subdirectory `[path]/eigen/build`
    4. Open the terminal and navigate to such directory with `cd [path]/eigen/build`
    5. Run `sudo cmake ..`
    6. Run `sudo cmake --build . --target install`


### Installing sciplot
sciplot does not require any installation (it is already managed via CMake).  
However, in order to work, it requires `gnuplot`.  

- ##### Windows
  1. Download `gp543-win64-mingw.exe` from [here](https://sourceforge.net/projects/gnuplot/files/gnuplot/5.4.3/)
  2. Run the executable
  3. Remember to check the option "_**Add application directory to your PATH environment variable**_"
  4. If your IDE was already open, close and re-open it, to refresh the PATH vaiable content  

- ##### Linux
    install gnuplot with `sudo apt install gnuplot`
    

### Before running the code

Please note that we wrote and tested the code in CLion.
Clion sets as default working directory the folder `cmake_build_debug`, but the code must be run from the root folder (where `main.cpp` is).
This can be done by editing the default configuration in CLion
