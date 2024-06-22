/*
    Ali Raza
    20I-0782
    PDC-Summer-2023
    Project - MPI Program
*/

//=========================================================
//Headers
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define NUM_OF_CITIES 4                         // Number of cities
#define MAX_INT 100000                          // Maximum integer for reduction

//=========================================================
//Global values

char cities[NUM_OF_CITIES] = {'A','B','C','D'}; // Names of cities

//Adjacency matrix
int distance[NUM_OF_CITIES][NUM_OF_CITIES] = {
    { 0, 10, 15, 20 },
    { 10, 0, 35, 25 },
    { 15, 35, 0, 30 },
    { 20, 25, 30, 0 }
};

char source = 'D';      // Starting city for TSP

//=========================================================
// Function to calculate factorial of a number
int factorial(int n) 
{
    if (n <= 1) 
    {
        return 1;
    }

    return n * factorial(n - 1);
}

//=========================================================
// Function to get the index of a city in the cities array
int getCityIndex(char chr) 
{
    for(int i = 0; i < NUM_OF_CITIES; i++) 
    {
        if(cities[i] == chr) 
        {
            return i;
        }
    }

    return -1;
}

//=========================================================
// Function to print the path of the TSP solution
void print_path(int* path, int length) 
{
    printf("Best Path: %c -> ", source);

    for (int i = 0; i < length; i++) 
    {
        printf("%c", cities[path[i]]);

        if (i < length - 1) 
        {
            printf(" -> ");
        }
    }

    printf(" -> %c\n", source);
}

//=========================================================
// Function to calculate the cost of a given path
int calculate_cost(int* path, int start_city) 
{
    int cost = 0;
    int current_city = start_city;

    for (int i = 0; i < NUM_OF_CITIES - 1; i++) 
    {
        cost += distance[current_city][path[i]];
        current_city = path[i];                     // Update current_city to the next city
    }

    cost += distance[current_city][start_city];     // Add return to the starting city

    return cost;
}

//=========================================================
// Function to generate permutations of cities for TSP
void generate_permutation(int* arr, int n, int index, int* visited, int* best_path, int* min_cost, int start_city) 
{
    if (index == n) 
    {
        int cost = calculate_cost(arr, start_city);

        if (cost < *min_cost) 
        {
            *min_cost = cost;

            for (int i = 0; i < n; i++) 
            {
                best_path[i] = arr[i];
            }
        }

        return;
    }

    for (int i = 0; i < n; i++) 
    {
        if (!visited[i]) 
        {
            visited[i] = 1;
            arr[index] = i;
            generate_permutation(arr, n, index + 1, visited, best_path, min_cost, start_city);
            visited[i] = 0;
        }
    }
}

//=========================================================
//Driver function
int main(int argc, char** argv) 
{

    //Variables for calculating execution time
	clock_t Parallel_start, Parallel_end;
	double Parallel_exec_time;

    int rank, size;
    int perm_count;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        Parallel_start = clock();		//Store start time of parallel section 
    }

    // Calculate the number of permutations
    perm_count = factorial(NUM_OF_CITIES - 1);

    // Calculate chunk size for each process
    int chunk_size = perm_count / size;
    int start = rank * chunk_size;
    int end;

    // Determine the end point of permutations for each process
    if (rank == size - 1) 
    {
        end = perm_count;
    } 
    else 
    {
        end = start + chunk_size;
    }

    // Initialize variables for minimum cost and best path
    int min_cost = MAX_INT;
    int best_path[NUM_OF_CITIES-1];

    // Initialize array to track visited cities
    int* visited = (int*)malloc(NUM_OF_CITIES * sizeof(int));

    for (int i = 0; i < NUM_OF_CITIES; i++) 
    {
        visited[i] = 0;
    }

    // Initialize array for permutations
    int arr[NUM_OF_CITIES - 1];
    int start_city_index = getCityIndex(source);
    generate_permutation(arr, NUM_OF_CITIES - 1, 0, visited, best_path, &min_cost, start_city_index);

    // Perform reduction to find global minimum cost
    int global_min_cost;
    MPI_Reduce(&min_cost, &global_min_cost, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    // Initialize array to gather global best path
    int* global_best_path;

    if (rank == 0) 
    {
        global_best_path = (int*)malloc((NUM_OF_CITIES - 1) * sizeof(int));
    }

    // Gather best paths from all processes
    MPI_Gather(best_path, NUM_OF_CITIES - 1, MPI_INT, global_best_path, NUM_OF_CITIES - 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Print results from root process
    if (rank == 0) 
    {
        printf("Minimum TSP cost: %d\n", global_min_cost);
        print_path(global_best_path, NUM_OF_CITIES - 1);
        free(global_best_path);
    }

    // Free dynamically allocated memory and finalize MPI
    free(visited);

    MPI_Barrier( MPI_COMM_WORLD);
    MPI_Finalize();

    if(rank == 0)
    {
        Parallel_end = clock();																//Store stop time of parallel section
	    Parallel_exec_time = (((double)(Parallel_end - Parallel_start))/CLOCKS_PER_SEC);	//Calculate execution time
        printf("PARALLEL EXECUTION TIME: %f seconds\n", Parallel_exec_time);
    }
    

    return 0;
}
