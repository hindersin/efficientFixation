# efficientFixation
This code contains the algorithm described in <a href="http://www.sciencedirect.com/science/article/pii/S0303264716301885"><b>Exact numerical calculation of fixation probability and time on graphs.</b></a> by <i>Laura Hindersin, Marius Möller, Arne Traulsen</i> and <i>Benedikt Bauer</i>.


##Dependencies
<i>igraph</i> and <i>eigen3</i> have to be installed in order to use our software. They can be downloaded <a href="http://igraph.org/c/#downloads">here</a> and <a href=http://eigen.tuxfamily.org/index.php?title=Main_Page#Download>here</a>.
(Under Linux, you can use apt get to install. Under OS X, you can use MacPorts.)
The GNU <i>make</i> tool is useful for compiling our code.

##Before compiling
Edit the makefile (starting in line 29) to account for the location of your igraph and eigen Include Paths and Library Paths. 
For igraph, locate the libabry file libigraph.dylib and add its path (for example /usr/local/lib or /opt/local/lib) to line 33 of the makefile. 
##Compiling
In the terminal, go to the folder efficientFixation and run the following commands:
<table>
<tr><td>make clean</td></tr>
<tr><td>make</td></tr>
</table>
##Usage of the software
You can run the code by using the command ./release/fixating.out, followed by the input parameters<br>
<b>Update Rule</b> (Bd or dB),<br>
<b>Population Size</b> (integer between 2 and 23),<br>
<b>Direction</b> (undirected or directed),<br>
<b>Fitness of the mutants</b> (any float larger than 0.0),<br>
<b>Category</b> ('ER' for Erdos-Reny, 'BB' for Barabasi-Albert, 'WS' for Watts-Strogatz, 'geo' for geometric, 'complete' for well-mixed, or 'custom' for a custom graph),<br>
<b>Argument 1</b> ('GNM' or 'GNP' for Erdos Reny, double power of preference for Barabasi, int dimension for small world, bool periodic for geometric, '0' for complete, adjacency matrix as a string of numbers for custom),<br>
<b>Argument 2</b> (probability for every edge in Erdos-Reny GNP and geometric, number of edges for Erdos-Reny GNM, m for Barabasi, probability of rewiring for small world, 0 for complete and custom),<br>
<b>Output</b> (probability, unconditional, conditional, or all).<br>
<br>
We have also provided a config file in the release directory named configbash.sh, in which one can define all input parameters. The config file can then be run by ./configbash.sh

An example of the config file (configbash.sh):
<table>
<tr><td>Update="Bd"</td></tr>
<tr><td>popSize="4"</td></tr>
<tr><td>direction="undirected"</td></tr>
<tr><td>fitness="2.0"</td></tr>
<tr><td>category="ER"</td></tr>
<tr><td>arg1="GNP"</td></tr>
<tr><td>arg2="0.5"</td></tr>
<tr><td>output="unconditional"</td></tr>
<tr><td> &nbsp; </td></tr>
<tr><td>./fixating $Update $popSize $direction $fitness $category $arg1 $arg2 $output</td></tr>
</table>

#License
See <i>LICENSE</i> for details.
