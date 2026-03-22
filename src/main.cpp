#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "graph.h"
#include "list_graph.h"
#include "matrix_graph.h"
#include "bfs.h"
#include "dfs.h"

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
  if (argc != 3)
  {
    cerr << "Usage: " << argv[0] << " <file> <list|matrix>" << endl;
    return 1;
  }

  const string filename = argv[1];
  const GraphType type = parse_graph_type(argv[2]);

  unique_ptr<Graph> graph(build_graph_from_file(filename, type));

  // Imprime a matriz/lista como você já estava fazendo
  graph->print_graph();

  cout << endl;

  bfs(graph.get(), 0);
  
  // --- ADICIONE AS LINHAS ABAIXO ---
  cout << "\nIniciando Busca em Profundidade (DFS) a partir do vertice 0:" << endl;
  
  // Chamamos o método estático da classe DFS que você criou no dfs.h
  DFS::execute(graph.get(), 0); 
  // --------------------------------

  return 0;
}