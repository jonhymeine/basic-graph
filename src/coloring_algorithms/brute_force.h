#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <functional>
#include <limits>

#include "../graph/graph.h"

using namespace std;
using namespace std::chrono;

void brute_force(Graph *graph)
{
  if (!graph)
    return;

  auto start = high_resolution_clock::now();

  vector<int> vertices = graph->get_vertices();
  const int n = vertices.size();

  if (n == 0)
  {
    cout << "Brute Force Coloring" << endl;
    cout << "Number of colors used: 0" << endl;
    cout << "Time: 0 ms" << endl;
    return;
  }

  unordered_map<int, int> vertex_id_to_index;
  vertex_id_to_index.reserve(n * 2);
  for (int i = 0; i < n; ++i)
    vertex_id_to_index[vertices[i]] = i;

  unordered_map<int, int> best_colors;
  best_colors.reserve(n * 2);
  for (int v : vertices)
    best_colors[v] = -1;
  int best_num_colors = n;

  unordered_map<int, int> colors;
  colors.reserve(n * 2);
  for (int v : vertices)
    colors[v] = -1;

  function<void(int, int)> dfs = [&](int pos, int used_colors)
  {
    if (used_colors >= best_num_colors)
      return;

    if (pos == n)
    {
      best_num_colors = used_colors;
      best_colors = colors;
      return;
    }

    int vertex = vertices[pos];

    vector<char> unavailable(used_colors, 0);
    for (int neighbor : graph->get_neighbors(vertex))
    {
      int color = colors[neighbor];
      if (color != -1 && color < (int)unavailable.size())
        unavailable[color] = 1;
    }

    for (int c = 0; c < used_colors; ++c)
    {
      if (unavailable[c])
        continue;
      colors[vertex] = c;
      dfs(pos + 1, used_colors);
      colors[vertex] = -1;
    }

    colors[vertex] = used_colors;
    dfs(pos + 1, used_colors + 1);
    colors[vertex] = -1;
  };

  dfs(0, 0);

  auto end = high_resolution_clock::now();
  double duration = duration_cast<microseconds>(end - start).count() / 1000.0;

  cout << "Brute Force Coloring" << endl;
  cout << "Number of colors used: " << best_num_colors << endl;
  cout << "Time: " << duration << " ms" << endl;

  if (graph->get_vertices_count() < 10)
  {
    for (auto pair : best_colors)
    {
      cout << "Vertex "
           << pair.first
           << " -> Color "
           << pair.second
           << endl;
    }
    cout << endl;
  }
}