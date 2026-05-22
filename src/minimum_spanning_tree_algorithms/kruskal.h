#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../graph/graph.h"

using namespace std;
using namespace std::chrono;

struct KruskalMSTResult
{
  float total_weight = 0;
  double execution_time_ms = 0;
  bool is_connected = true;
};

class DisjointSet
{
private:
  unordered_map<int, int> parent;
  unordered_map<int, int> rank;

public:
  void make_set(const vector<int> &vertices)
  {
    for (int v : vertices)
    {
      parent[v] = v;
      rank[v] = 0;
    }
  }

  int find(int v)
  {
    if (parent[v] != v)
      parent[v] = find(parent[v]);

    return parent[v];
  }

  void unite(int a, int b)
  {
    a = find(a);
    b = find(b);

    if (a != b)
    {
      if (rank[a] < rank[b])
        swap(a, b);

      parent[b] = a;

      if (rank[a] == rank[b])
        rank[a]++;
    }
  }
};

inline KruskalMSTResult kruskal_mst(Graph *graph)
{
  KruskalMSTResult result;
  vector<tuple<int, int, float>> result_edges;

  if (!graph)
    return result;

  vector<int> vertices = graph->get_vertices();

  if (vertices.empty())
    return result;

  auto start_time = high_resolution_clock::now();

  vector<tuple<float, int, int>> edges;

  for (int u : vertices)
  {
    for (int v : graph->get_neighbors(u))
    {
      if (u < v)
      {
        float weight = graph->get_edge_weight(u, v);
        edges.push_back({weight, u, v});
      }
    }
  }

  sort(edges.begin(), edges.end());

  DisjointSet ds;
  ds.make_set(vertices);

  for (const auto &[weight, u, v] : edges)
  {
    if (ds.find(u) != ds.find(v))
    {
      ds.unite(u, v);

      result_edges.push_back({u, v, weight});
      result.total_weight += weight;
    }
  }

  result.is_connected = (result_edges.size() == vertices.size() - 1);

  auto end_time = high_resolution_clock::now();

  result.execution_time_ms = duration<double, milli>(end_time - start_time).count();

  return result;
}

inline void print_kruskal_mst_result(KruskalMSTResult &result)
{
  cout << "Minimum Spanning Tree (Kruskal):" << endl;
  cout << "Sum of edges: " << result.total_weight << endl;
  cout << "Execution time: " << fixed << setprecision(6) << result.execution_time_ms << " ms" << endl;

  if (!result.is_connected)
    cout << "Warning: the graph is disconnected; the result represents a minimum spanning forest." << endl;
}

#endif