#include <iostream>
#include <vector>
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

// ==================== Bellman-Ford算法 ====================

typedef struct {
    int u, v;
    int weight;
} Edge;

bool BellmanFord(vector<Edge> &edges, int n, int src, vector<int> &dist) {
    dist.resize(n, INF);
    dist[src] = 0;
    vector<int> parent(n, -1);
    
    // 松弛 V-1 次
    printf("松弛过程：\n");
    for (int i = 1; i <= n - 1; i++) {
        bool updated = false;
        printf("第%d轮：", i);
        for (Edge &e : edges) {
            if (dist[e.u] != INF && dist[e.u] + e.weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.weight;
                parent[e.v] = e.u;
                updated = true;
                printf(" (%d->%d) dist[%d]=%d", e.u, e.v, e.v, dist[e.v]);
            }
        }
        if (!updated) {
            printf(" 收敛\n");
            break;
        }
        printf("\n");
    }
    
    // 检查负权环
    for (Edge &e : edges) {
        if (dist[e.u] != INF && dist[e.u] + e.weight < dist[e.v]) {
            printf("\n发现负权环！\n");
            return false;
        }
    }
    return true;
}

// 打印路径
void PrintPath(vector<int> &parent, int src, int dest) {
    vector<int> path;
    int v = dest;
    while (v != -1) {
        path.push_back(v);
        v = parent[v];
    }
    for (int i = path.size() - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0) printf("->");
    }
    printf("\n");
}

int main() {
    printf("========================================\n");
    printf("    Bellman-Ford最短路径\n");
    printf("========================================\n");
    printf("时间复杂度：O(VE)\n");
    printf("空间复杂度：O(V)\n");
    printf("适用：有负权边、检测负权环\n\n");
    
    int n = 5;
    vector<Edge> edges = {
        {0, 1, 6},
        {0, 3, 7},
        {1, 2, 5},
        {1, 3, 8},
        {1, 4, -4},
        {2, 1, -2},
        {3, 2, -3},
        {3, 4, 9},
        {4, 0, 2},
        {4, 2, 7}
    };
    
    printf("图结构（u->v weight）：\n");
    for (Edge &e : edges) {
        printf("%d->%d weight=%d\n", e.u, e.v, e.weight);
    }
    printf("\n");
    
    vector<int> dist;
    int src = 0;
    
    if (BellmanFord(edges, n, src, dist)) {
        printf("\n从顶点%d到各顶点的最短距离：\n", src);
        for (int i = 0; i < n; i++) {
            printf("到%d：", i);
            if (dist[i] == INF) {
                printf("不可达\n");
            } else {
                printf("%d\n", dist[i]);
            }
        }
    }
    
    printf("\n【Dijkstra vs Bellman-Ford对比】\n");
    printf("Dijkstra：O(V^2)或O(ElogV)，不能有负权边\n");
    printf("Bellman-Ford：O(VE)，可以有负权边，能检测负权环\n");
    
    getchar();
    return 0;
}