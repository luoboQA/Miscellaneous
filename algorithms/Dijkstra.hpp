/*
 *（距离数组，已访问集合，优先队列）。通过贪心策略，每次选择距离源点最近的未访问顶点，更新其邻接顶点的距离。
 *
 * Dijkstra算法思路：
 * 1. 初始化：源点到自身距离为0，到其他顶点为无穷大
 * 2. 从未访问顶点中选择距离源点最近的顶点 u
 * 3. 标记 u 为已访问
 * 4. 更新 u 的所有邻接顶点 v 的距离：若 dist[u] + w < dist[v]，则更新 dist[v]
 * 5. 重复步骤2-4，直到所有顶点都被访问
 *
 * 最短路径算法	数据结构	平均时间复杂度	空间复杂度	限制条件
 * Dijkstra	邻接矩阵	O(V²)	        O(V²)	    不能有负权边
 *             邻接链表	O(E log V)	O(V+E)	    不能有负权边
 * 
 * 图示例（5个顶点）：
 * 邻接矩阵：
 *       0   1   2   3   4
 *   0 [ 0, 10,  0, 30,100]
 *   1 [ 0,  0, 50,  0,  0]
 *   2 [ 0,  0,  0,  0, 10]
 *   3 [ 0,  0, 20,  0, 60]
 *   4 [ 0,  0,  0,  0,  0]
 * 
 * 源点：0
 * 
 * 初始状态：
 *   dist = [0, ∞, ∞, ∞, ∞]
 *   visited = [T, F, F, F, F]
 * 
 * 第1轮：选择距离最小的未访问顶点 → u = 0 (dist=0)
 *   标记 visited[0] = T
 *   更新邻接顶点：
 *     1: dist[0]+10=10 < ∞ → dist[1]=10
 *     3: dist[0]+30=30 < ∞ → dist[3]=30
 *     4: dist[0]+100=100 < ∞ → dist[4]=100
 *   dist = [0, 10, ∞, 30, 100]
 * 
 * 第2轮：选择距离最小的未访问顶点 → u = 1 (dist=10)
 *   标记 visited[1] = T
 *   更新邻接顶点：
 *     2: dist[1]+50=60 < ∞ → dist[2]=60
 *   dist = [0, 10, 60, 30, 100]
 * 
 * 第3轮：选择距离最小的未访问顶点 → u = 3 (dist=30)
 *   标记 visited[3] = T
 *   更新邻接顶点：
 *     2: dist[3]+20=50 < 60 → dist[2]=50
 *     4: dist[3]+60=90 < 100 → dist[4]=90
 *   dist = [0, 10, 50, 30, 90]
 * 
 * 第4轮：选择距离最小的未访问顶点 → u = 2 (dist=50)
 *   标记 visited[2] = T
 *   更新邻接顶点：
 *     4: dist[2]+10=60 < 90 → dist[4]=60
 *   dist = [0, 10, 50, 30, 60]
 * 
 * 第5轮：选择距离最小的未访问顶点 → u = 4 (dist=60)
 *   标记 visited[4] = T
 *   无邻接顶点需要更新
 *   dist = [0, 10, 50, 30, 60]
 * 
 * 最终最短路径：
 *   到0：0
 *   到1：10  (0→1)
 *   到2：50  (0→3→2)
 *   到3：30  (0→3)
 *   到4：60  (0→3→2→4)
 * 
 * 优先队列优化（邻接链表）：
 *   使用最小堆存储 (距离, 顶点)，每次取出堆顶元素
 *   时间复杂度从 O(V²) 优化到 O(E log V)
 *   适用于稀疏图
 * 
 * 注意事项：
 *   - Dijkstra算法不适用于含有负权边的图
 *   - 负权边请使用 Bellman-Ford 算法
 *   - 邻接矩阵适合稠密图，邻接链表+优先队列适合稀疏图
 */

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

/**
 * @brief Dijkstra最短路径算法（邻接矩阵版本）
 * 
 * @param graph 图的邻接矩阵（graph[i][j] 表示 i→j 的权重，0表示无边）
 * @param src   源点
 * @param target 目标点（-1表示输出所有顶点，否则只输出到target的最短路径）
 * 
 * @note 时间复杂度 O(V²)，空间复杂度 O(V²)
 * @note 适用于稠密图
 * @note 不能处理负权边
 * @note 如果指定target，会在找到target后提前终止，提高效率
 */
