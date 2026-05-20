#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>
#include <algorithm>
#include "../graph/graph.h"

using namespace std;

void print_path(unordered_map<int, int>& prev, int v) {
    vector<int> path;

    while (prev.find(v) != prev.end()) {
        path.push_back(v);
        v = prev[v];
    }
    path.push_back(v);

    reverse(path.begin(), path.end());

    for (int node : path) {
        cout << node << " ";
    }
}

void dijkstra(Graph* graph, int start_vertex) {
    if (!graph) return;

    unordered_map<int, float> dist;
    unordered_map<int, int> prev;

    vector<int> vertices = graph->get_vertices();

    for (int v : vertices) {
        dist[v] = numeric_limits<float>::infinity();
    }

    dist[start_vertex] = 0;

    priority_queue<
        pair<float, int>,
        vector<pair<float, int>>,
        greater<>
    > pq; // = prioridade na fila

    pq.push({0, start_vertex});

    while (!pq.empty()) {
        auto [current_dist, u] = pq.top();
        pq.pop();

        if (current_dist > dist[u]) continue;

        vector<int> neighbors = graph->get_neighbors(u);

        for (int v : neighbors) {
            float weight = graph->get_edge_weight(u, v);

            float new_dist = dist[u] + weight;

            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                prev[v] = u;
                pq.push({new_dist, v});
            }
        }
    }

    cout << "Dijkstra (origem " << start_vertex << "):\n";

    for (auto& [vertex, distance] : dist) {
        cout << "Vertice " << vertex << " | Distancia: ";

        if (distance == numeric_limits<float>::infinity()) {
            cout << "INF";
        } else {
            cout << distance;
        }

        cout << " | Caminho: ";
        print_path(prev, vertex);
        cout << endl;
    }
}

#endif