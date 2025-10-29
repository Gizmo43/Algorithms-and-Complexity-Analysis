#include "DHeap.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono> 
#include <random>

#define MIN 0
#define MAX 9999

class edge {
public:
    int vertex1;
    int vertex2;
    int weight;
    edge(int v1, int v2, int w) :vertex1(v1), vertex2(v2), weight(w) {};
};

std::vector<edge> test_graph{
    edge(0,1,7), edge(1,5,2), edge(1,2,5),
    edge(2,5,6), edge(2,3,11), edge(3,5,9),
    edge(3,4,8), edge(4,5,3), edge(4,0,4),
};

std::vector<int> LDGDijkstraMark(std::vector<edge> graph, int n, int start_vertex, int& time) {
    std::vector<int> h(n, 0);
    std::vector<int> dist(n, MAX);

    dist[start_vertex] = 0;
    int nq = n;


    auto start_time = std::chrono::high_resolution_clock::now();
    while (nq > 0) {
        int c = 0;
        while ((c < n) && (h[c] != 0)) {
            c++;
        };

        if (c > n) break;
        int i = c;
        for (int k = c; k < n; k++) {
            if ((h[k] == 0) && (dist[i] > dist[k])) {
                i = k;
            }
        }

        h[i] = 1;
        nq--;

        for (const auto& e : graph) {
            int j = -1;

            if (e.vertex1 == i) {
                j = e.vertex2;
            }
            else if (e.vertex2 == i) {
                j = e.vertex1;
            }
            if (j != -1 && j < n && h[j] == 0) {
                if (dist[j] > dist[i] + e.weight) {
                    dist[j] = dist[i] + e.weight;
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    time = duration.count();

    return dist;
};

std::vector<int> LDGDijkstra15Heap(std::vector<edge> graph, int n, int start_vertex, int& time) {
    int d = 15;
    std::vector<int> dist(n, MAX);
    std::vector<int> name(n);
    std::vector<int> key(n, MAX);
    std::vector<int> heap_index(n, -1);

    for (int i = 0; i < n; i++) {
        name[i] = i;
        key[i] = MAX;
    }

    key[start_vertex] = 0;


    DHeap<int> heap(d, name, key, heap_index, n);
    heap.buildQueue();

    auto start_time = std::chrono::high_resolution_clock::now();
    while (!heap.isEmpty()) {
        int name1, key1;
        heap.extractMinimum(name1, key1);
        int i = name1;

        if (key1 == MAX) {
            dist[i] = MAX;
            continue;
        }

        dist[i] = key1;

        for (const auto& e : graph) {
            int j = -1;

            if (e.vertex1 == i) {
                j = e.vertex2;
            }
            else if (e.vertex2 == i) {
                j = e.vertex1;
            }

            if (j != -1 && j < n) {
                if (heap.isInHeap(j)) {
                    int new_dist = dist[i] + e.weight;
                    if (new_dist < key[j]) {
                        heap.decreaseKey(j, new_dist);
                    }
                }
            }
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    time = duration.count();

    return dist;
}


std::vector<edge> generateRandomGraph(int n, int m, int min_weight = 1, int max_weight = 20) {
    std::vector<edge> graph;
    std::vector<std::pair<int, int>> all_possible_edges;


    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            all_possible_edges.push_back({ i, j });
        }
    }
    

    int max_possible_edges = all_possible_edges.size();
    if (m > max_possible_edges) {
        m = max_possible_edges;
        std::cout << "Warning: Requested " << m << " edges but maximum possible is " << max_possible_edges
            << ". Generating " << max_possible_edges << " edges.\n";
    }


    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(all_possible_edges.begin(), all_possible_edges.end(), g);
    std::uniform_int_distribution<int> weight_dist(min_weight, max_weight);


    for (int i = 0; i < m; i++) {
        int v1 = all_possible_edges[i].first;
        int v2 = all_possible_edges[i].second;
        int weight = weight_dist(g);
        graph.push_back(edge(v1, v2, weight));
    }

    return graph;
}

int main() {
    int n = 6;
    int start_vertex = 0;
    std::vector<int> dist;
    std::vector<edge> graph;
    int flag = -1;


    int choice = -1;
    std::cout << "Choose graph type:\n";
    std::cout << "1 - Use test graph\n";
    std::cout << "2 - Generate random graph\n";
    std::cin >> choice;


    switch (choice)
    {
    case 1:
        graph = test_graph;
        n = 6;        
        break;
    case 2:
        std::cout << "Enter number of vertices: ";
        std::cin >> n;
        int m, q, r;
        std::cout << "Enter number of edges: ";
        std::cin >> m;
        std::cout << "Enter min of weight: ";
        std::cin >> q;
        std::cout << "Enter max of weight: ";
        std::cin >> r;
        graph = generateRandomGraph(n, m, q, r);
        break;
    default:
        std::cout << "Invalid input" << "\n";
        break;
    }


    std::cout << "Start vertex: ";
    std::cin >> start_vertex;

   

    int time = 0;   
    std::cout << "Mark algorithm:" << "\n";
    dist = LDGDijkstraMark(graph, n, start_vertex, time);        
    for (int i = 0; i < n; i++) {
        std::cout << "Vertex: " << i << " Dist: = " << dist[i] << "\n";
    }
    std::cout << "Execution time: " << time << " microseconds\n";

    time = 0;
    std::cout << "15-Heap algorithm:" << "\n";
    dist = LDGDijkstra15Heap(graph, n, start_vertex, time);

    for (int i = 0; i < n; i++) {
        std::cout << "Vertex: " << i << " Dist: = " << dist[i] << "\n";
    }
    std::cout << "Execution time: " << time << " microseconds\n";


    

    return 0;
}