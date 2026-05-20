#ifndef WELSH_POWELL_H
#define WELSH_POWELL_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>

#include "../graph/graph.h"

using namespace std;
using namespace std::chrono;

void welsh_powell(Graph* graph)
{
    if (!graph)
        return;

    auto start = high_resolution_clock::now();

    unordered_map<int, int> colors;

    vector<int> vertices = graph->get_vertices();

    sort(vertices.begin(),
         vertices.end(),
         [&](int a, int b)
         {
             return graph->get_neighbors(a).size() >
                    graph->get_neighbors(b).size();
         });

    for (int vertex : vertices)
    {
        vector<bool> unavailable(vertices.size(), false);

        vector<int> neighbors = graph->get_neighbors(vertex);

        for (int neighbor : neighbors)
        {
            if (colors.find(neighbor) != colors.end())
            {
                unavailable[colors[neighbor]] = true;
            }
        }

        int color;

        for (color = 0; color < vertices.size(); color++)
        {
            if (!unavailable[color])
                break;
        }

        colors[vertex] = color;
    }

    int max_color = 0;

    for (auto pair : colors)
    {
        max_color = max(max_color, pair.second);
    }

    auto end = high_resolution_clock::now();

    double duration =
        duration_cast<microseconds>(end - start).count() / 1000.0;

    cout << "Welsh-Powell" << endl;
    cout << "Number of colors used: " << max_color + 1 << endl;
    cout << "Time: " << duration << " ms" << endl;

    if (graph->get_vertices_count() < 10)
    {
        cout << "Coloring:" << endl;

        for (auto pair : colors)
        {
            cout << "Vertex "
                 << pair.first
                 << " -> Color "
                 << pair.second
                 << endl;
        }
    }

    cout << endl;
}

#endif