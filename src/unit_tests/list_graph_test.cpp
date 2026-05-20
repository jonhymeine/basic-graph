#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../graph/list_graph.h"

using namespace std;

bool check(bool condition, const string &message)
{
  if (condition)
    cout << "[OK] ";
  else
    cout << "[FAIL] ";

  cout << message << endl;
  return condition;
}

bool contains_vertex(const vector<int> &items, int value)
{
  return find(items.begin(), items.end(), value) != items.end();
}

int main()
{
  int passed = 0;
  int total = 0;

  ListGraph graph(false, true);

  int vA = graph.add_vertex("A");
  int vB = graph.add_vertex("B");
  int vC = graph.add_vertex("C");
  int vD = graph.add_vertex("D");

  total++;
  if (check(graph.get_vertex_label(vA) == "A", "add_vertex/get_vertex_label para A"))
    passed++;

  total++;
  if (check(graph.get_vertex_label(vB) == "B", "add_vertex/get_vertex_label para B"))
    passed++;

  total++;
  if (check(graph.get_vertex_label(9999).empty(), "get_vertex_label para vertice inexistente"))
    passed++;

  total++;
  if (check(graph.add_edge(vA, vB, 2.5f), "add_edge(A, B, 2.5)"))
    passed++;

  total++;
  if (check(graph.add_edge(vA, vC, 1.0f), "add_edge(A, C, 1.0)"))
    passed++;

  total++;
  if (check(graph.add_edge(vA, vD, 3.0f), "add_edge(A, D, 3.0)"))
    passed++;

  total++;
  if (check(!graph.add_edge(vA, 9999, 1.0f), "add_edge com vertice inexistente"))
    passed++;

  total++;
  if (check(graph.has_edge(vA, vB), "has_edge(A, B)"))
    passed++;

  total++;
  if (check(graph.has_edge(vB, vA), "has_edge(B, A) em grafo nao direcionado"))
    passed++;

  total++;
  if (check(fabs(graph.get_edge_weight(vA, vB) - 2.5f) < 1e-6f, "get_edge_weight(A, B) == 2.5"))
    passed++;

  vector<int> neighborsA = graph.get_neighbors(vA);
  total++;
  if (check(contains_vertex(neighborsA, vB) && contains_vertex(neighborsA, vC), "get_neighbors(A) contem B e C"))
    passed++;

  total++;
  if (check(graph.remove_edge(vA, vB), "remove_edge(A, B)"))
    passed++;

  total++;
  if (check(!graph.has_edge(vA, vB), "has_edge(A, B) apos remocao"))
    passed++;

  total++;
  if (check(graph.remove_vertex(vC), "remove_vertex(C)"))
    passed++;

  total++;
  if (check(!graph.remove_vertex(vC), "remove_vertex(C) novamente deve falhar"))
    passed++;

  vector<int> neighborsAAfter = graph.get_neighbors(vA);
  total++;
  if (check(!contains_vertex(neighborsAAfter, vC), "get_neighbors(A) nao contem C apos remover vertice"))
    passed++;

  cout << "\nEstado final do grafo:" << endl;
  graph.print_graph();

  cout << "\nResumo: " << passed << "/" << total << " testes passaram." << endl;
  return (passed == total) ? 0 : 1;
}