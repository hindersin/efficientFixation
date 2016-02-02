#include "main.h"
#include <iomanip>

#define maxcount 10

int main(int argc, char* argv[])
{
    if (argc != 9)
    {   
        cout << "Usage: ./release/fixating <Update Rule: \"Bd\", \"dB\"> <integer: population size> <\"directed\" or \"undirected\"> <double: fitness of mutant> <category of graph: \"complete\", \"ER\", \"BB\", \"WS\", \"geo\", or \"custom\" > <secondary parameter for the category of graph: \"GNM\" or \"GNP\" for Erdos Reny, double power of preference for Barabasi, int dimension for small world, bool periodic for geometric, , adjacency matrix for custom> <tertiary parameter for the category of graph: probability for every edge in Erdos-Reny GNP and geometric, number of edges for Erdos-Reny GNM, m for barabasi, probability of rewiring for small world, 0 for custom> <output: \"probability\", \"conditional\", \"unconditional\", or \"all\">" << endl;
        return -1; 
    }
    //   ---------- If you want to stop time, uncomment all comments with //CLOCK//
    //CLOCK//
    std::clock_t start;
    //CLOCK//
    double bt = 0;
    //CLOCK//
    double st = 0;
 
    const unsigned int popSize = atoi(argv[2]);
    if (popSize > 23)
    {
        cout << "Code only possible for population size up to 23... aborting..." << endl;
        return -1;
    }
    const unsigned int numStates = 1 << popSize;
    
    string update = argv[1];
    if (update != "dB" && update != "Bd")
    {
        cout << "Only \"Bd\" or \"dB\" possible for update rule!... aborting..." << endl;
        return -1;
    }
    
    float fitnessMutants = atof(argv[4]);
    string direction = argv[3];
    string category = argv[5];
    igraph_t graph;
    int admat[popSize * popSize];

    string output = argv[8];
    if (output != "probability" && output != "conditional" && output != "unconditional" && output != "all")
    {
        cout << "Only \"probability\", \"unconditional\", \"conditional\" or \"all\" possible for output!... aborting..." << endl;
        return -1;
    }



    // ----------   Code snippet for fully connected graph   ----------
    if (category == "complete")
    {
      if (direction == "undirected")
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
    }


    // ----------   Code snippet for random graph   ----------
    else if (category == "ER")
    {
      string gn = argv[6];   

      igraph_rng_seed(igraph_rng_default(), std::clock());
      igraph_bool_t isConnected = 0;
       
      if (direction == "directed")
      {
	  //const int maxcount = 10;
          int counts = 0;
          while ((isConnected == 0) & (counts < maxcount))
          {
              if (gn == "GNP")
              {   
		      double edgeprob = atof(argv[7]);
		      if ((edgeprob > 1) || (edgeprob < 0))
		      {
			cout << "probabilities over 1 or smaller 0 ...aborting..." << endl;
			return -1;
		      }
		      igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP,
				    popSize, edgeprob,
				    true, false);
              }
              else if (gn == "GNM")
              {
		      int edgenumber = atoi(argv[7]);
                      if ((edgenumber < 1) || (edgenumber > popSize*(popSize-1)))
                      {
                        cout << "number of edges must be greater than 1 and smaller than N*(N-1) ...aborting..." << endl;
			return -1;
                      }
                      
		      igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNM,
				    popSize, edgenumber,
				    true, false);
              }
              else
              { 
                 cout << "Only \"GNM\" and \"GNP\" possible ... aborting..." << endl;
              }
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
              
              counts++;
          }
          if (counts == maxcount)
          {
            cout << "Probability or number of edges too low... Did not find a connected graph after "<< maxcount <<" attemps... aborting..." << endl;
            return -1;
          }
      }
      else if (direction == "undirected")
      {
          int counts = 0;
          while ((isConnected == 0) & (counts < maxcount))
          {
              if (gn == "GNP")
              {   
		      double edgeprob = atof(argv[7]);
		      if ((edgeprob > 1) || (edgeprob < 0))
		      {
			cout << "probabilities over 1 or smaller 0 ...aborting..." << endl;
			return -1;
		      }
		      igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP,
				    popSize, edgeprob,
				    false, false);
              }
              else if (gn == "GNM")
              {
		      int edgenumber = atoi(argv[7]);
                      if ((edgenumber < 1) || (edgenumber > popSize*(popSize-1)/2))
                      {
                        cout << "number of edges must be greater than 1 and smaller than N*(N-1)/2 ...aborting..." << endl;
			return -1;
                      }
		      igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNM,
				    popSize, edgenumber,
				    false, false);
              }
              else
              { 
                 cout << "Only \"GNM\" and \"GNP\" possible ... aborting..." << endl;
              }
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
              counts++;
          }
          if (counts == maxcount)
          {
            cout << "Probability or number of edges too low... Did not find a connected graph after "<< maxcount <<" attemps... aborting..." << endl;
            return -1;
          }
      }      
      else
      {
          cout << "Only \"directed\" and \"undirected\" possible for direction of graph!... aborting..." << endl;
          return -1;
      }
    }

