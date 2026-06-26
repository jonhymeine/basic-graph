#ifndef LOCAL_SEARCH_FLOW_H
#define LOCAL_SEARCH_FLOW_H

#include <iostream>
#include <vector>
#include <tuple>
#include <memory>

#include "../graph/graph.h"
#include "ford_fulkerson.h"

using namespace std;

struct LocalSearchFlowResult
{
    float original_flow;
    float final_flow;
    int steps;
};

inline unique_ptr<Graph> copy_graph_with_edges(Graph *graph)
{
    unique_ptr<Graph> copy = graph->create_copy_without_edges();

    for (int from : graph->get_vertices())
    {
        for (int to : graph->get_neighbors(from))
        {
            float weight = graph->get_edge_weight(from, to);
            copy->add_edge(from, to, weight);
        }
    }

    return copy;
}

inline LocalSearchFlowResult local_search_max_flow(Graph *graph, int source, int sink)
{
    LocalSearchFlowResult result;
    result.original_flow = ford_fulkerson(graph, source, sink);
    result.final_flow = result.original_flow;
    result.steps = 0;

    bool improved = true;
    unique_ptr<Graph> current = copy_graph_with_edges(graph);

    while (improved)
    {
        improved = false;

        unique_ptr<Graph> best_neighbor = nullptr;
        float best_flow = result.final_flow;

        vector<tuple<int, int, float>> edges;

        for (int from : current->get_vertices())
        {
            for (int to : current->get_neighbors(from))
            {
                float weight = current->get_edge_weight(from, to);
                edges.push_back({from, to, weight});
            }
        }

        for (auto edge : edges)
        {
            int from = get<0>(edge);
            int to = get<1>(edge);
            float weight = get<2>(edge);

            unique_ptr<Graph> neighbor = copy_graph_with_edges(current.get());

            neighbor->remove_edge(from, to);

            if (!neighbor->has_edge(to, from))
                neighbor->add_edge(to, from, weight);

            float neighbor_flow = ford_fulkerson(neighbor.get(), source, sink);

            if (neighbor_flow > best_flow)
            {
                best_flow = neighbor_flow;
                best_neighbor = move(neighbor);
                improved = true;
            }
        }

        if (improved)
        {
            current = move(best_neighbor);
            result.final_flow = best_flow;
            result.steps++;
        }
    }

    return result;
}

inline void print_local_search_flow_result(LocalSearchFlowResult result)
{
    cout << "Busca Local - Fluxo Maximo" << endl;
    cout << "Fluxo maximo da solucao original: " << result.original_flow << endl;
    cout << "Fluxo maximo da solucao final: " << result.final_flow << endl;
    cout << "Numero de passos utilizados: " << result.steps << endl;
}

#endif