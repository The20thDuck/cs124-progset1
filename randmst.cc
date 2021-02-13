#include <cstdio>
#include <random>
#include <time.h>
#include <chrono>
#include <iostream>
#include <assert.h>

#define MAX_N 262144

double getRand() {
    return (double) rand()/RAND_MAX;
}

// Run prim's algorithm on n vertices, where the edges have random weights in [0, 1]

double dist[MAX_N]; // Distance from root vertex to v
double prev[MAX_N]; // Previous vertex in minimal spanning tree
int S[MAX_N]; // Store whether or not vertex v is in S

double prim(int n) {
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
            if (S[w] == 0) {
                double edgeWeight = getRand(); // Length(v, w)
                if (dist[w] > edgeWeight) {
                    dist[w] = edgeWeight;
                    prev[w] = min_vertex;
                }
                // Insert(w, dist[w], H). Already done, bc we're using dist for H
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

    // double total_weight = 0;
    // int num_iterations = 100000;
    // for (int i = 0; i < num_iterations; i ++) {
    //     total_weight += prim(4);
    // }
    // double average_weight = total_weight/num_iterations;
    // printf("%f\n", average_weight);
    
    int n_values[12] = {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144};

    for (int n_value = 0; n_value < 12; n_value ++) {
        double total_weight = 0;
        int n = n_values[n_value];
        int num_iterations = 5;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        for (int i = 0; i < num_iterations; i ++) {
            total_weight += prim(n);
        }
        double average_weight = total_weight/num_iterations;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

        printf("Average weight for n = %d: %f\n", n, average_weight);

    }
}