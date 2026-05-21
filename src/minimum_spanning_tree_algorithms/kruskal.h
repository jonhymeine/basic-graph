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

struct KruskalMSTResult
{
  vector<tuple<int, int, float>> edges;
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

  if (!graph)
    return result;

  vector<int> vertices = graph->get_vertices();

  if (vertices.empty())
    return result;

  auto start_time = chrono::high_resolution_clock::now();

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

      result.edges.push_back({u, v, weight});
      result.total_weight += weight;
    }
  }

  result.is_connected = (result.edges.size() == vertices.size() - 1);

  auto end_time = chrono::high_resolution_clock::now();

  result.execution_time_ms =
      chrono::duration<double, milli>(end_time - start_time).count();

  return result;
}

inline void print_kruskal_mst_result(Graph *graph, const KruskalMSTResult &result)
{
  if (!graph)
    return;

  cout << "Arvore Geradora Minima (Kruskal):" << endl;

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

  cout << "Tempo de execucao: "
       << fixed << setprecision(6)
       << result.execution_time_ms << " ms" << endl;

  if (!result.is_connected)
  {
    cout << "Aviso: o grafo eh desconexo; "
         << "o resultado representa uma floresta geradora minima."
         << endl;
  }
}

#endif