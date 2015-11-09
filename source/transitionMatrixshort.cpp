#include "transitionMatrixshort.h"


void createTransitionMatrix(const unsigned int popSize, const unsigned int numStates, float fitnessMutants, std::string update, const int * const admat, t_vectorFP& data, t_vectorInt& row, t_vectorInt& col)
{
    
    // iterates over all states; i is both the iterator and the binary representation of the state, i.e. state 0001 is the state with a mutant at vertex 0 and wild-types at vertices 1,2, and 3.
    for(unsigned int i = 0 ; i < numStates ; i++)
    {
        float sumRow = 0.0f ;
        unsigned int numMutants = 0 ;
        std::vector<unsigned int> mutants; 
        std::vector<unsigned int> wilds;
        // calculates placement and number of both mutants and wild-types
        for(unsigned int j = 0; j < popSize ; j++)
        {
            unsigned int h = 1 << j; // meaning h = 2^j (bit shift to the left)
            if((h & i) >> j == 1)
            {
                numMutants += 1;
                mutants.push_back(j);
            }
            else
            {
                wilds.push_back(j);
            }
        }
        unsigned int numWilds = popSize - numMutants;   
        // iterates over all states that are adjacent to state i (these are states i+ 2^j or i - 2^j, depending on whether there is a mutant at j)
        for(unsigned int j = 0; j < popSize ; j++)
        {
			
            unsigned int h = 1 << j;
            
			if (update == "Bd")
            {
            	if ((h & i) >> j == 1) // decrease the number of mutants
            	{
					float prob = 0.0f;
                    for(unsigned int k = 0 ; k < numWilds ; k++)
                    {   
                        float numNeighbors = 0.0f;
                        // calculates the number of neighbors (successors) for a given wildtype
                        for(unsigned int t = 0 ; t < popSize ; t++)
                        {
                            numNeighbors = numNeighbors + admat[wilds[k] * popSize + t];
                            
                        }
                        
                        prob += ((float)admat[wilds[k] * popSize + j])/((float)numNeighbors);
                        
                    }  
                    // calculates probability of wildtype k propagating to position i-h              
                    prob = prob / (fitnessMutants * numMutants + numWilds);
                  
                    data.push_back(prob);
                    row.push_back(i);
                    col.push_back(i-h);
                    //data[(i-1) * popSize + j] = prob;
                    //row[(i-1) * popSize + j] = i;
                    //col[(i-1) * popSize + j] = i-h;
                    //std::cout << data[(i-1) * popSize + j] << ":" <<  (i-1) * popSize + j<< std::endl;
                        
                        
            	}
            	else // increase the number of mutants
            	{
                    float prob = 0.0f ;
                    for(unsigned int k = 0; k < numMutants; k++)
                    {
                        float numNeighbors = 0.0f ;
                        // calculates the number of neighbors for a given mutant
                        for(unsigned int t = 0 ; t < popSize ; t++)
                        {
                            numNeighbors = numNeighbors + admat[mutants[k] * popSize + t];
                            
                        }
                        prob += ((float)admat[mutants[k] * popSize + j])/((float)numNeighbors);
                    }
                    // calculates probability of mutant k propagating to position i+h
                    prob = prob * fitnessMutants / (fitnessMutants * numMutants + numWilds);
                                        
                    data.push_back(prob);
                    row.push_back(i);
                    col.push_back(i+h);                        
                    //data[(i-1) * popSize + j] = prob;
                    //row[(i-1) * popSize + j] = i;
                    //col[(i-1) * popSize + j] = i+h;
                    //std::cout << data[(i-1) * popSize + j] << ":" <<  (i-1) * popSize + j << std::endl;
              
            	}
           }
           
		   else if(update == "dB")
           {
			   std::vector<unsigned int> neighbors;
               if ((h & i) >> j == 1){ // mutant will be replaced by wildtype
				   
	               float numNeighbors = 0.0f;
	               float numMutantNeighbors = 0.0f;
	               for(unsigned int k = 0 ; k < popSize ; k++)
	               {
	                   numNeighbors += admat[k * popSize + j];
					   if (admat[k * popSize + j] == 1){
						   neighbors.push_back(k);
					   }
	               }

	               for(unsigned int n = 0 ; n < numNeighbors ; n++)
	               {
					   unsigned int positionNeighbor = neighbors[n];
					   unsigned int t = 1 << positionNeighbor;
					   if ((i & t) >> positionNeighbor == 1){ // there is a mutant at this position
						   numMutantNeighbors += 1;
					   }
	                   
	               }

	               float prob = 1/ (float)popSize * (numNeighbors - numMutantNeighbors) / (fitnessMutants * (numMutantNeighbors) + numNeighbors - numMutantNeighbors);

	               data.push_back(prob);
	               row.push_back(i);
	               col.push_back(i-h);
                       
                }
                else{  // wildtype will be replaced by mutant
					
					
					unsigned int numNeighbors = 0;
                    unsigned int numMutantNeighbors = 0;
                    for(unsigned int k = 0 ; k < popSize ; k++)
                    {
                       numNeighbors += admat[k * popSize + j];
 					   if (admat[k * popSize + j] == 1){
 						   neighbors.push_back(k);
 					   }
                    }
					
 	               for(unsigned int n = 0 ; n < numNeighbors ; n++)
 	               {
 					   unsigned int positionNeighbor = neighbors[n];
 					   unsigned int t = 1 << positionNeighbor;
 					   if ((i & t) >> positionNeighbor == 1){ // there is a mutant at this position
 						   numMutantNeighbors += 1;
 					   }
	                   
 	               }
                    
					float prob = 1/ (float)popSize * (fitnessMutants * numMutantNeighbors) / (fitnessMutants * numMutantNeighbors +numNeighbors - numMutantNeighbors);
                   
           
                    data.push_back(prob);
                    row.push_back(i);
                    col.push_back(i+h);
           	 	}
			}
            sumRow += data.back();
         }
         data.push_back(1-sumRow);
         row.push_back(i);
         col.push_back(i);
    }
}


