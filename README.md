#  Implementation of alternation functions in C++ for detection and evaluation of bias in ML
Authors: Ilaria Pilo, Davide Porello, Marco Porro  
Date: 04/07/2022  

## Table of contents
- [Introduction](#introduction)
- [Project design](#project-design)
- [Experimental results](#experimental-results)
- [Conclusion](#conclusion)
- [Installation](#installation)
- [References](#references)

## Introduction
The goal of the project is to implement the code relative to the _Detection and Evaluation of Machine Learning Bias_[[1]](#references) research paper in C++, and measure its performance.

## Project design
Given the starting dataset D[[2]](#references), the paper illustrates the following pipeline:
1. Train the model f(·) on the dataset D. 
2. Predict the class label for each data point in f(D) → ˆy. 
3. Apply the alternation function on the gender attribute ϕ(D) → Dϕ. 
4. Train the model f(·) on the alternative dataset Dϕ.
5. Predict the alternative predicted label f(Dϕ) → ˆyϕ.

To improve the performance of this operations, we decided to use parallelization, in the form of a thread pool, to compute concurrently the predictions of the machine learning model.
![Design_diagram](results/Design_diagram.png)

To help in the handling of the linear algebra computations we used the library [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page), and to produce the output plots we used [Sciplot](https://sciplot.github.io/).

### Preprocessing
The preprocessing is highly inspired by the paper, as we have reduced the dataset (files `dataset/censun-income.data` and `dataset/census-income.test`) from 300000 to 15000 records approximately.
We have selected a set of 10 attributes that, for us, are strictly related to the "wage" (that is the label).
We have filtered each categorial attribute (all of our attributes are categorial except the age) specifying part of the possible categorial values (see file `dataset/attributes.txt` for our choices and  `dataset/censun-income.names` for the list of all possible values).
The class `ClearedDataset` loads the dataset in an Eigen::MatrixXf and the labels in an Eigen::vectorXf taking the values from the file `dataset/cleaned-dataset.txt`, if the file is not present yet it creates the file using the class `Dataset`.
Attributes in `ClearedDataset`: age, education, marital stat, major industry code, major occupation code, race, sex, member of a labor union, full or part time employment stat and citizenship.

However, since the paper does not includes enough details on this step, our preprocessed dataset is different, and as such we also expect our findings to be different.

### K-fold and parallelization
As indicated in the paper, the model predictions must be evaluated using K-fold, with K=10. Additionally, this process must be repeated multiple times, on the original dataset and the alternated one(s).
Computing the model predictions is the most expensive task of the project, therefore we decided to parallelize it.
The simplest approach is to create a thread for each dataset, and then create K threads for each of the K folds. In each of these threads, the train and test sets are identified, the alternation function is computed on the test set (if needed), then the threads returns the mean and variance of the predictions inside a future.
To compute the KL and draw the graph we don't need to return the entire prediction vector.

Instead, we decided to create a more refined solution, using a thread pool of N worker threads.
For each fold of each dataset, a new task is created and enqueued in the thread pool. This decreases the memory requirements of the program, especially when detecting the bias of a PBA with many categories, and it allows for a better control of the parallelization.
If only 1 thread is created in the thread pool, the program is essentially sequential (plus a small overhead due the thread management), which allows us to measure the performance benefits of the parallelization.

In the code, this is done through the `TreadPool` class, which automatically precessed the enqueued task (class `KFoldTask`). Synchronization is handled thourgh a simple clock and condition variable.

Other kinds of parallelization can be proposed. For example, the alternation function or the variance calculations can be splitted in different threads, but this does not improves the performance, since they are not appreciably expensive, and the program already surpasses the number of hardware threads on any consumer machine.
### Machine Learning models
Unluckily, C++ does not appear to have any intuitive and portable machine learning library. For this reason, after long searches and attempts, we decided to develop our own models.  
Since [[1]](#references) uses machine learning to predict the wage (i.e., a continuous value), we developed two different regression algorithms:
- Linear regression (class `LinearRegression`)
- Polynomial regression (class `PolynomialRegression`)

Both classes extend the abstract class `ModelML`, which provides a general interface. This structure allows functions to use a generic model, so that we can easily select it in the program configuration phase.

Since the aim of the project was not to develop a machine learning algorithm, we decided to use a fully-sequential approach, having as core the Eigen library [[3]](#references). Eigen is a powerful C++ library supporting linear algebra operations; it is focused on versatility, performance and reliability.
#### Linear regression
Linear regression consists in interpreting pairs (sample, value) as points, and fitting them by means of a linear hyper-plane using the least square approach.  
Data is first centered, so that the mean of each feature is 0, and then fed to an Eigen solver based on Complete Orthogonal Decomposition. We chose this algorithm since it appears to be the best tradeoff between speed and reliability, working well also with matrices which are not full rank (information about other possible approaches can be found [here](https://eigen.tuxfamily.org/dox/group__LeastSquares.html#:~:text=An%20overdetermined%20system%20of%20equations,the%20Euclidean%20norm%20is%20used)). The result of this computation will be directly used to predict the value of unknown samples.
#### Polynomial regression
Polynomial regression is simply a special type of linear regression, where features are expanded considering all their possible combinations up to a specified degree. For example, if a sample has features [a, b, c] and the degree is 2, then the feature expansion will result in [1, a, b, c, ab, ac, ba, a<sup>2</sup>, b<sup>2</sup>, c<sup>2</sup>].  
For this reason, the `PolynomialRegression` class simply performs the feature expansion by means of a recursive function, and then calls the methods of `LinearRegression`.  
Notice that the computation of feature expansion is by definition a pretty expensive task. Moreover, since it increases the number of dimensions, it also increases the complexity of training and using the model. As a consequence, the application supports degrees only up to 4.
### Alternation function
The alternation function is a simple function that switches two categories of an attribute on a dataset. Since the time needed to compute this function is negligible (under 1 ms), it is computed sequentially (using multiple threads would just be an additional overhead).
It is calculated in the thread function, so different datasets are alternated in parallel.
### KL divergence computation
The KL divergence is computed using the same formula as in the paper. The KL divergence computation is not an expensive task (cost O(N), where N is the number of samples in the evaluation set). 
However, most of this cost comes from the calculation of the mean and variance of the evaluated sets. In our design we computed these metrics directly inside the thread pool (therefore taking advantege of the parallelization), and in the post process section at the end the code just computes the final formula, which requires less than a millisecond of time.
Therefore there is no need to consider additional measures to speed up this functionality.
## Experimental results
We have measured the performance of the program (excluding the dataset loading and the plots creation) using a different number of threads in the thread pool. The measurements have been collected with both the linear and polynomial regression and the attributes 'sex' and 'race' as PBA.

![Linear_regression_sex](results/LR_sex.png)
![Linear_regression_race](results/LR_race.png)
![Polynomial_regression_sex](results/PR_sex.png)
![Polynomial_regression_race](results/PR_race.png)

The program produces the following output plots. Here are reported only a few of the plots relative to the polynomial regression. All plots can be found in the `plots` folder after the code is executed.

![Alternation_female_male](results/alternation_Female_Male.jpg)
![Alternation_male_female](results/alternation_Male_Female.jpg)
![Bias_male_female](results/bias_evaluation_Female_Male.jpg)
![Alternation_black_white](results/alternation_Black_White.png)
![Alternation_white_black](results/alternation_White_Black.png)
![Bias_black_white](results/bias_evaluation_Black_White.png)
## Conclusion
After the development of the code, we can make some considerations:
- The paper states that the alternation function is computed on the dataset D. This is not precise, as with this approach the code would not produce appreciable results (it would just invert the category name in the dataset). Instead, the alternation function must be computed at each iteration on the test fold only.
- As seen in the alternation plots, our model predicts wages that are considerably different with respect to the paper's findings. Upon closer inspection, our results seem to be correct, as the average wage in our dataset is about 1800.
  A partial explanation for this behavious is that the difference might be caused by a different preprocessing of the original dataset.
  However, the same bias trends that are described in the paper can also be seen in our results.
- Our alternation plots are much closer together (there have an average delta of about 100), therefore the bias evaluation plots are not as neatly separated as in the paper, especially when using the polynomial regression on the "race" attribute.
- As expected, the linear regression model is considerably faster than the polynomial regression model. Additionally, the former generates comparably worse bias evaluation plots. 
  More in general, regression is not well suited for this application (the dataset contains mostly categorical attributes), but the actual predition precision is not in the scope of this project.
- The performance plots confirm the espected behaviour: running the code with a single thread gives the worst performance (the code is similar to a sequential one), and increasing the number of threads improves the performance.
  These benchmarks were collected on a machine with 4 hardware threads, and as expected, we see that after N=4 the performance does not improve as much. On the contrary, incrementing N further increases the thread management overhead, which contributes to a small increase in the processing time.
  To conclude, the most efficient way to run this code is to run it with a number of threads equal (or slightly superior) to the number of hardware threads of the machine.

## Installation

Before attempting to run the code, please read all the sections of this document.
The code was developed on Windows, but it also works on Linux (Ubuntu or derivatives)

### Dataset

The [_Census-Income Data Set_](https://archive.ics.uci.edu/ml/machine-learning-databases/census-income-mld/census.tar.gz) must be downloaded and extracted as-is inside the _dataset_ folder

### Installing Eigen

[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.  
In order to build and use this project is necessary to download and install it

- ##### Windows
    Requires [CMake](https://cmake.org/download/) and Visual Studio (with C++ building tools).
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
  4. If your IDE was already open, close and re-open it, to refresh the PATH variable content  

- ##### Linux
    install gnuplot with `sudo apt install gnuplot`
    

### Before running the code

Please note that we wrote and tested the code in CLion.
Clion sets as default working directory the folder `cmake_build_debug`, but the code must be run from the root folder (where `main.cpp` is).
This can be done by editing the default configuration in CLion

### Run the code

To run the code, just build `main.cpp` and run it. 
In the first part of the `main.cpp` file there are some useful configuration variables that can be changed to test different aspects of the code. 

## References
[1] 2021, _Detection and Evaluation of Machine Learning Bias._ Alelyani, Salem. Applied Sciences 11, no. 14: 6271. https://doi.org/10.3390/app11146271  
[2] _Census-Income (KDD) Data Set_. Machine Learning Repository. https://archive.ics.uci.edu/ml/datasets/Census-Income+%28KDD%29  
[3] _Eigen v3_. https://eigen.tuxfamily.org/index.php?title=Main_Page


