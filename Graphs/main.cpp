#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include "graph.h"

using namespace std;


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

// Prim's algorithm for Minimum Spanning Tree (MST)
vector <vector <pair<int,int>>> findMST(int n,vector <vector <pair<int,int>>> &adj,int source,int &sum) {
    vector <vector <pair<int,int>>> result;
    result.resize(n);
    priority_queue <pair<int,pair<int,int>>, vector <pair<int,pair<int,int>>>, greater<pair<int,pair<int,int>>>> pq;
    vector <int> visited(n,0);
    pq.push({0,{source,-1}});
    while (!pq.empty()) {
        auto it = pq.top();
        pq.pop();
        int weight = it.first;
        int node = it.second.first;
        int parent = it.second.second;
        if (visited[node] == 1) {continue;}
        visited[node] = 1;
        sum += weight;
        if (parent != -1) {
            result[node].push_back({parent,weight});
        }
        for (auto it: adj[node]) {
            int adjnode = it.first;
            int weight = it.second;
            if (!visited[adjnode]) {
                pq.push({weight,{adjnode,node}});
            }
        }
    }
    return result;
}

int main() {
    Graph g(5); // undirected graph
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

    // make a weighted undirected graph and test MST
    // make hardcoded values graph to put in mst directly without user input
    int n1 = 5, m1 = 7;
    vector <vector <pair<int,int>>> adj1;
    adj1.resize(n1);
    adj1[0].push_back({1,2});
    adj1[0].push_back({3,6});
    adj1[1].push_back({0,2});
    adj1[1].push_back({2,3});
    adj1[1].push_back({3,8});
    adj1[1].push_back({4,5});
    adj1[2].push_back({1,3});
    adj1[2].push_back({3,7});
    adj1[3].push_back({0,6});
    adj1[3].push_back({1,8});
    adj1[3].push_back({2,7});
    adj1[3].push_back({4,9});
    adj1[4].push_back({1,5});
    adj1[4].push_back({3,9});

    // print graph adj list
    cout << "Adjacency List of the graph:" << endl;
    for (int i = 0; i < n1; i++) {
        cout << i << ": ";
        for (auto it : adj1[i]) {
            cout << "(" << it.first << ", " << it.second << ") ";
        }
        cout << endl;
    }

    //
    cout << "MST for this graph is: " << endl;
    int sum1 = 0;
    vector <vector <pair<int,int>>> res1 = findMST(n1,adj1,0,sum1);
    for (int i=0;i<res1.size();i++) {
        if (!res1[i].empty()) {
            cout << "(" << i << " - " << res1[i][0].first << ", weight = " << res1[i][0].second << ") ";
        }
    }
    cout << endl << "MST sum: " << sum1 << endl;

    // make a weighted undirected graph and test MST
    // user input graph
    cout<<endl;
    int n,m;
    cout << "Enter new n and m for MST graph: " << endl;
    cin >> n >> m;
    vector <vector <pair<int,int>>> adj;
    adj.resize(n);
    cout << "Enter the edges for MST graph: " << endl;
    for (int i = 0; i < m; i++) {
        int u,v,weight;
        cin>>u>>v>>weight;
        adj[u].push_back({v,weight});
        adj[v].push_back({u,weight});
    }
    cout << "MST for this graph is: " << endl;
    int sum = 0;
    vector <vector <pair<int,int>>> res = findMST(n,adj,0,sum);
    for (int i=0;i<res.size();i++) {
        if (!res[i].empty()) {
            cout << "(" << i << " - " << res[i][0].first << ", weight = " << res[i][0].second << ") ";
        }
    }
    cout << "MST sum: " << endl;
    cout << sum << endl;

    return 0;
}
//
// int main() {
//     // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the
//     // <b>lang</b> variable name to see how CLion can help you rename it.
//     cout << "Enter n and m for Topological sort graph: " << endl;
//     int n,m;
//     cin>>n>>m;
//     vector<vector<int>> graph;
//     graph.resize(n);
//     cout << "Enter the edges for topological sort graph: " << endl;
//     for (int i = 0; i < m; i++) {
//         int u,v;
//         cin>>u>>v;
//         graph[u].push_back(v);
//     }
//     vector <int> result = topologicalSort(n,graph);
//     cout << "Toplogical sort for this graph is: " << endl;
//     for (int i = 0; i < result.size(); i++) {
//         cout<<result[i]<<" ";
//     }
//
//     return 0;
// }
