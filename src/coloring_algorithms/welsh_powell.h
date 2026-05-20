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

    vector<int> vertices = graph->get_vertices();

    // ordena por decrescente
    sort(vertices.begin(),
         vertices.end(),
         [&](int a, int b)
         {
             return graph->get_neighbors(a).size() >
                    graph->get_neighbors(b).size();
         });

    unordered_map<int, int> colors;

    // inicia sem cor
    for (int vertex : vertices)
    {
        colors[vertex] = -1;
    }

    int current_color = 0;

    // vertice sem cor
    while (true)
    {
        bool has_uncolored = false;

        for (int vertex : vertices)
        {
            if (colors[vertex] == -1)
            {
                has_uncolored = true;
                break;
            }
        }

        if (!has_uncolored)
            break;

        // percorre todos vertices sem cor e tenta colorir com a cor atual
        for (int vertex : vertices)
        {
            if (colors[vertex] != -1)
                continue;

            bool can_color = true;

            vector<int> neighbors =
                graph->get_neighbors(vertex);

            // tem vizinho com mesma cor = não pode colorir
            for (int neighbor : neighbors)
            {
                if (colors[neighbor] == current_color)
                {
                    can_color = false;
                    break;
                }
            }

            // insere cor atual
            if (can_color)
            {
                colors[vertex] = current_color;
            }
        }

        current_color++;
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
    cout << "Number of colors used: "
         << max_color + 1 << endl;

    cout << "Time: "
         << duration
         << " ms" << endl;

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