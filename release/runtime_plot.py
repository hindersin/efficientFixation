import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
from matplotlib.backends.backend_pdf import PdfPages

f = open('runtimeRandom.txt','r')

lines = f.readlines()
numberOfGraphs = 20
popSizeMin = 4
popSizeMax = 23
popSizeRange = popSizeMax+1 - popSizeMin

runtimesBuilding = np.zeros(popSizeRange, dtype=float)
runtimesSolving = np.zeros(popSizeRange, dtype=float)
averagingArrayBuilding = np.zeros(numberOfGraphs,dtype=float)
averagingArraySolving = np.zeros(numberOfGraphs,dtype=float)

for h in range(popSizeRange):    
   for i in range(numberOfGraphs):
      buildingLine = lines[4*numberOfGraphs*h+4*i+1]
      averagingArrayBuilding[i] = float(buildingLine[15:25])
      solvingLine = lines[4*numberOfGraphs*h+4*i+2]
      averagingArraySolving[i] = float(solvingLine[14:25])
   runtimesBuilding[h] = sum(averagingArrayBuilding)/numberOfGraphs
   runtimesSolving[h] = sum(averagingArraySolving)/numberOfGraphs
    

runtimesTotal = runtimesBuilding + runtimesSolving


popSizes=range(popSizeMin, popSizeMax+1)

plot1 = plt.figure(1, figsize=(5.0,3.0))
plt.semilogy(popSizes, runtimesBuilding)
plt.semilogy(popSizes, runtimesSolving, 'r--')
plt.semilogy(popSizes, runtimesTotal, 'k:')
plt.xlim([popSizeMin,popSizeMax])
plt.xlabel("Population Size")
plt.ylabel("Time in Seconds")
plt.title("Running Times for Neutral Mutant")
plt.subplots_adjust(bottom=0.17, left=0.14)
pp = PdfPages('runtimeRandomPlot'+'.pdf')
pp.savefig(plot1)
pp.close()
plt.show()
