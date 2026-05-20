#ifndef DSATUR_H
#define DSATUR_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>

#include "../graph/graph.h"

using namespace std;
using namespace std::chrono;

void dsatur(Graph* graph)
{
    if (!graph)
        return;

    auto start = high_resolution_clock::now();

    vector<int> vertices = graph->get_vertices();

    // grau decrescente para desempate inicial
    sort(vertices.begin(),
         vertices.end(),
         [&](int a, int b)
         {
             return graph->get_neighbors(a).size() >
                    graph->get_neighbors(b).size();
         });

    unordered_map<int, int> colors;
    unordered_map<int, int> saturation;

    // inicializa sem cor e saturação zero
    for (int vertex : vertices)
    {
        colors[vertex] = -1;
        saturation[vertex] = 0;
    }

    // vertice de maior grau = primeiro a ser colorido
    int first_vertex = vertices[0];
    colors[first_vertex] = 0;

    // atualiza saturação dos vizinhos do primeiro vértice
    for (int neighbor : graph->get_neighbors(first_vertex))
    {
        saturation[neighbor] = 1;
    }

    // enquanto vertice = sem cor
    while (true)
    {
        int selected = -1;
        int highest_saturation = -1;
        int highest_degree = -1;

        // pega maior saturation
        for (int vertex : vertices)
        {
            if (colors[vertex] != -1)
                continue;

            int current_saturation =
                saturation[vertex];

            int current_degree =
                graph->get_neighbors(vertex).size();

            // desempata por maior grau se empatar
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

        // cores dos vizinhos = indisponíveis
        for (int neighbor :
             graph->get_neighbors(selected))
        {
            if (colors[neighbor] != -1)
            {
                unavailable[colors[neighbor]] = true;
            }
        }

        // cor disponivel mais baixa
        int color;

        for (color = 0;
             color < vertices.size();
             color++)
        {
            if (!unavailable[color])
                break;
        }

        colors[selected] = color;

        // continua loop atualizando saturação dos vizinhos sem cor
        for (int neighbor :
             graph->get_neighbors(selected))
        {
            if (colors[neighbor] == -1)
            {
                vector<bool> used(vertices.size(), false);

                for (int nn :
                     graph->get_neighbors(neighbor))
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

    cout << "DSATUR" << endl;
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