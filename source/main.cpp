#include "main.h"
#include <iomanip>

int main(int argc, char* argv[])
{
    if (argc != 6)
    {
        cout << "Usage: ./release/fixating <Quantity: \"probability\", \"unconditional\" time, or \"conditional\" time> <Update Rule: \"Bd\", \"dB\"> <integer: population size> <\"directed\" or \"undirected\"> <double: fitness of mutant>" << endl;
        return -1;
    }

    //   ---------- If you want to stop time, uncomment all comments with //CLOCK//
    //CLOCK//
    std::clock_t start;
    //CLOCK//
    double bt = 0;
    //CLOCK//
    double st = 0;
    
    const unsigned int popSize = atoi(argv[3]);
    if (popSize > 23)
    {
        cout << "Code only possible for population size up to 23... aborting..." << endl;
        return -1;
    }
    const unsigned int numStates = 1 << popSize;
    
    string update = argv[2];
    if (update != "dB" && update != "Bd")
    {
        cout << "Only \"Bd\" or \"dB\" possible for update rule!... aborting..." << endl;
        return -1;
    }
    
    float fitnessMutants = atof(argv[5]);
    string direction = argv[4];
    string quantity = argv[1];

    igraph_t graph;
    



    // ----------   Code snippet for fully connected graph   ----------
/*    if (direction == "undirected")
    {
        igraph_full(&graph, popSize, false, false);
    }
    else if (direction == "directed")
    {
        igraph_full(&graph, popSize, true, false);
    }
    else
    {
      cout << "Only \"directed\" and \"undirected\" possible for direction of graph!... aborting..." << endl;
      return -1;
    }
*/    




    // ----------   Code snippet for random graph   ----------

      igraph_bool_t isConnected = 0;
      if (direction == "directed")
      {
          while (isConnected == 0)
          {
              igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP,
			    popSize, 0.1,
			    true, false);
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
          }
      }
      else if (direction == "undirected")
      {
          while (isConnected == 0)
          {
              igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP,
			    popSize, 0.1,
			    false, false);
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
          }
      }
      else
      {
          cout << "Only \"directed\" and \"undirected\" possible for direction of graph!... aborting..." << endl;
          return -1;
      }
    


    // ----------   Code snippet for graph with manual edge list   ---------- 
    igraph_vector_t v;
    /* Make an edge list like this example for a bi-directional line with 18 nodes:
     igraph_real_t edges[] = {0,1, 1,2, 2,3, 3,4, 4,5, 5,6, 6,7, 7,8, 8,9, 9,10, 10,11, 11,12, 12,13, 13,14, 14,15, 15,16, 16,17};
     For directed graphs, in the edge list you have to distinguish between 0,1 (an edge originating in vertex 0, pointing to vertex 1) and 1,0 (the other way around).
     */
    //
