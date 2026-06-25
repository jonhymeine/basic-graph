#ifndef FORD_FULKERSON_H
#define FORD_FULKERSON_H

#include <algorithm>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../graph/graph.h"
#include "../search_algorithms/dfs.h"

using namespace std;

inline void add_residual_neighbor(unordered_map<int, vector<int>> &neighbors, int from_vertex, int to_vertex)
{
  vector<int> &vertex_neighbors = neighbors[from_vertex];

  if (find(vertex_neighbors.begin(), vertex_neighbors.end(), to_vertex) == vertex_neighbors.end())
    vertex_neighbors.push_back(to_vertex);
}

inline float ford_fulkerson(Graph *graph, int source, int sink)
{
  if (!graph || source == sink)
    return 0;

  unordered_map<int, unordered_map<int, float>> residual_capacity;
  unordered_map<int, vector<int>> residual_neighbors;
  vector<int> vertices = graph->get_vertices();

  for (int vertex : vertices)
  {
    residual_neighbors[vertex] = vector<int>();
  }

  for (int from_vertex : vertices)
  {
    for (int to_vertex : graph->get_neighbors(from_vertex))
    {
      float capacity = graph->get_edge_weight(from_vertex, to_vertex);

      if (capacity <= 0)
        continue;

      residual_capacity[from_vertex][to_vertex] += capacity;
      residual_capacity[to_vertex].try_emplace(from_vertex, 0);

      add_residual_neighbor(residual_neighbors, from_vertex, to_vertex);
      add_residual_neighbor(residual_neighbors, to_vertex, from_vertex);
    }
  }

  float max_flow = 0;

  while (true)
  {
    unordered_set<int> visited;
    unordered_map<int, int> parent;

    if (!DFS::find_path(source, sink, residual_neighbors, residual_capacity, visited, parent, true))
      break;

    float path_flow = numeric_limits<float>::infinity();

    for (int vertex = sink; vertex != source; vertex = parent[vertex])
    {
      int previous = parent[vertex];
      path_flow = min(path_flow, residual_capacity[previous][vertex]);
    }

    for (int vertex = sink; vertex != source; vertex = parent[vertex])
    {
      int previous = parent[vertex];
      residual_capacity[previous][vertex] -= path_flow;
      residual_capacity[vertex][previous] += path_flow;
    }

    max_flow += path_flow;
  }

  return max_flow;
}

#endif
