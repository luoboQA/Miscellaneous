#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 拓扑排序（Kahn算法-BFS） ====================

bool TopologicalSort_Kahn(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<int> indegree(n, 0);
    vector<int> result;
    
    // 计算入度
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            indegree[graph[i][j]]++;
        }
    }
    
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);
        
        for (int v : graph[u]) {
            indegree[v]--;
            if (indegree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    // 判断是否有环
    if (result.size() != n) {
        return false;  // 有环
    }
    
    printf("拓扑排序（Kahn算法）：");
    for (int v : result) {
        printf("%d ", v);
    }
    printf("\n");
    return true;
}

// ==================== 拓扑排序（DFS） ====================

bool DFS_Topological(vector<vector<int>> &graph, int u, vector<int> &visited, vector<int> &result) {
    visited[u] = 1;  // 正在访问
    
    for (int v : graph[u]) {
        if (visited[v] == 1) {
            return false;  // 发现环
        }
        if (visited[v] == 0) {
            if (!DFS_Topological(graph, v, visited, result)) {
                return false;
            }
        }
    }
    
    visited[u] = 2;  // 访问完成
    result.push_back(u);
    return true;
}

void TopologicalSort_DFS(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<int> visited(n, 0);
    vector<int> result;
    
    for (int i = 0; i < n; i++) {
        if (visited[i] == 0) {
            if (!DFS_Topological(graph, i, visited, result)) {
                printf("图存在环，无法进行拓扑排序！\n");
                return;
            }
        }
    }
    
    // 反转得到拓扑序
    reverse(result.begin(), result.end());
    printf("拓扑排序（DFS算法）：");
    for (int v : result) {
        printf("%d ", v);
    }
    printf("\n");
}

int main() {
    printf("========================================\n");
    printf("    拓扑排序（Topological Sort）\n");
    printf("========================================\n");
    printf("时间复杂度：O(V+E)\n");
    printf("空间复杂度：O(V)\n");
    printf("适用：有向无环图（DAG）\n\n");
    
    // 示例图
    int n = 6;
    vector<vector<int>> graph(n);
    graph[5].push_back(2);
    graph[5].push_back(0);
    graph[4].push_back(0);
    graph[4].push_back(1);
    graph[2].push_back(3);
    graph[3].push_back(1);
    
    printf("图结构：\n");
    printf("5 -> 2, 0\n");
    printf("4 -> 0, 1\n");
    printf("2 -> 3\n");
    printf("3 -> 1\n\n");
    
    TopologicalSort_Kahn(graph);
    TopologicalSort_DFS(graph);
    
    printf("\n【带环图测试】\n");
    vector<vector<int>> graphWithCycle(n);
    graphWithCycle[0].push_back(1);
    graphWithCycle[1].push_back(2);
    graphWithCycle[2].push_back(0);  // 形成环
    
    TopologicalSort_DFS(graphWithCycle);
    
    getchar();
    return 0;
}