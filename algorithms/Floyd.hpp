/*
 *（动态规划，中间顶点，松弛）。通过动态规划逐次引入中间顶点，更新所有顶点对之间的最短路径。
 *
 * Floyd-Warshall算法思路：
 * 1. 初始化距离矩阵 dist[i][j] = graph[i][j]
 * 2. 初始化路径矩阵 next[i][j] = j（若i→j有边）
 * 3. 对每个中间顶点 k（0到n-1）：
 *    - 对每对顶点 (i, j)：
 *      - 若 dist[i][k] + dist[k][j] < dist[i][j]
 *      - 则更新 dist[i][j] = dist[i][k] + dist[k][j]
 *      - 同时更新 next[i][j] = next[i][k]
 * 4. 检测负权环：若 dist[i][i] < 0，则存在负权环
 *
 * 最短路径算法	时间复杂度	空间复杂度	适用场景
 * Floyd-Warshall	O(V³)	O(V²)	所有顶点对最短路径，可处理负权边（无负权环）
 * 
 * 图示例（5个顶点）：
 * 邻接矩阵：
 *       0   1   2   3   4
 *   0 [ 0,  3,  8, INF,-4]
 *   1 [INF, 0, INF, 1,  7]
 *   2 [INF, 4,  0, INF,INF]
 *   3 [ 2, INF,-5,  0,INF]
 *   4 [INF,INF,INF, 6,  0]
 * 
 * Floyd过程（引入中间顶点k=0）：
 *   所有经过顶点0的路径：
 *     i=3→0→1: dist[3][1] = min(INF, 2+3=5) = 5
 *     i=3→0→2: dist[3][2] = min(-5, 2+8=10) = -5
 *     i=3→0→3: dist[3][3] = min(0, 2+(-4)=-2) = -2（负权环检测）
 *     i=3→0→4: dist[3][4] = min(INF, 2+(-4)=-2) = -2
 * 
 * 引入中间顶点k=1：
 *   i=0→1→3: dist[0][3] = min(INF, 3+1=4) = 4
 *   i=0→1→4: dist[0][4] = min(-4, 3+7=10) = -4
 *   i=2→1→3: dist[2][3] = min(INF, 4+1=5) = 5
 *   i=2→1→4: dist[2][4] = min(INF, 4+7=11) = 11
 * 
 * 引入中间顶点k=2：
 *   i=1→2→3: dist[1][3] = min(1, INF+INF) = 1
 *   i=3→2→0: dist[3][0] = min(2, -5+INF) = 2
 *   i=3→2→1: dist[3][1] = min(5, -5+4=-1) = -1
 *   i=3→2→4: dist[3][4] = min(-2, -5+11=6) = -2
 * 
 * 引入中间顶点k=3：
 *   i=0→3→2: dist[0][2] = min(8, 4-5=-1) = -1
 *   i=0→3→4: dist[0][4] = min(-4, 4+(-2)=2) = -4
 *   i=1→3→2: dist[1][2] = min(INF, 1-5=-4) = -4
 *   i=1→3→4: dist[1][4] = min(7, 1+(-2)=-1) = -1
 *   i=4→3→2: dist[4][2] = min(INF, 6-5=1) = 1
 * 
 * 引入中间顶点k=4：
 *   i=0→4→3: dist[0][3] = min(4, -4+INF) = 4
 *   i=1→4→3: dist[1][3] = min(1, -1+INF) = 1
 *   i=2→4→3: dist[2][3] = min(5, 11+INF) = 5
 *   i=3→4→? : 无更新
 * 
 * 最终最短距离矩阵：
 *       0   1   2   3   4
 *   0 [ 0,  1, -3,  2, -4]
 *   1 [ 3,  0, -4,  1, -1]
 *   2 [ 7,  4,  0,  5,  3]
 *   3 [ 2, -1, -5,  0, -2]
 *   4 [ 8,  5,  1,  6,  0]
 * 
 * 路径 0→4：0→1→3→2→4（0→1:3, 1→3:1, 3→2:-5, 2→4:3, 总长=2）
 * 最短路径 0→4 = -4（0→4 直接边）
 */

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

/**
 * @brief Floyd-Warshall算法（所有顶点对最短路径）
 * 
 * @param graph 邻接矩阵（graph[i][j] 表示 i→j 的权重，INF表示无边）
 * 
 * @note 时间复杂度 O(V³)，空间复杂度 O(V²)
 * @note 可处理负权边，但不能有负权环
 * @note 使用动态规划，逐次引入中间顶点
 * @note 输出最短距离矩阵和路径示例
 */
void FloydWarshall(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<vector<int>> dist = graph;
    vector<vector<int>> next(n, vector<int>(n, -1));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] != INF && i != j) {
                next[i][j] = j;
            }
        }
    }
    
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
    
    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) {
            printf("存在负权环！\n");
            return;
        }
    }
    
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