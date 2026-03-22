#ifndef DFS_H
#define DFS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "graph.h"

class DFS {
public:
    static void execute(Graph* graph, int start_vertex) {
        if (graph->get_vertex_label(start_vertex) == "" && start_vertex >= 0) {
             
        }

        std::unordered_map<int, bool> visited;
        std::cout << "Ordem de visita: ";
        dfs_recursive(graph, start_vertex, visited);
        std::cout << std::endl;
    }

private:
    static void dfs_recursive(Graph* graph, int current, std::unordered_map<int, bool>& visited) {
        visited[current] = true;
        std::cout << current << " ";

        std::vector<int> neighbors = graph->get_neighbors(current);

        for (int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                dfs_recursive(graph, neighbor, visited);
            }
        }
    }
};

#endif