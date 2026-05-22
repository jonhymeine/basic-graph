#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include <unordered_map>
#include <vector>

#include "graph.h"

using namespace std;

class ListGraph : public Graph
{
  struct Edge
  {
    int to_vertex;
    float weight;
  };

  vector<vector<Edge>> adjacency_list;
  unordered_map<int, int> vertex_id_to_index;
  vector<int> index_to_vertex_id;

public:
  ListGraph(bool is_directed = false, bool is_weighted = false);

  int add_vertex(string label = "") override;
  bool remove_vertex(int vertex) override;
  void print_graph() override;
  vector<int> get_neighbors(int vertex) override;
  vector<int> get_vertices() override;
  unique_ptr<Graph> create_copy_without_edges() const override;

  bool add_edge(int from_vertex, int to_vertex, float weight = 1) override;
  bool remove_edge(int from_vertex, int to_vertex) override;
  bool has_edge(int from_vertex, int to_vertex) override;
  float get_edge_weight(int from_vertex, int to_vertex) override;
};

#endif