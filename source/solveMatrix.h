#include "global.h"
	
void fixProb( Eigen::SparseMatrix<double> mat, const unsigned int popSize, const unsigned int numStates, float *fixProbAllStates);
void time( Eigen::SparseMatrix<double> mat, const unsigned int popSize, const unsigned int numStates, float *fixTimeAllStates);
