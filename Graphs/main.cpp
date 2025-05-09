#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using namespace std;


class Graph {
private:
    vector<vector<int>> adjList;
    int isDirected;

    void DFSHelper(int start, vector<bool>& visited) {
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

    void BFSHelper(int start, vector<bool>& visited) {
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

public:
    // constructor
    Graph(const int vertices, int isDirected = 0) {
        adjList.resize(vertices);
        this->isDirected = isDirected;
    }

    // getters
    int size() {return adjList.size();}
    vector<int> getAdjList(int index) {return adjList[index];}

    void addEdge(int src, int dst) {
        adjList[src].push_back(dst);
        if (isDirected == 0) // if undirected
            adjList[dst].push_back(src);
    }

    // I made it iterative better than recursive as stack overflow is a problem in recursive DFS and BFS
    // Shows only one component of the graph
    void DFS(int start) {
        vector<bool> visited(adjList.size(), false);
        DFSHelper(start, visited);
        cout << endl;
    }
    void BFS(int start) {
        vector<bool> visited(adjList.size(), false);
        BFSHelper(start, visited);
        cout << endl;
    }

    // Shows all components of the graph
    void DFSAll() {
        vector<bool> visited(adjList.size(), false);
        for (int i = 0; i < adjList.size(); i++) {
            if (!visited[i]) {
                DFSHelper(i, visited);
            }
        }
        cout << endl;
    }
    void BFSAll() {
        vector<bool> visited(adjList.size(), false);
        for (int i = 0; i < adjList.size(); i++) {
            if (!visited[i]) {
                BFSHelper(i, visited);
            }
        }
        cout << endl;
    }

    void print() {
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << ": ";
            for (int j : adjList[i]) {
                cout << j << " -> ";
            }
            cout << endl;
        }
    }


};

// Khan's algorithm (can detect cycles)
vector<int> findTopologicalOrdering(Graph& graph) {
    int n = graph.size();
    vector<int> inDegree(n, 0);
    // calculate in-degrees of all vertices
    for (int i = 0; i < n; i++) {
        for (int j : graph.getAdjList(i)) {
            inDegree[j]++;
        }
    }
    // find all vertices with in-degree 0
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }
    // perform BFS
    vector<int> order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);
        // reduce in-degree of neighbors
        for (int neighbor : graph.getAdjList(v)) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }
    // check if there was a cycle
    if (order.size() != n) {
        cout << "Graph has a cycle, topological ordering not possible." << endl;
        return {};
    }
    return order;
}

int main() {
    Graph g(5, 0); // undirected graph
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    g.print();
    cout << endl;

    g.DFS(0); // output should be 0 4 3 2 1
    g.DFS(1); // output should be 1 4 3 2 0

    g.BFS(0); // output should be 0 1 4 2 3
    g.BFS(1); // output should be 1 0 2 3 4
    cout << endl;

    // make a directed acyclic graph and test topological ordering
    Graph g2(6, 1); // directed graph
    g2.addEdge(5, 2);
    g2.addEdge(5, 0);
    g2.addEdge(4, 0);
    g2.addEdge(4, 1);
    g2.addEdge(2, 3);
    g2.addEdge(3, 1);

    vector<int> order = findTopologicalOrdering(g2);
    if (!order.empty()) {
        cout << "Topological Ordering: ";
        for (int v : order) {
            cout << v << " ";
        }
        cout << endl;
    }


    return 0;
}
