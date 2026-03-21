#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include <unordered_map>
#include <vector>

#include "graph.h"

using namespace std;

class MatrixGraph : public Graph
{
  vector<vector<float>> adjacency_matrix;
  unordered_map<int, int> vertex_id_to_index;
  vector<int> index_to_vertex_id;

public:
  MatrixGraph(bool is_directed = false, bool is_weighted = false);

  int add_vertex(string label = "") override;
  bool remove_vertex(int vertex) override;
  void print_graph() override;
  vector<int> get_neighbors(int vertex) override;

  bool add_edge(int from_vertex, int to_vertex, float weight = 1) override;
  bool remove_edge(int from_vertex, int to_vertex) override;
  bool has_edge(int from_vertex, int to_vertex) override;
  float get_edge_weight(int from_vertex, int to_vertex) override;
};

#endif