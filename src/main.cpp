#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>

#include "graph.h"
#include "list_graph.h"
#include "matrix_graph.h"
#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"
#include "greedy_coloring.h"
#include "welsh_powell.h"

using namespace std;

enum class GraphType
{
  List,
  Matrix
};

Graph *build_graph_from_file(const string &filename, GraphType type)
{
  ifstream file(filename);
  if (!file.is_open())
    throw runtime_error("It was not possible to open the file: " + filename);

  int vertices_count, edges_count;
  bool is_directed, is_weighted;

  string line;
  getline(file, line); // vertices_count edges_count is_directed is_weighted
  istringstream iss(line);
  iss >> vertices_count >> edges_count >> is_directed >> is_weighted;

  cout << "Vertices: " << vertices_count << ", Edges: " << edges_count
       << ", Directed: " << is_directed << ", Weighted: " << is_weighted << endl;

  Graph *graph;

  if (type == GraphType::List)
    graph = new ListGraph(is_directed, is_weighted);
  else if (type == GraphType::Matrix)
    graph = new MatrixGraph(is_directed, is_weighted);
  else
    throw runtime_error("Invalid graph type");

  for (int i = 0; i < vertices_count; i++)
  {
    graph->add_vertex();
  }

  while (getline(file, line))
  {
    istringstream edge_iss(line);
    int from_vertex, to_vertex;
    float weight = 1;

    edge_iss >> from_vertex >> to_vertex;
    if (is_weighted)
      edge_iss >> weight;

    graph->add_edge(from_vertex, to_vertex, weight);
  }

  return graph;
}

GraphType parse_graph_type(string type_text)
{
  if (type_text == "list")
    return GraphType::List;

  if (type_text == "matrix")
    return GraphType::Matrix;

  throw runtime_error("Invalid graph type. Use 'list' or 'matrix'.");
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    cerr << "Usage: " << argv[0] << " <file> <list|matrix> <start_vertex>" << endl;
    return 1;
  }
  else if (argc > 4)
  {
    cerr << "Too many arguments. Usage: " << argv[0] << " <file> <list|matrix> <start_vertex>" << endl;
    return 1;
  }

  const string filename = argv[1];
  const GraphType type = parse_graph_type(argv[2]);
  const int start_vertex = stoi(argv[3]);

  unique_ptr<Graph> graph(build_graph_from_file(filename, type));

  if (start_vertex < 0 || start_vertex >= graph->get_vertices_count())
  {
    cerr << "Invalid start vertex: " << start_vertex << ". It should be between 0 and " << graph->get_vertices_count() - 1 << "." << endl;
    return 1;
  }

  graph->print_graph();

  cout << endl;

  cout << "Execucao dos algoritmos:";
  cout << endl;
  bfs(graph.get(), start_vertex);
  cout << endl;
  DFS::execute(graph.get(), start_vertex);
  cout << endl;
  dijkstra(graph.get(), start_vertex);
  cout << endl;
  greedy_coloring(graph.get());
  cout << endl;
  welsh_powell(graph.get());
  cout << endl;

  return 0;
}