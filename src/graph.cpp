#include "graph.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <stack>
using namespace std;

// disjoint set

DisjointSet::DisjointSet(int numVertices)
{
    subsets.resize(numVertices);

    for (int i = 0; i < numVertices; ++i)
        makeSet(i);
}

void DisjointSet::makeSet(Vertex x)
{
    subsets[x].parent = x;
}

Vertex DisjointSet::findSet(Vertex x)
{
    if (subsets[x].parent != x)
        subsets[x].parent = findSet(subsets[x].parent);

    return subsets[x].parent;
}

void DisjointSet::unionSets(Vertex x, Vertex y)
{
    Vertex rootX = findSet(x);
    Vertex rootY = findSet(y);

    if (rootX != rootY)
        subsets[rootY].parent = rootX;
}

// graph

Graph Graph::sort_edges() const
{
    Graph sorted = *this;
    sort(sorted.begin(), sorted.end(), [](const Edge& a, const Edge& b) { return a.weight < b.weight;});

    return sorted;
}

VertexList Graph::edges_from(Vertex vertex) const
{
    VertexList neighbors;

    for (const Edge& e : *this)
    {
        if (e.u == vertex)
            neighbors.push_back(e.v);

        else if (e.v == vertex)
            neighbors.push_back(e.u);
    }

    return neighbors;
}

// kruskal

EdgeList Kruskals(const Graph& G)
{
    EdgeList mst;
    Graph sorted = G.sort_edges();
    DisjointSet ds(G.numVertices);

    for (const Edge& e : sorted)
    {
        if (ds.findSet(e.u) != ds.findSet(e.v))
        {
            mst.push_back(e);
            ds.unionSets(e.u, e.v);
        }
    }

    return mst;
}


// dfs

VertexList dfs(const Graph& graph, Vertex startVertex)
{
    VertexList result;
    vector<bool> visited(graph.numVertices, false);
    stack<Vertex> S;
    
    visited[startVertex] = true;
    S.push(startVertex);

    while (!S.empty())
    {
        Vertex v = S.top();
        
        S.pop();
        result.push_back(v);

        VertexList neighbors = graph.edges_from(v);

        for (Vertex w : neighbors)
        {
            if (!visited[w])
            {
                visited[w] = true;
                S.push(w);
            }
        }
    }

    return result;
}

// bfs

VertexList bfs(const Graph& graph, Vertex startVertex)
{
    VertexList result;
    vector<bool> visited(graph.numVertices, false);
    queue<Vertex> Q;
    
    visited[startVertex] = true;
    Q.push(startVertex);

    while (!Q.empty())
    {
        Vertex v = Q.front();
        
        Q.pop();
        result.push_back(v);

        VertexList neighbors = graph.edges_from(v);

        for (Vertex w : neighbors)
        {
            if (!visited[w])
            {
                visited[w] = true;
                Q.push(w);
            }
        }
    }

    return result;
}

// helpers

void error(string msg) 
{
    cerr << msg << endl;
}

int sum_weights(EdgeList const& L)
{
    int total = 0;

    for (const Edge& e : L)
        total += e.weight;

    return total;
}

string get_arg(int argc, char *argv[], string def)
{
    if (argc > 1)
        return argv[1];

    return def;
}

void file_to_graph(string filename, Graph & G)
{
    ifstream in(filename);

    if (!in)
    {
        error("cant open file");
        return;
    }

    in >> G;
}
