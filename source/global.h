#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <vector> 
#include <array>
#include <stdio.h>
#include <ctime>
#include <eigen3/Eigen/Sparse>
#include <string>
using namespace std;

typedef Eigen::SparseMatrix<double> SpMat;
typedef Eigen::Triplet<double> T;

//typedef std::array<unsigned int, (numStates-2)*(popSize)+numStates> t_ArrayInt;
//typedef std::array<float, (numStates-2)*(popSize)+numStates> t_ArrayFP;

typedef std::vector<unsigned int> t_vectorInt;
typedef std::vector<float> t_vectorFP;

#endif
