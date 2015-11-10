# efficientFixation
This code contains the algorithm described in <a href="http://arxiv.org/abs/1511.02696"><b>Exact numerical calculation of fixation probability and time on graphs.</b></a> by <i>Laura Hindersin, Marius Möller, Arne Traulsen</i> and <i>Benedikt Bauer</i>.


##Dependencies
<i>igraph</i> and <i>eigen3</i> have to be installed in order to use our software. They can be downloaded <a href="http://igraph.org/c/#downloads">here</a> and <a href=http://eigen.tuxfamily.org/index.php?title=Main_Page#Download>here</a>.
(Under Linux, you can use apt get to install. Under OS X, you can use MacPorts.)
The GNU <i>make</i> tool is useful for compiling our code.

##Usage
###Before compiling
Edit the makefile (starting in line 29) to account for the location of your igraph and eigen Include Paths and Library Paths. 
For igraph, locate the libabry file libigraph.dylib and add its path (for example /usr/local/lib or /opt/local/lib) to line 33 of the makefile. 
###Compiling
In the terminal, go to the folder efficientFixation and run the following commands:
<table>
<tr><td>make clean</td></tr>
<tr><td>make</td></tr>
</table>
###Running the code
You can run the code with the command ./release/fixating, followed by the input parameters <b>Quantity</b> (probability, unconditional or conditional), <b>Update Rule</b> (Bd or dB), <b>Population size</b> (integer between 2 and 23), <b>Direction</b> (undirected or directed), <b>Fitness of the mutants</b> (any float larger than 0.0).

A usage example:
<table>
<tr><td>./release/fixating probability Bd 10 directed 2.0</td></tr>
</table>

###Manipulating the graph structure
Inside <i>main.cpp</i> you can choose between different code snippets for either random graph, complete graph or you can manually provide an edge list. This is quite straight forward and there is more documentation inside of <i>main.cpp</i>.

#License
See <i>LICENSE</i> for details.
