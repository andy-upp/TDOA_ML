# TDOA_ML
Machine learning based optimization on TDOA localization.

This project is to try something interesting on TDOA localization problem. The Time Difference of Arrival(TDOA) localization is widely used in radar localization and acoustic underwater localization. Nowadays, it is also popular for localizations between robots in IoT.

There are many ways to solve a TDOA localization, like solving hyperbola geometric and solving polynomial functions. In this project we use numerical optimization method, like Newton method, to solve the problem iteratively.

One important thing for Newton method is to give an appropriate initial point, but this is usually hard to find in the real world due to the noise. Therefore, we are going to do some research on machine learning ways to find a good initial point for Newton method in TDOA, and implement the algorithm to running on NIVIDA GPUs to judge the utility of this way to solve TDOA localization.

## How To Use
Clone this repo to your local device. Enter the project root directory, then in your terminal, type and enter `make main` or `make` to build the project. After the successful building, type and enter `main` or `./main` to run the program.
To clean the .exe and .o files, type and enter `make clean`
