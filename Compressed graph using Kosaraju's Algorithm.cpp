#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    vector<vector<int>> adj, adj1;
    vector<int> visited, sccId;
    stack<int> sccStack;
    int sccCount = 0;
    void dfs1(int node) {
        visited[node] = 1;
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs1(neighbor);
            }
        }
        sccStack.push(node); 
    }
    void dfs2(int node, int id) {
        sccId[node] = id; // Assign SCC ID
        for (int neighbor : adj1[node]) {
            if (sccId[neighbor] == -1) {
                dfs2(neighbor, id);
            }
        }
    }
    vector<int> countVisitedNodes(vector<int>& edges) {
        int n = edges.size();
        adj.resize(n);
        adj1.resize(n);
        visited.assign(n, 0);
        sccId.assign(n, -1);
        for (int i = 0; i < n; i++) {
            int y = edges[i];
            adj[i].push_back(y); // Original graph
            adj1[y].push_back(i); // Reversed graph
        }
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs1(i);
            }
        }
        while (!sccStack.empty()) {
            int node = sccStack.top();
            sccStack.pop();
            if (sccId[node] == -1) {
                dfs2(node, sccCount++);
            }
        }
        vector<int> sccSize(sccCount, 0); // Size of each SCC
        for (int i = 0; i < n; i++) {
            sccSize[sccId[i]]++; // Count nodes in each SCC
        }
        vector<vector<int>> compressedGraph(sccCount);
        for (int i = 0; i < n; i++) {
            for (int neighbor : adj[i]) {
                if (sccId[i] != sccId[neighbor]) {
                    compressedGraph[sccId[neighbor]].push_back(sccId[i]);
                }
            }
        }
        vector<int> result(n, 0);
        for (int i = 0; i < n; i++) {
            result[i] = sccSize[sccId[i]];
        }
        vector<int> dist(sccCount, -1); // Distance for each SCC
        queue<int> q;
        for (int i = 0; i < sccCount; i++) {
            if (sccSize[i] > 1) {
                dist[i] = sccSize[i]; 
                q.push(i);            // Start BFS from this SCC
            }
        }
        while (!q.empty()) {
            int currSCC = q.front();
            q.pop();

            for (int neighbor : compressedGraph[currSCC]) {
                if (dist[neighbor] < dist[currSCC] + 1) { 
                    dist[neighbor] = dist[currSCC] + 1;
                    q.push(neighbor); 
                }
            }
        }
        for (int i = 0; i < n; i++) {
            result[i] = dist[sccId[i]];
        }
        return result; 
    }
};
