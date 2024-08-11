# Traveling Salesman Problem (TSP) Solver in Parallel Execution
This repository contains a parallel implementation of the Traveling Salesman Problem (TSP) solver using MPI (Message Passing Interface) for distributed computing. The solver finds the shortest possible tour that visits a set of cities and returns to the starting city.
### Files
* TSP.c: Parallel implementation of TSP solver using MPI.
* TSP_Serial.c: Serial implementation of TSP solver (in a separate file).
### Observations
The parallel version contains approximately 67% parallel code and 33% serial code.
Running the parallel version on 2 processors yields a speedup of 1.5038 and an efficiency of 75.19%.
However, due to the small problem size, the serial version is currently faster. A larger graph is expected to showcase the parallel version's performance advantage.
### Compilation and Execution
To compile and run the parallel version, use the following commands:

- mpicc parallel_tsp.c -o parallel_tsp
- mpirun -np 2 ./parallel_tsp

Replace 2 with the desired number of processors.

#### [Note: For larger graphs, the parallel version is expected to outperform the serial version. The serial version is provided in a separate file (serial_tsp.c) for comparison purposes.]