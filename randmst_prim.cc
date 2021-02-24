#include <cstdio>
#include <random>
#include <time.h>
#include <chrono>
#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <math.h>

#define MAX_N 262144


// Run prim's algorithm on n vertices, where the edges have random weights in [0, 1]

double dist[MAX_N]; // Distance from root vertex to v
double prev[MAX_N]; // Previous vertex in minimal spanning tree
int S[MAX_N]; // Store whether or not vertex v is in S

double pos[MAX_N][4]; // Store positions in square, cube, hypercube
int dim; // Dimension

double calcDist(int u, int v) {
    double total = 0;
    for (int i = 0; i < dim; i++) {
        double d = pos[u][i] - pos[v][i];
        total += d*d;
    }
    return sqrt(total);
}

double getRand() {
    return (double) rand()/RAND_MAX;
}

double prim(int n) {
    // Initialize point positions
    if (dim > 0) { 
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < dim; j++) {
                pos[i][j] = getRand();
            }
        }
    }

    // Loop over vertices
    for (int v = 0; v < n; v++) {
        dist[v] = MAX_N; // Infinity
        S[v] = 0; // S starts with no vertices
        prev[v] = -1; // -1 represents no previous vertex
    }
    dist[0] = 0; // Vertex 0 is the root vertex

    for (int i = 0; i < n; i++) { // Add n vertices to the MST
        // Deletemin by looping over dist
        int min_vertex = -1;
        double min_dist = 2; // All edges <= 1
        for (int j = 0; j < n; j++) {
            if (S[j] == 0 && dist[j] < min_dist) { // If distance < min_dist and j not in S
                min_vertex = j;
                min_dist = dist[j];
            }
        }
        assert(min_vertex != -1);
        S[min_vertex] = 1;
        // Loop over edges coming out of min_vertex, that land in V-S
        for (int w = 0; w < n; w++) { 
            if (S[w] == 0) { // If w not in S, update distance
                double edgeWeight = dim > 0 ? calcDist(min_vertex, w) : getRand(); // Length(v, w)
                if (dist[w] > edgeWeight) {
                    dist[w] = edgeWeight;
                    prev[w] = min_vertex;
                }
                // Don't need to insert into H bc we're using dist for H
            }
        }
    }
    double total_dist = 0;
    for (int i = 0; i < n; i++) {
        total_dist += dist[i];
    }
    return total_dist;
}

int main(int argc, char* argv[]) {
    srand (time(NULL));

    if (argc == 5) {
        // Input format: ./randmst 0 numpoints numtrials dimension
        // Output format: average numpoints numtrials dimension
        int flag = atoi(argv[1]);
        int numpoints = atoi(argv[2]);
        int numtrials = atoi(argv[3]);
        dim = atoi(argv[4]);
        double total_weight = 0;
        for (int i = 0; i < numtrials; i ++) {
            double weight = prim(numpoints);
            total_weight += weight;
        }
        double average_weight = total_weight/numtrials;
        printf("%f %d %d %d\n", average_weight, numpoints, numtrials, dim);

    }

    else if (argc == 2) { // Run stats
        dim = atoi(argv[1]);
        int n_values[12] = {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144};
        for (int n_value = 0; n_value < 12; n_value ++) {
            double total_weight = 0;
            int n = n_values[n_value];
            int num_iterations = 5;
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            for (int i = 0; i < num_iterations; i ++) {
                double weight = prim(n);
                total_weight += weight;
                
            }
            double average_weight = total_weight/num_iterations;
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            int total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            double t = (double)total_ms/1000/num_iterations;

            printf("%d, %d, %f, %f\n", n, num_iterations, average_weight, t);

        }
    }
}