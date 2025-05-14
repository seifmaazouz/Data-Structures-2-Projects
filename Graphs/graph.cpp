#include "graph.h"
#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;


// constructor
Graph::Graph(int vertices, bool isDirected) : isDirected(isDirected) {
    adjList.resize(vertices);
}

// getters
int Graph::size() {
    return adjList.size();
}

vector<int> Graph::getAdjList(int index) {
    return adjList[index];
}

void Graph::addEdge(int src, int dst) {
    adjList[src].push_back(dst);
    if (isDirected == 0) // if undirected
        adjList[dst].push_back(src);
}

// I made it iterative better than recursive as stack overflow is a problem in recursive DFS and BFS
// Shows only one component of the graph
void Graph::DFSHelper(int start, vector<bool>& visited) {
    stack<int> s;
    s.push(start);
    visited[start] = true;

    cout << "DFS Traversal: ";
    while (!s.empty()) {
        int v = s.top();
        s.pop();
        cout << v << " ";

        for (int neighbor : adjList[v]) {
            if (!visited[neighbor]) {
                s.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}

void Graph::BFSHelper(int start, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    cout << "BFS Traversal: ";
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        cout << v << " ";

        for (int neighbor : adjList[v]) {
            if (!visited[neighbor]) {
                q.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}

// DFS from a specific node
void Graph::DFS(int start) {
    vector<bool> visited(adjList.size(), false);
    DFSHelper(start, visited);
    cout << endl;
}

// BFS from a specific node
void Graph::BFS(int start) {
    vector<bool> visited(adjList.size(), false);
    BFSHelper(start, visited);
    cout << endl;
}

// DFS across all components (useful for disconnected graphs)
void Graph::DFSAll() {
    vector<bool> visited(adjList.size(), false);
    for (int i = 0; i < adjList.size(); i++) {
        if (!visited[i]) {
            DFSHelper(i, visited);
        }
    }
    cout << endl;
}

// BFS across all components (useful for disconnected graphs)
void Graph::BFSAll() {
    vector<bool> visited(adjList.size(), false);
    for (int i = 0; i < adjList.size(); i++) {
        if (!visited[i]) {
            BFSHelper(i, visited);
        }
    }
    cout << endl;
}

void Graph::print() {
    for (int i = 0; i < adjList.size(); i++) {
        cout << i << ": ";
        for (int j : adjList[i]) {
            cout << j << " -> ";
        }
        cout << endl;
    }
}
