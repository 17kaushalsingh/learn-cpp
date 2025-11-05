# Chapter 14: Graphs 🕸️

## Graph Traversals

### BFS Traversal
Breadth-First Search explores graph level by level.

```cpp
void bfs(int startNode, vector<int> adj[], int V) {
    queue<int> q;
    vector<bool> visited(V, false);

    q.push(startNode);
    visited[startNode] = true;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        process(cur); // saving/printing

        for (int neighbor : adj[cur]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}
```

### DFS Traversal
Depth-First Search explores graph depth-first.

```cpp
void dfs(int node, vector<int> adj[], vector<bool>& visited) {
    visited[node] = true;
    process(node); // save/print

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor, adj, visited);
        }
    }
}
```

**Note:** `process()` can be:
- Normal traversal: add to `ans[]`
- Print node value
- Flood fill: change color/properties

**neighbors[i]**: adjacent nodes (adjacency list) or connected cells (grid movement)

---

## Cycle Detection

### Undirected Graph Cycle Detection

#### DFS Approach
```cpp
bool dfsCycle(int node, int parent, vector<int> adj[], vector<bool>& visited) {
    visited[node] = true;

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            if (dfsCycle(neighbor, node, adj, visited))
                return true;
        } else if (neighbor != parent) {
            return true; // Cycle detected
        }
    }
    return false;
}
```

#### BFS Approach
```cpp
bool bfsCycle(int start, vector<int> adj[], int V) {
    vector<bool> visited(V, false);
    queue<pair<int, int>> q; // {node, parent}

    q.push({start, -1});
    visited[start] = true;

    while (!q.empty()) {
        auto [node, parent] = q.front();
        q.pop();

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                q.push({neighbor, node});
                visited[neighbor] = true;
            } else if (neighbor != parent) {
                return true; // Cycle detected
            }
        }
    }
    return false;
}
```

**Key Changes:**
1. **DFS**: Add `parent` parameter
2. **BFS**: Store `{node, parent}` pairs in queue
3. **Cycle Detection**: If neighbor visited but not parent → cycle exists

---

## Bipartite Graph

A graph is bipartite if nodes can be partitioned into two sets A and B such that every edge connects a node from A to B.

### DFS Bipartite Check
```cpp
bool dfsBipartite(int node, vector<int> adj[], vector<int>& color) {
    for (int neighbor : adj[node]) {
        if (color[neighbor] == -1) {
            color[neighbor] = 1 - color[node];
            if (!dfsBipartite(neighbor, adj, color))
                return false;
        } else if (color[neighbor] == color[node]) {
            return false; // Same color on adjacent nodes
        }
    }
    return true;
}
```

### BFS Bipartite Check
```cpp
bool bfsBipartite(int start, vector<int> adj[], vector<int>& color) {
    queue<int> q;
    q.push(start);
    color[start] = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : adj[node]) {
            if (color[neighbor] == -1) {
                color[neighbor] = 1 - color[node];
                q.push(neighbor);
            } else if (color[neighbor] == color[node]) {
                return false; // Conflict detected
            }
        }
    }
    return true;
}
```

**Key Changes:**
1. Use `color[]` array: `-1` (uncolored), `0/1` (opposite colors)
2. **BFS**: Compare current with all neighbors for conflicts
3. **DFS**: Assign color, then recurse if uncolored; check conflict if colored

---

## Topological Sort

**Only possible for Directed Acyclic Graphs (DAG)**

### Kahn's Algorithm (BFS)
```cpp
vector<int> bfsTopo(vector<int> adj[], int V) {
    vector<int> inDegree(V, 0);

    // Calculate in-degrees
    for (int i = 0; i < V; i++) {
        for (int neighbor : adj[i]) {
            inDegree[neighbor]++;
        }
    }

    queue<int> q;
    vector<int> result;

    // Push all nodes with inDegree = 0
    for (int i = 0; i < V; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        result.push_back(node);

        for (int neighbor : adj[node]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    return result;
}
```

### DFS Topological Sort
```cpp
void dfsTopo(int node, vector<int> adj[], vector<bool>& visited, stack<int>& st) {
    visited[node] = true;

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfsTopo(neighbor, adj, visited, st);
        }
    }

    st.push(node); // Add after all neighbors processed
}

vector<int> topologicalSortDFS(vector<int> adj[], int V) {
    vector<bool> visited(V, false);
    stack<int> st;

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            dfsTopo(i, adj, visited, st);
        }
    }

    vector<int> result;
    while (!st.empty()) {
        result.push_back(st.top());
        st.pop();
    }

    return result; // Stack contains topo sort in reverse order
}
```

