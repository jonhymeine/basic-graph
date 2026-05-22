#ifndef PRIM_H
#define PRIM_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <limits>
#include <unordered_set>
#include <vector>

#include "../graph/graph.h"

using namespace std;
using namespace std::chrono;

struct PrimMSTResult
{
  float total_weight = 0;
  double execution_time_ms = 0;
  bool is_connected = true;
};

inline PrimMSTResult prim_mst(Graph *graph)
{
  PrimMSTResult result;
  unique_ptr<Graph> mst_graph;

  if (!graph)
    return result;

  vector<int> vertices = graph->get_vertices();
  if (vertices.empty())
    return result;

  auto start_time = high_resolution_clock::now();

  mst_graph = graph->create_copy_without_edges();

  unordered_set<int> visited;
  int next_visited_vertex = vertices.front();
  visited.insert(next_visited_vertex);

  while (visited.size() < vertices.size())
  {
    int from_vertex = next_visited_vertex;
    float smallest_weight = numeric_limits<float>::max();

    for (int visited_vertex : visited)
    {
      for (int neighbor : graph->get_neighbors(visited_vertex))
      {
        if (visited.find(neighbor) != visited.end())
          continue;

        float weight = graph->get_edge_weight(visited_vertex, neighbor);
        if (weight < smallest_weight)
        {
          from_vertex = visited_vertex;
          next_visited_vertex = neighbor;
          smallest_weight = weight;
        }
      }
    }

    if (visited.find(next_visited_vertex) != visited.end())
    {
      result.is_connected = false;
      break;
    }

    visited.insert(next_visited_vertex);
    result.total_weight += smallest_weight;
    mst_graph->add_edge(from_vertex, next_visited_vertex, smallest_weight);
  }

  auto end_time = high_resolution_clock::now();
  result.execution_time_ms = duration<double, milli>(end_time - start_time).count();

  return result;
}

inline void print_prim_mst_result(const PrimMSTResult &result)
{
  cout << "Minimum Spanning Tree (Prim):" << endl;
  cout << "Sum of edges: " << result.total_weight << endl;
  cout << "Execution time: " << fixed << setprecision(6) << result.execution_time_ms << " ms" << endl;

  if (!result.is_connected)
    cout << "Warning: the graph is disconnected; the result represents only the reachable component." << endl;
}

#endif
