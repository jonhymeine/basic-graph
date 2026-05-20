#ifndef PRIM_H
#define PRIM_H

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../graph/graph.h"

using namespace std;

struct PrimMSTResult
{
  vector<tuple<int, int, float>> edges;
  float total_weight = 0;
  double execution_time_ms = 0;
  bool is_connected = true;
};

inline PrimMSTResult prim_mst(Graph *graph)
{
  PrimMSTResult result;

  if (!graph)
    return result;

  vector<int> vertices = graph->get_vertices();
  if (vertices.empty())
    return result;

  auto start_time = chrono::high_resolution_clock::now();

  unordered_map<int, float> key;
  unordered_map<int, int> parent;
  unordered_set<int> in_mst;

  for (int vertex : vertices)
  {
    key[vertex] = numeric_limits<float>::infinity();
    parent[vertex] = -1;
  }

  priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;

  int start_vertex = vertices.front();
  key[start_vertex] = 0;
  pq.push({0, start_vertex});

  while (!pq.empty())
  {
    auto [current_key, u] = pq.top();
    pq.pop();

    if (in_mst.find(u) != in_mst.end())
      continue;

    in_mst.insert(u);

    for (int v : graph->get_neighbors(u))
    {
      if (in_mst.find(v) != in_mst.end())
        continue;

      float weight = graph->get_edge_weight(u, v);
      if (weight < key[v])
      {
        key[v] = weight;
        parent[v] = u;
        pq.push({key[v], v});
      }
    }
  }

  result.is_connected = (in_mst.size() == vertices.size());

  for (int vertex : vertices)
  {
    if (parent[vertex] != -1)
    {
      result.edges.push_back({parent[vertex], vertex, key[vertex]});
      result.total_weight += key[vertex];
    }
  }

  auto end_time = chrono::high_resolution_clock::now();
  result.execution_time_ms = chrono::duration<double, milli>(end_time - start_time).count();

  return result;
}

inline void print_prim_mst_result(Graph *graph, const PrimMSTResult &result)
{
  if (!graph)
    return;

  cout << "Arvore Geradora Minima (Prim):" << endl;

  if (result.edges.empty())
  {
    cout << "Nenhuma aresta foi selecionada." << endl;
  }
  else
  {
    for (const auto &[from, to, weight] : result.edges)
    {
      cout << from;
      string from_label = graph->get_vertex_label(from);
      if (!from_label.empty())
        cout << "(" << from_label << ")";

      cout << " - " << to;
      string to_label = graph->get_vertex_label(to);
      if (!to_label.empty())
        cout << "(" << to_label << ")";

      cout << " | peso: " << weight << endl;
    }
  }

  cout << "Soma das arestas: " << result.total_weight << endl;
  cout << "Tempo de execucao: " << fixed << setprecision(6) << result.execution_time_ms << " ms" << endl;

  if (!result.is_connected)
    cout << "Aviso: o grafo eh desconexo; o resultado representa apenas a componente alcancada." << endl;
}

#endif
