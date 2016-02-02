#! /bin/bash

Update="Bd"			# Update Rule --- 'Bd' for birth in first step according to fitness, death in second step random choice || 'dB' accordingly the other way round
popSize="4"			# Population Size
direction="undirected"		# directedness of the graph --- possible values: 'undirected' or 'directed'
fitness="2.0" 			# fitness of the mutants
category="ER"			# category of the graph: 'ER' for Erdos-Reny, 'BB' for Barabasi-Albert, 'WS' for Watts-Strogatz, 'geo' for geometric or 'custom' for a custom graph 
arg1="GNP"			# secondary parameter for the category of graph: 'GNM' or 'GNP' for Erdos Reny, double power of preference for Barabasi, int dimension for small world, bool periodic for geometric, adjacency matrix as a string of numbers for custom
arg2="0.5"			# tertiary parameter for the category of graph: probability for every edge in Erdos-Reny GNP and geometric, number of edges for Erdos-Reny GNM, m for Barabasi, probability of rewiring for small world, 0 for custom
output="unconditional"  		#possible values: 'probability', 'conditional', 'unconditional' and 'all'

### Do NOT edit this line!!! ###
./fixating $Update $popSize $direction $fitness $category $arg1 $arg2 $output
