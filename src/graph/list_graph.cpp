#include <vector>
#include <unordered_map>
#include <iostream>

#include "list_graph.h"

using namespace std;

ListGraph::ListGraph(bool is_directed, bool is_weighted)
    : Graph(is_directed, is_weighted)
{
}

int ListGraph::add_vertex(string label)
{
  // Assign a new vertex ID and add it to the vertices map
  int vertex_id = new_vertex_id++;
  vertices[vertex_id] = label;

  adjacency_list.push_back(vector<Edge>()); // New row for the new vertex

  // Update the vertex ID to index mapping
  vertex_id_to_index[vertex_id] = vertices.size() - 1;
  index_to_vertex_id.push_back(vertex_id);

  return vertex_id;
}

bool ListGraph::remove_vertex(int vertex)
{
  if (vertices.find(vertex) == vertices.end())
    return false;

  // Remove the vertex from the vertices map
  vertices.erase(vertex);

  // Remove vertex from the adjacency list and remove edges to it
  int index = vertex_id_to_index[vertex];
  adjacency_list.erase(adjacency_list.begin() + index);
  for (vector<Edge> &edges : adjacency_list)
  {
    for (auto it = edges.begin(); it != edges.end(); it++)
    {
      if (it->to_vertex == vertex)
      {
        it = edges.erase(it); // Remove edge to the deleted vertex
        break;                // Exit the loop after removing the edge
      }
    }
  }

  // Update the vertex ID to index mapping
  vertex_id_to_index.erase(vertex);
  index_to_vertex_id.erase(index_to_vertex_id.begin() + index);

  // Update the vertex_id_to_index mapping for remaining vertices
  for (auto &pair : vertex_id_to_index)
  {
    int vertex_id = pair.first;
    int &idx = pair.second;
    if (idx > index)
      vertex_id_to_index[vertex_id] = idx - 1;
  }

  return true;
}

void ListGraph::print_graph()
{
  cout << "Adjacency List:\n";

  for (int i = 0; i < adjacency_list.size(); i++)
  {
    cout << index_to_vertex_id[i] << " ->\t";
    for (const Edge &edge : adjacency_list[i])
    {
      cout << edge.to_vertex << ":" << edge.weight << "\t";
    }
    cout << endl;
  }
}

vector<int> ListGraph::get_neighbors(int vertex)
{
  vector<int> neighbors;
  if (vertices.find(vertex) == vertices.end())
    return neighbors;

  int vertex_index = vertex_id_to_index[vertex];
  vector<Edge> &edges = adjacency_list[vertex_index];
  for (auto it = edges.begin(); it != edges.end(); ++it)
  {
    neighbors.push_back(it->to_vertex);
  }

  return neighbors;
}

bool ListGraph::add_edge(int from_vertex, int to_vertex, float weight)
{
  if (vertices.find(from_vertex) == vertices.end() || vertices.find(to_vertex) == vertices.end())
    return false;

  // Get the indices of the vertices in the adjacency list
  int from_index = vertex_id_to_index[from_vertex];
  int to_index = vertex_id_to_index[to_vertex];

  // Add the edge with the specified weight
  adjacency_list[from_index].push_back({to_vertex, weight});
  if (!is_directed)
    adjacency_list[to_index].push_back({from_vertex, weight});

  return true;
}

bool ListGraph::remove_edge(int from_vertex, int to_vertex)
{
  if (vertices.find(from_vertex) == vertices.end() || vertices.find(to_vertex) == vertices.end())
    return false;

  // Get the indices of the vertices in the adjacency list
  int from_index = vertex_id_to_index[from_vertex];
  int to_index = vertex_id_to_index[to_vertex];

  // Remove the edge from from_vertex to to_vertex
  auto &edges_from = adjacency_list[from_index];
  for (auto it = edges_from.begin(); it != edges_from.end(); it++)
  {
    if (it->to_vertex == to_vertex)
    {
      it = edges_from.erase(it); // Remove edge to the target vertex
      break;                     // Exit the loop after removing the edge
    }
  }

  if (!is_directed)
  {
    // Remove the edge from to_vertex to from_vertex
    auto &edges_to = adjacency_list[to_index];
    for (auto it = edges_to.begin(); it != edges_to.end(); it++)
    {
      if (it->to_vertex == from_vertex)
      {
        it = edges_to.erase(it); // Remove edge to the target vertex
        break;                   // Exit the loop after removing the edge
      }
    }
  }

  return true;
}

bool ListGraph::has_edge(int from_vertex, int to_vertex)
{
  if (vertices.find(from_vertex) == vertices.end() || vertices.find(to_vertex) == vertices.end())
    return false;

  // Get the indices of the vertices in the adjacency list
  int from_index = vertex_id_to_index[from_vertex];

  const vector<Edge> &edges = adjacency_list[from_index];
  for (const Edge &edge : edges)
  {
    if (edge.to_vertex == to_vertex)
      return true;
  }

  return false;
}

float ListGraph::get_edge_weight(int from_vertex, int to_vertex)
{
  if (vertices.find(from_vertex) == vertices.end() || vertices.find(to_vertex) == vertices.end())
    return 0;

  // Get the indices of the vertices in the adjacency list
  int from_index = vertex_id_to_index[from_vertex];

  const vector<Edge> &edges = adjacency_list[from_index];
  for (const Edge &edge : edges)
  {
    if (edge.to_vertex == to_vertex)
      return edge.weight;
  }
  return 0;
}

vector<int> ListGraph::get_vertices()
{
  vector<int> result;

  for (const auto &pair : vertices)
  {
    result.push_back(pair.first);
  }

  return result;
}

unique_ptr<Graph> ListGraph::create_copy_without_edges() const
{
  auto copy = make_unique<ListGraph>(is_directed, is_weighted);
  copy->vertices = vertices;
  copy->new_vertex_id = new_vertex_id;

  copy->adjacency_list.reserve(vertices.size());
  for (const auto &pair : vertices)
  {
    int vertex_id = pair.first;
    copy->vertex_id_to_index[vertex_id] = copy->adjacency_list.size();
    copy->index_to_vertex_id.push_back(vertex_id);
    copy->adjacency_list.push_back(vector<Edge>());
  }

  return copy;
}