#include <iostream>
#include <vector>
#include <climits>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define INF 99999

typedef int Status;
typedef int ElemType;

// ==================== Floyd-Warshall算法 ====================

void FloydWarshall(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<vector<int>> dist = graph;
    vector<vector<int>> next(n, vector<int>(n, -1));
    
    // 初始化next矩阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] != INF && i != j) {
                next[i][j] = j;
            }
        }
    }
    
    // Floyd核心算法
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    
    // 检测负权环
    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) {
            printf("存在负权环！\n");
            return;
        }
    }
    
    // 输出结果
    printf("所有顶点对之间的最短距离：\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF) {
                printf("INF\t");
            } else {
                printf("%d\t", dist[i][j]);
            }
        }
        printf("\n");
    }
    
    // 打印路径
    printf("\n路径示例（0->4）：");
    vector<int> path;
    int u = 0, v = 4;
    if (next[u][v] == -1) {
        printf("无路径\n");
    } else {
        path.push_back(u);
        while (u != v) {
            u = next[u][v];
            path.push_back(u);
        }
        for (int i = 0; i < path.size(); i++) {
            printf("%d", path[i]);
            if (i < path.size() - 1) printf("->");
        }
        printf("\n");
    }
}

int main() {
    printf("========================================\n");
    printf("    Floyd-Warshall 算法\n");
    printf("========================================\n");
    printf("时间复杂度：O(V^3)\n");
    printf("空间复杂度：O(V^2)\n");
    printf("适用：所有顶点对最短路径\n\n");
    
    int n = 5;
    vector<vector<int>> graph = {
        {0, 3, 8, INF, -4},
        {INF, 0, INF, 1, 7},
        {INF, 4, 0, INF, INF},
        {2, INF, -5, 0, INF},
        {INF, INF, INF, 6, 0}
    };
    
    printf("邻接矩阵（INF表示无穷）：\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] == INF) {
                printf("INF\t");
            } else {
                printf("%d\t", graph[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
    
    FloydWarshall(graph);
    
    getchar();
    return 0;
}