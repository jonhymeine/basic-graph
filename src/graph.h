#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Graph
{
protected:
  bool is_directed;
  bool is_weighted;
  unordered_map<int, string> vertices;
  int new_vertex_id = 0;

public:
  Graph(bool is_directed = false, bool is_weighted = false)
  {
    this->is_directed = is_directed;
    this->is_weighted = is_weighted;
  }

  virtual int add_vertex(string label = "") = 0;
  virtual bool remove_vertex(int vertex) = 0;
  virtual void print_graph() = 0;
  virtual vector<int> get_neighbors(int vertex) = 0;

  virtual bool add_edge(int from_vertex, int to_vertex, float weight) = 0;
  virtual bool remove_edge(int from_vertex, int to_vertex) = 0;
  virtual bool has_edge(int from_vertex, int to_vertex) = 0;
  virtual float get_edge_weight(int from_vertex, int to_vertex) = 0;

  string get_vertex_label(int vertex)
  {
    if (vertices.find(vertex) != vertices.end())
      return vertices[vertex];

    return "";
  }
};

#endif