/*
    igraph_real_t edges[] = {0,1, 1,0, 1,2, 2,1, 1,3, 3,1, 2,3, 3,2};
    igraph_vector_view(&v, edges, sizeof(edges)/sizeof(double));
    if (direction == "directed") 
    igraph_create(&graph, &v, 0, IGRAPH_DIRECTED);
    else if (direction == "undirected")
    igraph_create(&graph, &v, 0, IGRAPH_UNDIRECTED);
    else
    {
        cout << "Only \"directed\" and \"undirected\" possible for direction of graph!" << endl;
        return -1;
    }
    
    igraph_bool_t isConnected = 0;
    igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
    
    if (isConnected == 0) {
        cout<< "Graph has to be strongly connected...aborting..."<<endl;
        return -1;
    }
*/


    // ----------   Here the adjacency matrix gets copied into an array  ----------
    
    
    igraph_matrix_t admatv;
    igraph_matrix_init(&admatv, 0,0);
    igraph_get_adjacency( &graph, &admatv,IGRAPH_GET_ADJACENCY_BOTH,false);
    int admat[popSize * popSize];
    for(unsigned int i = 0 ; i < popSize ; i++)
    {
        for(unsigned int k = 0 ; k < popSize ; k++)
        {
            admat[ i*popSize + k] = MATRIX(admatv,i,k );
        }
    }
    
    igraph_destroy(&graph);
    igraph_matrix_destroy(&admatv);
    
    for (int i=0; i<popSize; i++) {
        
        for (int j=0; j<popSize; j++) {
            // If you want to print the adjacency matrix:
            //cout<<admat[i * popSize + j]<<" ";
        }
        //cout<<endl;
    }

    t_vectorFP data;
    t_vectorInt row;    
    t_vectorInt col;
    data.reserve(popSize * numStates);
    row.reserve(popSize * numStates);
    col.reserve(popSize * numStates);
    
    //CLOCK//
    start = std::clock();
    createTransitionMatrix(popSize, numStates, fitnessMutants, update, admat, data, row, col);
    
    
    std::vector<T> tripletList;
    tripletList.reserve(popSize * numStates);

    for( unsigned int j = 0 ; j < data.size() ; j++)
    {
        tripletList.push_back(T(col.at(j),row.at(j),data.at(j)));
    }

    SpMat mat(numStates,numStates);
    mat.setFromTriplets(tripletList.begin(), tripletList.end());

    // Stopping time after creating transition matrix
    //CLOCK//
    bt = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    //for (int i = 0; i<data.size(); i++)
    //    cout<<"unconditional: transition prob from state "<<row[i]<<" to state "<<col[i]<<" is "<<data[i]<<endl;
    string s1;
    


    /*   ----------   Distinguishing between "probability", "unconditional" time, and "conditional" time   ----------    */

    if (quantity == "probability")
    {
        float * fixProbAllStates = (float*) malloc(numStates * sizeof(float));
        fixProb(mat, popSize, numStates, fixProbAllStates);

        // Stopping time after solving fixation probabilities
        //CLOCK//
        st = ( std::clock() - start) / (double) CLOCKS_PER_SEC - bt;
    
        float probOne = 0.0;
        for(unsigned int i = 0; i < popSize; i++)
        {
            int j = 1 << i;
    
            probOne = probOne + fixProbAllStates[j];
            
        }
        probOne = probOne / (float)(popSize);

        cout << "Average fixation probability starting with one randomly placed mutant: " << probOne << endl;
        
        /*   ----------   Printing the fixation probability starting from all states   ----------    */
        
        for(unsigned int i = 0; i < numStates; i++)
        {
            bitset<23> b1(i);
            s1 =  b1.to_string(); 
            cout<<"fixation probability in state ";
            cout<< s1.substr(23-popSize,popSize);
            cout <<" is "<<fixProbAllStates[i]<<endl;
        }
    }
    
    else if (quantity == "unconditional")
    {
    
        float * uncondFixTimeAllStates = (float*) malloc(numStates * sizeof(float));
        // Stopping the time for solving for unconditional fixation time
        //CLOCK// start = std::clock();
        //CLOCK// bt = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        time(mat, popSize, numStates, uncondFixTimeAllStates);
        //CLOCK//
        st = ( std::clock() - start) / (double) CLOCKS_PER_SEC - bt;
     
        float avUncondTime = 0.0;
        for(unsigned int i = 0 ; i < popSize ; i++)
        {
            int j = 1 << i;
            avUncondTime = avUncondTime + uncondFixTimeAllStates[j];
        }
        avUncondTime = avUncondTime / (float)(popSize);

        cout << "Average unconditional fixation time starting with one randomly placed mutant: " << avUncondTime << endl;
        
        /*   ----------   Printing the average unconditional fixation time starting from all states   ----------    */

        //for(unsigned int i = 0; i < numStates; i++)
        //{
        //    bitset<23> b1(i);
        //    s1 =  b1.to_string();
            //cout<<"Unconditional fixation time in state ";
            //cout<< s1.substr (23-popSize,popSize);
            //cout <<" is "<<uncondFixTimeAllStates[i]<<endl;
        //}
    }
    
    else if (quantity == "conditional")
    {
        float * fixProbAllStates = (float*) malloc(numStates * sizeof(float));
        fixProb(mat, popSize, numStates, fixProbAllStates);
        createConditionalTransitionMatrix(popSize, numStates, fixProbAllStates, data, row, col);
        
        std::vector<T> tripletListCond;
        tripletListCond.reserve(popSize * numStates);
        
        for( unsigned int j = 0 ; j < data.size() ; j++)
        {
            tripletListCond.push_back(T(col.at(j),row.at(j),data.at(j)));
        }
        
        SpMat conditionalMatrix(numStates,numStates);
        conditionalMatrix.setFromTriplets(tripletListCond.begin(), tripletListCond.end());
    
        
        float * condFixTimeAllStates = (float*) malloc(numStates * sizeof(float));
        time(conditionalMatrix, popSize, numStates, condFixTimeAllStates);
        
        
        float avCondTime = 0.0;
        for(unsigned int i = 0 ; i < popSize ; i++)
        {
            int j = 1 << i;
            avCondTime = avCondTime + condFixTimeAllStates[j];
        }
        avCondTime = avCondTime / (float)(popSize);

        cout << "Average conditional fixation time starting with one randomly placed mutant: " << avCondTime << endl;

        /*   ----------   Printing the average conditional fixation time starting from all states   ----------    */
        
        //for(unsigned int i = 0; i < numStates; i++)
        //{
            //bitset<23> b1(i);
            //s1 =  b1.to_string();
            //cout<<"Conditional fixation time in state ";
            //cout<< s1.substr (23-popSize,popSize);
            //cout <<" is "<<condFixTimeAllStates[i]<<endl;
        //}
    }
    else
    {
        cout << "Only \"probability\", \"unconditional\", and \"conditional\" as arguments possible for the quantity!"<<endl;
        return -1;
    }

    //CLOCK//
    cout<<"building time: "<< bt <<'\n';
    //CLOCK//
    cout<<"solving time: "<< st << "\n\n";

}


