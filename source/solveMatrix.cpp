#include "solveMatrix.h"

void fixProb(Eigen::SparseMatrix<double> mat, const unsigned int popSize, const unsigned int numStates, float *fixProbAllStates)
{
    /*
    Calculates the fixation probability for a given transition matrix.


    */
    
    Eigen::SparseMatrix<double> fixatingColumn(1,numStates-2);
    
    fixatingColumn = mat.block(numStates-1,1,1,numStates-2) * (-1); // this is the last column, it refers to the transitions into the all-mutant-state
    
    // cut off the first and last row and column (absorbing states)
    mat = mat.block(1,1,numStates-2,numStates-2);
    
    //calculate A-I
    for(unsigned int i = 0 ; i < numStates-2 ; i++)
    {
         mat.coeffRef(i,i) = mat.coeffRef(i,i) - 1;
    }
    mat = mat.transpose() ;
    
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double> > solver;
    solver.compute(mat);
    
    Eigen::VectorXd b(numStates-2);
    for(unsigned int i = 0 ; i < numStates-2 ; i++)
    {
         b(i) = fixatingColumn.coeffRef(0,i);
    }

    Eigen::VectorXd x(numStates-2);
    x = solver.solve(b); // solves (A-I)x = b
    

    fixProbAllStates[0] = 0;
    fixProbAllStates[numStates-1] = 1;
    for (unsigned int i = 1; i < numStates-1; i++)
    {
        
        fixProbAllStates[i] = x[i-1];
    }

}

void time(Eigen::SparseMatrix<double> mat, const unsigned int popSize, const unsigned int numStates, float *fixTimeAllStates)
{
    /*
    Calculates the unconditional fixation time for a given transition matrix.
    */
    
    
    // cut off the first and last row and column (absorbing states)
    mat = mat.block(1,1,numStates-2,numStates-2);
    // calculates A-I
    for(unsigned int i = 0 ; i < numStates-2 ; i++)
    {
         mat.coeffRef(i,i) = mat.coeffRef(i,i) - 1;
    }
    mat = mat.transpose() ;    
    Eigen::VectorXd b(numStates-2);
    for(unsigned int i = 0 ; i < numStates-2 ; i++)
    {
         b(i) = -1;
    }
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double> > solver;
    solver.compute(mat);
    Eigen::VectorXd x(numStates-2);
    x = solver.solve(b);    //solves (A-I)*x= b
    
    
    fixTimeAllStates[0] = 0;
    fixTimeAllStates[numStates-1] = 0;
    for (unsigned int i = 1; i < numStates-1; i++)
    {
        fixTimeAllStates[i] = x[i-1];
    }
    
    

}
