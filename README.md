This program uses classic graph algorithm - DFS.

The hardest operation for algorithm - redistribution of water after filling pools
Effective sollution: distribute just graph connectivity components
There's pointer in struct pool to see distrbuted subgraphs
If subgraph was redist - skip

Average programm time - 17-20 seconds 

Build script was done using bash-script

All functions for work with struct pool was declared in file struct.c

Programm was checked using Valgrind, there's no memory leaks

Best wishes, Andrew Tretyakov
