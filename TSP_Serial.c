

//=========================================================
//Headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_OF_CITIES 4             // Number of cities
#define MAX_INT 100000              // Maximum integer for reduction

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

char source = 'D'; // Starting city for TSP

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
        current_city = path[i];
    }

    cost += distance[current_city][start_city];

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
int main() 
{
    //Variables for calculating execution time
	clock_t Serial_start, Serial_end;
	double Serial_exec_time;

    int perm_count = factorial(NUM_OF_CITIES - 1);

    int min_cost = MAX_INT;
    int best_path[NUM_OF_CITIES-1];

    Serial_start = clock();

    // Initialize array to track visited cities
    int* visited = (int*)malloc(NUM_OF_CITIES * sizeof(int));

    for (int i = 0; i < NUM_OF_CITIES; i++) 
    {
        visited[i] = 0;
    }

    // Initialize array for permutations
    int arr[NUM_OF_CITIES - 1];

    // Find the index of the starting city
    int start_city_index = getCityIndex(source);

    // Generate permutations and find the best path
    generate_permutation(arr, NUM_OF_CITIES - 1, 0, visited, best_path, &min_cost, start_city_index);

    // Print the minimum TSP cost and the best path
    printf("Minimum TSP cost: %d\n", min_cost);
    print_path(best_path, NUM_OF_CITIES - 1);

    // Free dynamically allocated memory
    free(visited);

    Serial_end = clock();																//Store stop time of parallel section
    Serial_exec_time = (((double)(Serial_end - Serial_start))/CLOCKS_PER_SEC);	//Calculate execution time
    printf("SERIAL EXECUTION TIME: %f seconds\n", Serial_exec_time);

    return 0;
}
