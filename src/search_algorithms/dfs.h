#ifndef DFS_H
#define DFS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../graph/graph.h"

class DFS {
public:
    static void execute(Graph* graph, int start_vertex, bool ignore_zero_cost_edges = false) {
        if (graph->get_vertex_label(start_vertex) == "" && start_vertex >= 0) {
             
        }

        std::unordered_map<int, bool> visited;
        std::cout << "DFS: ";
        dfs_recursive(graph, start_vertex, visited, ignore_zero_cost_edges);
        std::cout << std::endl;
    }

    static bool find_path(
        int current,
        int target,
        std::unordered_map<int, std::vector<int>>& neighbors,
        std::unordered_map<int, std::unordered_map<int, float>>& costs,
        std::unordered_set<int>& visited,
        std::unordered_map<int, int>& parent,
        bool ignore_zero_cost_edges = false) {
        if (current == target) {
            return true;
        }

        visited.insert(current);

        for (int neighbor : neighbors[current]) {
            if (visited.find(neighbor) != visited.end()) {
                continue;
            }

            if (ignore_zero_cost_edges && costs[current][neighbor] <= 0) {
                continue;
            }

            parent[neighbor] = current;

            if (find_path(neighbor, target, neighbors, costs, visited, parent, ignore_zero_cost_edges)) {
                return true;
            }
        }

        return false;
    }

private:
    static void dfs_recursive(Graph* graph, int current, std::unordered_map<int, bool>& visited, bool ignore_zero_cost_edges) {
        visited[current] = true;
        std::cout << current << " ";

        std::vector<int> neighbors = graph->get_neighbors(current);

        for (int neighbor : neighbors) {
            if (ignore_zero_cost_edges && graph->get_edge_weight(current, neighbor) <= 0) {
                continue;
            }

            if (visited.find(neighbor) == visited.end()) {
                dfs_recursive(graph, neighbor, visited, ignore_zero_cost_edges);
            }
        }
    }
};

#endif
