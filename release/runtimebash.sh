#! /bin/bash

for i in {4..23}
do
	for j in {1..10}
	do
		./fixating probability Bd $i undirected 1.0 >> runtimeRandomp.txt
	done
done
