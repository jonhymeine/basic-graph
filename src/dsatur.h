#ifndef DSATUR_H
#define DSATUR_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>

#include "graph.h"

using namespace std;
using namespace std::chrono;

void dsatur(Graph* graph)
{
    if (!graph)
        return;

    auto start = high_resolution_clock::now();

    vector<int> vertices = graph->get_vertices();

    unordered_map<int, int> colors;
    unordered_map<int, int> saturation;

    for (int vertex : vertices)
    {
        colors[vertex] = -1;
        saturation[vertex] = 0;
    }

    while (true)
    {
        int selected = -1;
        int highest_saturation = -1;
        int highest_degree = -1;

        for (int vertex : vertices)
        {
            if (colors[vertex] != -1)
                continue;

            int current_saturation = saturation[vertex];
            int current_degree =
                graph->get_neighbors(vertex).size();

            if (current_saturation > highest_saturation ||
               (current_saturation == highest_saturation &&
                current_degree > highest_degree))
            {
                highest_saturation = current_saturation;
                highest_degree = current_degree;
                selected = vertex;
            }
        }

        if (selected == -1)
            break;

        vector<bool> unavailable(vertices.size(), false);

        vector<int> neighbors =
            graph->get_neighbors(selected);

        for (int neighbor : neighbors)
        {
            if (colors[neighbor] != -1)
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

        colors[selected] = color;

        for (int neighbor : neighbors)
        {
            if (colors[neighbor] == -1)
            {
                vector<bool> used(vertices.size(), false);

                vector<int> neighbor_neighbors =
                    graph->get_neighbors(neighbor);

                for (int nn : neighbor_neighbors)
                {
                    if (colors[nn] != -1)
                    {
                        used[colors[nn]] = true;
                    }
                }

                int sat = 0;

                for (bool value : used)
                {
                    if (value)
                        sat++;
                }

                saturation[neighbor] = sat;
            }
        }
    }

    int max_color = 0;

    for (auto pair : colors)
    {
        max_color = max(max_color, pair.second);
    }

    auto end = high_resolution_clock::now();

    double duration =
        duration_cast<microseconds>(end - start).count() / 1000.0;

    cout << "Metodos de forca bruta: Algoritmo DSATUR" << endl;
    cout << "Cores utilizadas: " << max_color + 1 << endl;
    cout << "Tempo: " << duration << " ms" << endl;

    if (graph->get_vertices_count() < 10)
    {
        cout << "Coloracao:" << endl;

        for (auto pair : colors)
        {
            cout << "Vertice "
                 << pair.first
                 << " -> Cor "
                 << pair.second
                 << endl;
        }
    }

    cout << endl;
}

#endif