---

## Directed Graph Cycle Detection

### DFS Approach (Path Visited)
```cpp
bool dfsCycleDir(int node, vector<int> adj[], vector<bool>& visited, vector<bool>& pathVisited) {
    visited[node] = true;
    pathVisited[node] = true;

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            if (dfsCycleDir(neighbor, adj, visited, pathVisited))
                return true;
        } else if (pathVisited[neighbor]) {
            return true; // Back edge found
        }
    }

    pathVisited[node] = false; // Backtrack
    return false;
}
```

### BFS Approach (Kahn's Algorithm)
```cpp
bool hasCycleTopo(vector<int> adj[], int V) {
    vector<int> topoResult = bfsTopo(adj, V);
    return topoResult.size() != V; // If size < V, cycle exists
}
```

**Key Points:**
- **DFS**: Maintain both `visited[]` and `pathVisited[]` arrays
- **Cycle**: If neighbor is in `pathVisited[]` → back edge (cycle)
- **BFS**: If topological sort size ≠ vertices → cycle exists
- Remember to backtrack: `pathVisited[node] = false` after exploring all neighbors

---

## Time & Space Complexity

| Algorithm | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| BFS/DFS Traversal | O(V + E) | O(V) |
| Cycle Detection (Undirected) | O(V + E) | O(V) |
| Cycle Detection (Directed) | O(V + E) | O(V) |
| Topological Sort (BFS) | O(V + E) | O(V) |
| Topological Sort (DFS) | O(V + E) | O(V) |
| Bipartite Check | O(V + E) | O(V) |

Where V = vertices, E = edges

---

## Common Interview Questions

### 1. **Find the number of connected components**
```cpp
int connectedComponents(vector<int> adj[], int V) {
    vector<bool> visited(V, false);
    int components = 0;

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            bfs(i, adj, V);
            components++;
        }
    }
    return components;
}
```

### 2. **Shortest path in unweighted graph**
```cpp
vector<int> shortestPath(int src, vector<int> adj[], int V) {
    vector<int> dist(V, INT_MAX);
    queue<int> q;

    dist[src] = 0;
    q.push(src);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : adj[node]) {
            if (dist[neighbor] == INT_MAX) {
                dist[neighbor] = dist[node] + 1;
                q.push(neighbor);
            }
        }
    }

    return dist;
}
```

### 3. **Check if graph is a tree**
```cpp
bool isTree(int V, vector<int> adj[]) {
    vector<bool> visited(V, false);

    // Check for cycle
    if (dfsCycle(0, -1, adj, visited))
        return false;

    // Check if all nodes are connected
    for (int i = 0; i < V; i++) {
        if (!visited[i])
            return false;
    }

    return true;
}
```

---

## Graph Representation Types

### Adjacency List
```cpp
vector<int> adj[V]; // V = number of vertices
adj[u].push_back(v); // Edge u -> v
```

### Adjacency Matrix
```cpp
vector<vector<int>> matrix(V, vector<int>(V, 0));
matrix[u][v] = 1; // Edge u -> v
```

### When to use which:
- **Adjacency List**: Sparse graphs (E << V²)
- **Adjacency Matrix**: Dense graphs, quick edge lookup

---

## Quick Reference Cheat Sheet

```cpp
// BFS Template
queue<int> q;
vector<bool> visited(V, false);
q.push(start); visited[start] = true;

while (!q.empty()) {
    int node = q.front(); q.pop();
    // Process node

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            q.push(neighbor);
        }
    }
}

// DFS Template
void dfs(int node, vector<bool>& visited) {
    visited[node] = true;
    // Process node

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor, visited);
        }
    }
}

// Cycle Detection (Undirected)
if (visited[neighbor] && neighbor != parent) // cycle

// Cycle Detection (Directed)
if (pathVisited[neighbor]) // cycle

// Bipartite Check
if (color[neighbor] == color[node]) // not bipartite
```

---

**Key Takeaways:**
1. **BFS**: Level order traversal, shortest path in unweighted graphs
2. **DFS**: Connectivity, cycle detection, topological sort
3. **Topological Sort**: Only for DAGs, two approaches (DFS stack, BFS Kahn's)
4. **Cycle Detection**: Different approaches for directed vs undirected graphs
5. **Bipartite**: Two-coloring technique, check for adjacent same colors