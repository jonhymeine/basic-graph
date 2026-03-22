#ifndef BFS_H
#define BFS_H

#include <iostream>
#include <queue>
#include <unordered_set>
#include "graph.h"

using namespace std;

void bfs(Graph* graph, int start_vertex)
{
    if (!graph)
        return;

    unordered_set<int> visited;
    queue<int> q; // q = fila

    visited.insert(start_vertex);
    q.push(start_vertex);

    cout << "BFS: ";

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        cout << current << " ";

        vector<int> neighbors = graph->get_neighbors(current);

        for (int neighbor : neighbors)
        {
            if (!visited.contains(neighbor))
            {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    cout << endl;
}

#endif