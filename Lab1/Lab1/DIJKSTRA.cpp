#include "DHeap.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono> 

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

std::vector<int> LDGDijkstraMark(std::vector<edge> graph, int n, int start_vertex) {
    std::vector<int> h(n, 0);
    std::vector<int> dist(n, MAX);

    dist[start_vertex] = 0;
    int nq = n;

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

    return dist;
};

std::vector<int> LDGDijkstra15Heap(std::vector<edge> graph, int n, int start_vertex) {
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

    return dist;
}

int main() {
    int n = 6;
    int start_vertex = 0;
    std::vector<int> dist;
    int flag = -1;


    std::cout << "Start vertex: ";
    std::cin >> start_vertex;

    std::cout << "Chose the algorithm" << "\n"
        << "1 - Mark algorithm:" << "\n" << "2 - 15-Heap algorithm:" << "\n";
    std::cin >> flag;

    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    switch (flag)
    {
    case 1:
        start_time = std::chrono::high_resolution_clock::now();
        std::cout << "Mark algorithm:" << "\n";
        dist = LDGDijkstraMark(test_graph, n, start_vertex);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        for (int i = 0; i < n; i++) {
            std::cout << "Vertex: " << i << " Dist: = " << dist[i] << "\n";
        }
        std::cout << "Execution time: " << duration.count() << " microseconds\n";
        break;
    case 2:
        std::cout << "15-Heap algorithm:" << "\n";
        start_time = std::chrono::high_resolution_clock::now();
        dist = LDGDijkstra15Heap(test_graph, n, start_vertex);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        for (int i = 0; i < n; i++) {
            std::cout << "Vertex: " << i << " Dist: = " << dist[i] << "\n";
        }
        std::cout << "Execution time: " << duration.count() << " microseconds\n";
        break;
    default:
        std::cout << "Invalid input" << "\n";
        break;
    }

    /*std::cout << "Mark algorithm:" << "\n";
    std::vector<int> dist_mark = LDGDijkstraMark(test_graph, n, start_vertex);
    for (int i = 0; i < n; i++) {
        std::cout << "Vertex: " << i << " Dist: = " << dist_mark[i] << "\n";
    }

    std::cout << "15-Heap algorithm:" << "\n";
    std::vector<int> dist_heap = LDGDijkstra15Heap(test_graph, n, start_vertex);
    for (int i = 0; i < n; i++) {
        std::cout << "Vertex: " << i << " Dist: = " << dist_heap[i] << "\n";
    }*/

    return 0;
}