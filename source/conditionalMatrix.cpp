#include "conditionalMatrix.h"


void createConditionalTransitionMatrix(const unsigned int popSize, const unsigned int numStates, float *fixProbAllStates, t_vectorFP& data, t_vectorInt& row, t_vectorInt& col)
{
    /*
     This method takes the transition matrix and weights all entries by the ratio of the fixation probability in the state "going to" divided by "coming from".  
    
    */
    
    for(unsigned int i = 0 ; i < data.size() ; i++)
    {
        unsigned int rowNow = row[i];
        unsigned int colNow = col[i];
        
        
        if (fixProbAllStates[rowNow]==0) {
            data[i] = data[i];
        }
        else
        {
            data[i] = data[i] * fixProbAllStates[colNow] / fixProbAllStates[rowNow];
        }
        
        
    }
        
    
}




