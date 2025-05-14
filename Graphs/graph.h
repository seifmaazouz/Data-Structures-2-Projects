#ifndef GRAPH_H
#define GRAPH_H
#include <vector>

using namespace std;

class Graph {
private:
    vector<vector<int>> adjList;
    bool isDirected;

    void DFSHelper(int start, vector<bool>& visited);
    void BFSHelper(int start, vector<bool>& visited);

public:
    Graph(int vertices, bool isDirected = false);

    int size();
    vector<int> getAdjList(int index);
    void addEdge(int src, int dst);

    void DFS(int start);
    void BFS(int start);

    void DFSAll();
    void BFSAll();

    void print();
};

#endif //GRAPH_H
