#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>

#include "graph/graph.h"
#include "graph/list_graph.h"
#include "graph/matrix_graph.h"

#include "search_algorithms/bfs.h"
#include "search_algorithms/dfs.h"
#include "search_algorithms/dijkstra.h"

#include "minimum_spanning_tree_algorithms/prim.h"
#include "minimum_spanning_tree_algorithms/kruskal.h"

#include "coloring_algorithms/greedy_coloring.h"
#include "coloring_algorithms/welsh_powell.h"
#include "coloring_algorithms/dsatur.h"
#include "coloring_algorithms/brute_force.h"

#include "flow_algorithms/ford_fulkerson.h"

using namespace std;

enum class GraphType
{
  List,
  Matrix
};

Graph *build_graph_from_file(const string &filename, GraphType type, bool print_file_info = true)
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

  if (print_file_info)
  {
    cout << "Vertices: " << vertices_count << ", Edges: " << edges_count
         << ", Directed: " << is_directed << ", Weighted: " << is_weighted << endl;
  }

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
  if (argc < 4)
  {
    cerr << "Usage:\n  Search: " << argv[0] << " <file> <list|matrix> search <start_vertex>\n  Coloring: " << argv[0] << " <file> <list|matrix> coloring\n  MST: " << argv[0] << " <file> <list|matrix> mst\n  Flow: " << argv[0] << " <file> <list|matrix> flow <source> <sink>" << endl;
    return 1;
  }
  else if (argc > 6)
  {
    cerr << "Too many arguments. See usage: " << argv[0] << " <file> <list|matrix> [search <start_vertex> | coloring | mst | flow <source> <sink>]" << endl;
    return 1;
  }

  const string filename = argv[1];
  const GraphType type = parse_graph_type(argv[2]);
  const string mode = argv[3];

  if (mode != "search" && mode != "coloring" && mode != "mst" && mode != "flow")
  {
    cerr << "Invalid mode '" << mode << "'. Use 'search', 'coloring', 'mst' or 'flow'." << endl;
    return 1;
  }

  if (mode == "search" && argc != 5)
  {
    cerr << "Search mode requires a start vertex. Usage: " << argv[0] << " <file> <list|matrix> search <start_vertex>" << endl;
    return 1;
  }

  if ((mode == "coloring" || mode == "mst") && argc != 4)
  {
    cerr << "This mode does not take a start vertex. Usage: " << argv[0] << " <file> <list|matrix> " << mode << endl;
    return 1;
  }

  if (mode == "flow" && argc != 6)
  {
    cerr << "Flow mode requires source and sink vertices. Usage: " << argv[0] << " <file> <list|matrix> flow <source> <sink>" << endl;
    return 1;
  }

  int start_vertex = -1;
  if (mode == "search")
  {
    try
    {
      start_vertex = stoi(argv[4]);
    }
    catch (...)
    {
      cerr << "Invalid start vertex: must be an integer." << endl;
      return 1;
    }
  }

  int source_vertex = -1;
  int sink_vertex = -1;
  if (mode == "flow")
  {
    try
    {
      source_vertex = stoi(argv[4]);
      sink_vertex = stoi(argv[5]);
    }
    catch (...)
    {
      cerr << "Invalid source or sink vertex: both must be integers." << endl;
      return 1;
    }
  }

  unique_ptr<Graph> graph(build_graph_from_file(filename, type, mode != "flow"));

  if (mode == "search")
  {
    if (start_vertex < 0 || start_vertex >= graph->get_vertices_count())
    {
      cerr << "Invalid start vertex: " << start_vertex << ". It should be between 0 and " << graph->get_vertices_count() - 1 << "." << endl;
      return 1;
    }
  }

  if (mode == "mst" && graph->is_directed_graph())
  {
    cerr << "MST mode requires an undirected graph. Use a file with is_directed = 0." << endl;
    return 1;
  }

  if (mode == "flow")
  {
    if (source_vertex < 0 || source_vertex >= graph->get_vertices_count())
    {
      cerr << "Invalid source vertex: " << source_vertex << ". It should be between 0 and " << graph->get_vertices_count() - 1 << "." << endl;
      return 1;
    }

    if (sink_vertex < 0 || sink_vertex >= graph->get_vertices_count())
    {
      cerr << "Invalid sink vertex: " << sink_vertex << ". It should be between 0 and " << graph->get_vertices_count() - 1 << "." << endl;
      return 1;
    }

    if (!graph->is_directed_graph())
    {
      cerr << "Flow mode requires a directed graph. Use a file with is_directed = 1." << endl;
      return 1;
    }

    if (!graph->is_weighted_graph())
    {
      cerr << "Flow mode requires a weighted graph. Use a file with is_weighted = 1." << endl;
      return 1;
    }
  }

  if (mode != "flow")
    graph->print_graph();

  if (mode != "flow")
  {
    cout << endl;
    cout << "Execucao dos algoritmos:";
    cout << endl;
  }

  if (mode == "search")
  {
    bfs(graph.get(), start_vertex);
    cout << endl;
    DFS::execute(graph.get(), start_vertex);
    cout << endl;
    dijkstra(graph.get(), start_vertex);
    cout << endl;
  }
  else if (mode == "coloring")
  {
    greedy_coloring(graph.get());
    cout << endl;
    welsh_powell(graph.get());
    cout << endl;
    dsatur(graph.get());
    cout << endl;
    brute_force(graph.get());
    cout << endl;
  }
  else if (mode == "mst")
  {
    cout << "PRIM" << endl;
    PrimMSTResult result = prim_mst(graph.get());
    print_prim_mst_result(result);
    cout << endl;

    cout << "KRUSKAL" << endl;
    KruskalMSTResult kruskal_result = kruskal_mst(graph.get());
    print_kruskal_mst_result(kruskal_result);
    cout << endl;
  }
  else if (mode == "flow")
  {
    float max_flow = ford_fulkerson(graph.get(), source_vertex, sink_vertex);

    cout << "Ford-Fulkerson" << endl;
    cout << "Origem: " << source_vertex << endl;
    cout << "Destino: " << sink_vertex << endl;
    cout << "Fluxo maximo: " << max_flow << endl;
  }

  return 0;
}
