/*
 *（入度，零入度队列，DFS）。对DAG的顶点进行线性排序，使得每条有向边的起点都在终点之前。
 *
 * 拓扑排序思路（Kahn算法-BFS）：
 * 1. 计算每个顶点的入度
 * 2. 将所有入度为0的顶点入队
 * 3. 取出队首顶点，加入拓扑序列
 * 4. 移除该顶点的所有出边，将其邻接顶点的入度减1
 * 5. 若邻接顶点入度变为0，入队
 * 6. 重复步骤3-5，直到队列为空
 * 7. 若拓扑序列中的顶点数等于总顶点数，则无环；否则有环
 *
 * 图算法	        时间复杂度	空间复杂度	适用场景
 * 拓扑排序-Kahn	O(V+E)	O(V)	    有向无环图（DAG）
 * 拓扑排序-DFS	O(V+E)	O(V)	    有向无环图（DAG）
 * 
 * 图示例（6个顶点）：
 *   5 → 2 → 3 → 1
 *   5 → 0
 *   4 → 0
 *   4 → 1
 * 
 * 邻接表：
 *   0: []
 *   1: []
 *   2: [3]
 *   3: [1]
 *   4: [0, 1]
 *   5: [2, 0]
 * 
 * Kahn算法过程：
 *   第1步：计算入度
 *     indegree[0]=2 (5→0, 4→0)
 *     indegree[1]=2 (3→1, 4→1)
 *     indegree[2]=1 (5→2)
 *     indegree[3]=1 (2→3)
 *     indegree[4]=0
 *     indegree[5]=0
 * 
 *   第2步：入度为0的顶点入队
 *     队列：[4, 5]
 * 
 *   第3步：取出4
 *     拓扑序列：[4]
 *     删除4的出边：4→0, 4→1
 *       indegree[0]-- → 1
 *       indegree[1]-- → 1
 * 
 *   第4步：取出5
 *     拓扑序列：[4, 5]
 *     删除5的出边：5→2, 5→0
 *       indegree[2]-- → 0 → 入队
 *       indegree[0]-- → 0 → 入队
 *     队列：[2, 0]
 * 
 *   第5步：取出2
 *     拓扑序列：[4, 5, 2]
 *     删除2的出边：2→3
 *       indegree[3]-- → 0 → 入队
 *     队列：[0, 3]
 * 
 *   第6步：取出0
 *     拓扑序列：[4, 5, 2, 0]
 *     0无边
 *     队列：[3]
 * 
 *   第7步：取出3
 *     拓扑序列：[4, 5, 2, 0, 3]
 *     删除3的出边：3→1
 *       indegree[1]-- → 0 → 入队
 *     队列：[1]
 * 
 *   第8步：取出1
 *     拓扑序列：[4, 5, 2, 0, 3, 1]
 *     顶点数=6，无环，排序完成
 * 
 * DFS算法过程（后序+反转）：
 *   从顶点0开始DFS，完成后将顶点加入栈
 *   栈（从底到顶）：[1, 3, 2, 0, 5, 4]
 *   反转后：[4, 5, 0, 2, 3, 1]
 * 
 *   得到另一个合法的拓扑序列：[4, 5, 0, 2, 3, 1]
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 拓扑排序（Kahn算法-BFS） ====================

/**
 * @brief Kahn算法拓扑排序
 * 
 * @param graph 图的邻接表
 * @return bool 成功返回true（无环），失败返回false（有环）
 * 
 * @note 时间复杂度 O(V+E)，空间复杂度 O(V)
 * @note 使用队列存储入度为0的顶点
 * @note 适用于有向无环图（DAG）
 */
bool TopologicalSort_Kahn(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<int> indegree(n, 0);
    vector<int> result;
    
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
    
    if (result.size() != n) {
        return false;
    }
    
    return true;
}

// ==================== 拓扑排序（DFS） ====================

/**
 * @brief DFS拓扑排序辅助函数
 * 
 * @param graph   图的邻接表
 * @param u       当前顶点
 * @param visited 访问状态（0-未访问，1-访问中，2-已访问完成）
 * @param result  拓扑序列（逆序存储）
 * @return bool   成功返回true（无环），失败返回false（有环）
 */
bool DFS_Topological(vector<vector<int>> &graph, int u, vector<int> &visited, vector<int> &result) {
    visited[u] = 1;
    
    for (int v : graph[u]) {
        if (visited[v] == 1) {
            return false;
        }
        if (visited[v] == 0) {
            if (!DFS_Topological(graph, v, visited, result)) {
                return false;
            }
        }
    }
    
    visited[u] = 2;
    result.push_back(u);
    return true;
}

/**
 * @brief DFS拓扑排序
 * 
 * @param graph 图的邻接表
 * 
 * @note 时间复杂度 O(V+E)，空间复杂度 O(V)
 * @note 使用递归DFS，检测环
 * @note 后序遍历 + 反转 = 拓扑序
 */
void TopologicalSort_DFS(vector<vector<int>> &graph) {
    int n = graph.size();
    vector<int> visited(n, 0);
    vector<int> result;
    
    for (int i = 0; i < n; i++) {
        if (visited[i] == 0) {
            if (!DFS_Topological(graph, i, visited, result)) {
                return;
            }
        }
    }
    
    reverse(result.begin(), result.end());
}