//---------------------------- Code snippet for small world network --------------------------------//
     else if (category == "WS")  
     {
      igraph_rng_seed(igraph_rng_default(), std::clock());
      igraph_bool_t isConnected = 0;

      double edgeprob = atof(argv[7]);
      if ((edgeprob > 1) || (edgeprob < 0))
      {
        cout << "probabilities over 1 or smaller 0 ...aborting..." << endl;
        return -1;
      }
      
      int dim = atoi(argv[6]);  
      int latSize = pow(popSize,1/double(dim));   
 
      if (direction == "directed")
      {
          while (isConnected == 0)
          {
              igraph_watts_strogatz_game(&graph, dim,
			                 latSize, 1,
			                 edgeprob, 0, 
			                 0);
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
          }
      }
      else if (direction == "undirected")
      {
          while (isConnected == 0)
          {
              igraph_watts_strogatz_game(&graph, dim,
			                 latSize, 1,
			                 edgeprob, 0, 
			                 0);
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
          }
      }
      else
      {
          cout << "Only \"directed\" and \"undirected\" possible for direction of graph!... aborting..." << endl;
          return -1;
      }    
     }

//---------------------------- Code snippet for geometric generator --------------------------------//
     else if(category == "geo")
     {
      igraph_rng_seed(igraph_rng_default(), std::clock());
      igraph_bool_t isConnected = 0;
      double edgeprob = atof(argv[7]);
      if ((edgeprob > 1) || (edgeprob < 0))
      {
        cout << "probabilities over 1 or smaller 0 ...aborting..." << endl;
        return -1;
      }
      bool torus = (atoi(argv[6]) == 1);
      double radius = sqrt(edgeprob/3.14);
      if (direction == "directed")
      {
          while (isConnected == 0)
          {
              igraph_grg_game(&graph, popSize,
		              radius, torus,
		              0, 0);
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
          }
      }
      else if (direction == "undirected")
      {
          while (isConnected == 0)
          {
              igraph_grg_game(&graph, popSize,
		              radius, torus,
		              0, 0);
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
          }
      }
      else
      {
          cout << "Only \"directed\" and \"undirected\" possible for direction of graph!... aborting..." << endl;
          return -1;
      }
     }
