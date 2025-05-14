#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include "graph.h"

using namespace std;

// Khan's Algorithm for Topological Sorting (Detects Cycles)
vector<int> findTopologicalOrdering(Graph& graph) {
    int n = graph.size();
    vector<int> inDegree(n, 0);

    // Calculate in-degrees
    for (int i = 0; i < n; i++) {
        for (int j : graph.getAdjList(i)) {
            inDegree[j]++;
        }
    }

    // Enqueue vertices with in-degree 0
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    // Perform BFS for topological sorting
    vector<int> order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);

        // Decrement in-degree of neighbors and enqueue if it becomes 0 (removes the edge outgoing from v)
        for (int neighbor : graph.getAdjList(v)) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    // If the order size is not equal to n, there is a cycle in the graph
    if (order.size() != n) {
        cout << "Topological Sort: Graph has a cycle. Ordering not possible.\n";
        return {};
    }

    return order;
}

// Prim's algorithm for Minimum Spanning Tree (MST)
vector<vector<pair<int, int>>> findMST(int n, vector<vector<pair<int, int>>> &adj, int source, int &sum) {
    vector<vector<pair<int, int>>> result;  // Resulting MST (each node stores its parent and edge weight)
    result.resize(n);

    // Min-heap priority queue to choose the edge with the smallest weight
    // Format: {weight, {current_node, parent_node}}
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;

    // Visited array to mark which nodes are included in MST
    vector<int> visited(n, 0);

    // Start from the source node (e.g., node 0) with 0 cost and no parent (-1)
    pq.push({0, {source, -1}});

    while (!pq.empty()) {
        // Extract edge with minimum weight
        auto it = pq.top();
        pq.pop();
        int weight = it.first;           // Weight of the current edge
        int node = it.second.first;      // Current node
        int parent = it.second.second;   // Parent node from which we came to this node

        // Skip if the node is already included in the MST
        if (visited[node] == 1) {
            continue;
        }

        // Mark the node as visited
        visited[node] = 1;

        // Add the weight of this edge to the total sum
        sum += weight;

        // If parent is valid (not the starting node), store the edge in result
        if (parent != -1) {
            result[node].push_back({parent, weight});
        }

        // Traverse all neighbors of the current node
        for (auto it : adj[node]) {
            int adjnode = it.first;   // Adjacent node
            int weight = it.second;   // Edge weight

            // If the neighbor is not visited, push it to the priority queue
            if (!visited[adjnode]) {
                pq.push({weight, {adjnode, node}});
            }
        }
    }

    // Return the resulting MST
    return result;
}

int main() {
    cout << "===== UNDIRECTED GRAPH TRAVERSALS =====\n";
    Graph g(5); // undirected graph
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    g.print();
    cout << "\nDFS starting from node 0: ";
    g.DFS(0);
    cout << "DFS starting from node 1: ";
    g.DFS(1);
    cout << "BFS starting from node 0: ";
    g.BFS(0);
    cout << "BFS starting from node 1: ";
    g.BFS(1);


    cout << "\n===== TOPOLOGICAL SORT =====\n";
    Graph g2(6, 1); // directed graph
    g2.addEdge(5, 2);
    g2.addEdge(5, 0);
    g2.addEdge(4, 0);
    g2.addEdge(4, 1);
    g2.addEdge(2, 3);
    g2.addEdge(3, 1);

    g2.print();
    vector<int> order = findTopologicalOrdering(g2);
    if (!order.empty()) {
        cout << "Topological Ordering: ";
        for (int v : order) cout << v << " ";
        cout << endl;
    }

    // Test with a graph that has a cycle
    Graph g3(6, 1); // directed graph with a cycle
    g3.addEdge(0, 1);
    g3.addEdge(1, 2);
    g3.addEdge(2, 3);
    g3.addEdge(3, 1); // cycle: 1 → 2 → 3 → 1

    g3.print();
    order = findTopologicalOrdering(g3);
    if (!order.empty()) {
        cout << "Topological Ordering: ";
        for (int v : order) cout << v << " ";
        cout << endl;
    }


    cout << "\n===== MINIMUM SPANNING TREE (HARD-CODED GRAPH) =====\n";
    int n1 = 5;
    vector<vector<pair<int, int>>> adj1(n1);
    adj1[0] = {{1, 2}, {3, 6}};
    adj1[1] = {{0, 2}, {2, 3}, {3, 8}, {4, 5}};
    adj1[2] = {{1, 3}, {3, 7}};
    adj1[3] = {{0, 6}, {1, 8}, {2, 7}, {4, 9}};
    adj1[4] = {{1, 5}, {3, 9}};

    cout << "Adjacency List:\n";
    for (int i = 0; i < n1; i++) {
        cout << i << ": ";
        for (auto [v, w] : adj1[i]) {
            cout << "(" << v << ", " << w << ") ";
        }
        cout << endl;
    }

    int sum1 = 0;
    vector<vector<pair<int, int>>> res1 = findMST(n1, adj1, 0, sum1);

    cout << "MST Edges:\n";
    for (int i = 0; i < n1; i++) {
        for (auto [parent, weight] : res1[i]) {
            cout << "(" << i << " - " << parent << ", weight = " << weight << ") ";
        }
    }
    cout << "\nTotal MST Weight: " << sum1 << endl;

    cout << "\n===== MINIMUM SPANNING TREE (USER INPUT) =====\n";
    int n, m;
    cout << "Enter number of nodes and edges: ";
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n);
    cout << "Enter " << m << " edges (u v weight):\n";
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    int sum = 0;
    vector<vector<pair<int, int>>> res = findMST(n, adj, 0, sum);

    cout << "MST Edges:\n";
    for (int i = 0; i < n; i++) {
        for (auto [parent, weight] : res[i]) {
            cout << "(" << i << " - " << parent << ", weight = " << weight << ") ";
        }
    }
    cout << "\nTotal MST Weight: " << sum << endl;

    return 0;
}
