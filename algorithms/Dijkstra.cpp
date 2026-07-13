#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define INF INT_MAX

typedef int Status;
typedef int ElemType;

// ==================== Dijkstra算法（邻接矩阵） ====================

void Dijkstra_Matrix(vector<vector<int>> graph, int src) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    
    dist[src] = 0;
    
    for (int count = 0; count < n - 1; count++) {
        // 选择未访问的最小距离顶点
        int u = -1;
        int minDist = INF;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        
        if (u == -1) break;
        visited[u] = true;
        
        // 更新邻接顶点距离
        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }
    
    // 输出结果
    printf("顶点\t距离\t路径\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t", i, dist[i]);
        // 打印路径
        vector<int> path;
        for (int v = i; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        for (int j = path.size() - 1; j >= 0; j--) {
            printf("%d", path[j]);
            if (j > 0) printf("->");
        }
        printf("\n");
    }
}

// ==================== Dijkstra算法（邻接链表 + 优先队列优化） ====================

typedef struct Edge {
    int to;
    int weight;
    Edge(int t, int w) : to(t), weight(w) {}
} Edge;

typedef pair<int, int> PII;  // (距离, 顶点)

void Dijkstra_List(vector<vector<Edge>> graph, int src) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<int> parent(n, -1);
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        
        for (auto &edge : graph[u]) {
            int v = edge.to;
            int w = edge.weight;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    
    printf("\n【优先队列优化版】\n");
    printf("顶点\t距离\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\n", i, dist[i]);
    }
}

int main() {
    printf("========================================\n");
    printf("    Dijkstra最短路径算法\n");
    printf("========================================\n");
    printf("时间复杂度：O(V^2) 或 O(ElogV)\n");
    printf("空间复杂度：O(V^2) 或 O(V+E)\n");
    printf("限制：不能有负权边\n\n");
    
    int n = 5;
    vector<vector<int>> graph = {
        {0, 10, 0, 30, 100},
        {0, 0, 50, 0, 0},
        {0, 0, 0, 0, 10},
        {0, 0, 20, 0, 60},
        {0, 0, 0, 0, 0}
    };
    
    printf("邻接矩阵图：\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    printf("从顶点0出发的最短路径：\n");
    Dijkstra_Matrix(graph, 0);
    
    // 邻接链表版本
    vector<vector<Edge>> graph2(n);
    graph2[0].push_back({1, 10});
    graph2[0].push_back({3, 30});
    graph2[0].push_back({4, 100});
    graph2[1].push_back({2, 50});
    graph2[2].push_back({4, 10});
    graph2[3].push_back({2, 20});
    graph2[3].push_back({4, 60});
    
    Dijkstra_List(graph2, 0);
    
    getchar();
    return 0;
}