void Dijkstra_Matrix(vector<vector<int>> graph, int src, int target = -1) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    
    dist[src] = 0;
    
    // 如果指定的target就是源点，直接返回结果
    if (target == src) {
        printf("起点%d → 终点%d: 最短距离 = 0 (源点即终点)\n", src, target);
        return;
    }
    
    for (int count = 0; count < n - 1; count++) {
        int u = -1;
        int minDist = INF;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || dist[u] == INF) break;
        
        // 如果找到了目标点，提前终止
        if (target != -1 && u == target) break;
        
        visited[u] = true;
        
        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }
    
    // 如果指定了目标点，只输出到目标点的最短路径
    if (target != -1) {
        printf("起点%d → 终点%d: ", src, target);
        if (dist[target] == INF) {
            printf("不可达\n");
        } else {
            printf("最短距离 = %d\n", dist[target]);
            
            // 输出路径
            printf("路径: ");
            vector<int> path;
            for (int v = target; v != -1; v = parent[v]) {
                path.push_back(v);
            }
            for (int j = path.size() - 1; j >= 0; j--) {
                printf("%d", path[j]);
                if (j > 0) printf("->");
            }
            printf("\n");
        }
        return;
    }
    
    // 输出所有顶点的最短距离和路径
    printf("起点: %d\n", src);
    printf("顶点\t距离\t路径\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t", i);
        if (dist[i] == INF) {
            printf("不可达\t-\n");
        } else {
            printf("%d\t", dist[i]);
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
}

// ==================== Dijkstra算法（邻接链表 + 优先队列优化） ====================

/**
 * @brief 边结构体
 * 
 * @param to     目标顶点
 * @param weight 边的权重
 */
typedef struct Edge {
    int to;
    int weight;
    Edge(int t, int w) : to(t), weight(w) {}
} Edge;

typedef pair<int, int> PII;  // (距离, 顶点)

/**
 * @brief Dijkstra最短路径算法（邻接链表 + 优先队列优化版本）
 * 
 * @param graph 图的邻接链表（graph[i] 存储从 i 出发的所有边）
 * @param src   源点
 * @param target 目标点（-1表示输出所有顶点，否则只输出到target的最短路径）
 * 
 * @note 时间复杂度 O(E log V)，空间复杂度 O(V + E)
 * @note 适用于稀疏图
 * @note 使用优先队列（最小堆）优化选择最近顶点的过程
 * @note 不能处理负权边
 * @note 如果指定target，会在找到target后提前终止，提高效率
 */
void Dijkstra_List(vector<vector<Edge>> graph, int src, int target = -1) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<int> parent(n, -1);
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    
    dist[src] = 0;
    pq.push({0, src});
    
    // 如果指定的target就是源点，直接返回结果
    if (target == src) {
        printf("\n【优先队列优化版】\n");
        printf("起点%d → 终点%d: 最短距离 = 0 (源点即终点)\n", src, target);
        return;
    }
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        
        // 如果找到了目标点，提前终止
        if (target != -1 && u == target) break;
        
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
    
    // 如果指定了目标点，只输出到目标点的最短路径
    if (target != -1) {
        printf("起点%d → 终点%d: ", src, target);
        if (dist[target] == INF) {
            printf("不可达\n");
        } else {
            printf("最短距离 = %d\n", dist[target]);
            
            // 输出路径
            printf("路径: ");
            vector<int> path;
            for (int v = target; v != -1; v = parent[v]) {
                path.push_back(v);
            }
            for (int j = path.size() - 1; j >= 0; j--) {
                printf("%d", path[j]);
                if (j > 0) printf("->");
            }
            printf("\n");
        }
        return;
    }
    
    // 输出所有顶点的最短距离和路径
    printf("起点: %d\n", src);
    printf("顶点\t距离\t路径\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t", i);
        if (dist[i] == INF) {
            printf("不可达\t-\n");
        } else {
            printf("%d\t", dist[i]);
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
}