//---------------------------- Code snippet for barabasi generator --------------------------------//
     else if(category == "BB")
     {
      double power = atof(argv[6]);
      int m = atoi(argv[7]);
      
      igraph_rng_seed(igraph_rng_default(), std::clock());
      igraph_bool_t isConnected = 0;
      if (direction == "directed")
      {
          while (isConnected == 0)
          {
              igraph_barabasi_game(&graph, popSize,
			 power, 
			 m,
			 0,
			 0,
			 1.0,
			 true,
			 IGRAPH_BARABASI_PSUMTREE,
			 0);
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
          }
      }
      else if (direction == "undirected")
      {
          while (isConnected == 0)
          {
              igraph_barabasi_game(&graph, popSize,
			 power, 
			 m,
			 0,
			 0,
			 1.0,
			 false,
			 IGRAPH_BARABASI_PSUMTREE,
			 0);
              igraph_is_connected(&graph, &isConnected, IGRAPH_STRONG);
          }
      }
      else
      {
          cout << "Only \"directed\" and \"undirected\" possible for direction of graph!... aborting..." << endl;
          return -1;
      }
     }
    // ----------   Code snippet for custom graph   ---------- 
     else if(category == "custom")
     {
     std::string admats = argv[6];
     std::vector<int> ints;
     std::transform(std::begin(admats), std::end(admats), std::back_inserter(ints),    
      [](char c) 
        {
          return c - '0';
        }
        );
     std::copy(ints.begin(), ints.end(), admat);
     }
     
     else
     {
          cout << "Only \"complete\", \"ER\", \"BB\", \"WS\", or \"geo\" as categories... aborting..." << endl;
          return -1;
     }


    // ----------   Here the adjacency matrix gets copied into an array  ----------
    
    if(category!="custom")
    {
    igraph_matrix_t admatv;
    igraph_matrix_init(&admatv, 0,0);
    igraph_get_adjacency( &graph, &admatv,IGRAPH_GET_ADJACENCY_BOTH,false);
    for(unsigned int i = 0 ; i < popSize ; i++)
    {
        for(unsigned int k = 0 ; k < popSize ; k++)
        {
            admat[ i*popSize + k] = MATRIX(admatv,i,k );
        }
    }
    
    igraph_destroy(&graph);
    igraph_matrix_destroy(&admatv);
    }
    for (unsigned int i=0; i<popSize; i++) {
        
        for (unsigned int j=0; j<popSize; j++) {
            // If you want to print the adjacency matrix:
            cout<<admat[i * popSize + j]<<" ";
        }
    }
    cout<<endl;
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
    


    /*   ----------   No distinguishing between "probability", "unconditional" time, and "conditional" time   ----------    */
   
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

        cout << "fixation probability:" << probOne << endl;   
        /*   ----------   Printing the fixation probability starting from all states   ----------    */
        /*
        for(unsigned int i = 0; i < numStates; i++)
        {
            bitset<23> b1(i);
            s1 =  b1.to_string(); 
            cout<<"fixation probability in state ";
            cout<< s1.substr(23-popSize,popSize);
            cout <<" is "<<fixProbAllStates[i]<<endl;
        }
        */
    if((output == "unconditional")||(output == "all"))
    {        
        float * uncondFixTimeAllStates = (float*) malloc(numStates * sizeof(float));
        // Stopping the time for solving for unconditional fixation time
        //CLOCK// start = std::clock();
        //CLOCK// bt = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        time(mat, popSize, numStates, uncondFixTimeAllStates);
        //CLOCK//
     
        float avUncondTime = 0.0;
        for(unsigned int i = 0 ; i < popSize ; i++)
        {
            int j = 1 << i;
            avUncondTime = avUncondTime + uncondFixTimeAllStates[j];
        }
        avUncondTime = avUncondTime / (float)(popSize);

        cout<< "unconditional fixation time:" << avUncondTime << endl;
    }    
        /*   ----------   Printing the average unconditional fixation time starting from all states   ----------    */

        //for(unsigned int i = 0; i < numStates; i++)
        //{
        //    bitset<23> b1(i);
        //    s1 =  b1.to_string();
            //cout<<"Unconditional fixation time in state ";
            //cout<< s1.substr (23-popSize,popSize);
            //cout <<" is "<<uncondFixTimeAllStates[i]<<endl;
        //}
 
        //float * fixProbAllStates = (float*) malloc(numStates * sizeof(float));
        //fixProb(mat, popSize, numStates, fixProbAllStates);
    if((output == "conditional")||(output == "all"))
    {  
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

        cout << "conditional fixation time:" << avCondTime << endl;
     }
        /*   ----------   Printing the average conditional fixation time starting from all states   ----------    */
        
        //for(unsigned int i = 0; i < numStates; i++)
        //{
            //bitset<23> b1(i);
            //s1 =  b1.to_string();
            //cout<<"Conditional fixation time in state ";
            //cout<< s1.substr (23-popSize,popSize);
            //cout <<" is "<<condFixTimeAllStates[i]<<endl;
        //}
    
    st = ( std::clock() - start) / (double) CLOCKS_PER_SEC - bt;
    //CLOCK//
    cout<<"building time: "<< bt <<'\n';
    //CLOCK//
    cout<<"solving time: "<< st << "\n\n";

}


