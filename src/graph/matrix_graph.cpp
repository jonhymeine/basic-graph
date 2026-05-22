#include <vector>
#include <unordered_map>
#include <iostream>

#include "matrix_graph.h"

using namespace std;

MatrixGraph::MatrixGraph(bool is_directed, bool is_weighted)
    : Graph(is_directed, is_weighted)
{
}

int MatrixGraph::add_vertex(string label)
{
  // Assign a new vertex ID and add it to the vertices map
  int vertex_id = new_vertex_id++;
  vertices[vertex_id] = label;

  // Expand the adjacency matrix for the new vertex
  for (vector<float> &row : adjacency_matrix)
  {
    row.push_back(0); // No edge to the new vertex
  }
  adjacency_matrix.push_back(vector<float>(vertices.size(), 0)); // New row for the new vertex

  // Update the vertex ID to index mapping
  vertex_id_to_index[vertex_id] = vertices.size() - 1;
  index_to_vertex_id.push_back(vertex_id);

  return vertex_id;
}

bool MatrixGraph::remove_vertex(int vertex)
{
  if (vertices.find(vertex) == vertices.end())
    return false;

  // Remove the vertex from the vertices map
  vertices.erase(vertex);

  // Remove the corresponding row and column from the adjacency matrix
  int index = vertex_id_to_index[vertex];
  adjacency_matrix.erase(adjacency_matrix.begin() + index);
  for (vector<float> &row : adjacency_matrix)
  {
    row.erase(row.begin() + index);
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

void MatrixGraph::print_graph()
{
  cout << "Adjacency Matrix:\nindex\t";

  for (const int vertex_id : index_to_vertex_id)
  {
    cout << vertex_id << "\t";
  }
  cout << endl;

  for (int i = 0; i < adjacency_matrix.size(); i++)
  {
    cout << index_to_vertex_id[i] << "\t";
    for (float weight : adjacency_matrix[i])
    {
      cout << weight << "\t";
    }
    cout << endl;
  }
}

vector<int> MatrixGraph::get_neighbors(int vertex)
{
  vector<int> neighbors;
  if (vertices.find(vertex) == vertices.end())
    return neighbors;

  int vertex_index = vertex_id_to_index[vertex];
  for (size_t i = 0; i < adjacency_matrix[vertex_index].size(); i++)
  {
    if (adjacency_matrix[vertex_index][i] != 0)
      neighbors.push_back(index_to_vertex_id[i]);
  }
  return neighbors;
}

bool MatrixGraph::add_edge(int from_vertex, int to_vertex, float weight)
{
  if (vertices.find(from_vertex) == vertices.end() || vertices.find(to_vertex) == vertices.end())
    return false;

  // Get the indices of the vertices in the adjacency matrix
  int from_index = vertex_id_to_index[from_vertex];
  int to_index = vertex_id_to_index[to_vertex];

  // Add the edge with the specified weight
  adjacency_matrix[from_index][to_index] = weight;
  if (!is_directed)
    adjacency_matrix[to_index][from_index] = weight;

  return true;
}

bool MatrixGraph::remove_edge(int from_vertex, int to_vertex)
{
  return add_edge(from_vertex, to_vertex, 0);
}

bool MatrixGraph::has_edge(int from_vertex, int to_vertex)
{
  if (vertices.find(from_vertex) == vertices.end() || vertices.find(to_vertex) == vertices.end())
    return false;

  // Get the indices of the vertices in the adjacency matrix
  int from_index = vertex_id_to_index[from_vertex];
  int to_index = vertex_id_to_index[to_vertex];

  return adjacency_matrix[from_index][to_index] != 0;
}

float MatrixGraph::get_edge_weight(int from_vertex, int to_vertex)
{
  if (vertices.find(from_vertex) == vertices.end() || vertices.find(to_vertex) == vertices.end())
    return 0;

  // Get the indices of the vertices in the adjacency matrix
  int from_index = vertex_id_to_index[from_vertex];
  int to_index = vertex_id_to_index[to_vertex];

  return adjacency_matrix[from_index][to_index];
}

vector<int> MatrixGraph::get_vertices()
{
  vector<int> result;

  for (const auto &pair : vertices)
  {
    result.push_back(pair.first);
  }

  return result;
}

unique_ptr<Graph> MatrixGraph::create_copy_without_edges() const
{
  auto copy = make_unique<MatrixGraph>(is_directed, is_weighted);
  copy->vertices = vertices;
  copy->new_vertex_id = new_vertex_id;

  copy->adjacency_matrix.reserve(vertices.size());
  for (const auto &pair : vertices)
  {
    int vertex_id = pair.first;
    copy->vertex_id_to_index[vertex_id] = copy->adjacency_matrix.size();
    copy->index_to_vertex_id.push_back(vertex_id);
    copy->adjacency_matrix.push_back(vector<float>(vertices.size(), 0));
  }

  return copy;
}