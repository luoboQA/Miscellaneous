/*
 *（并查集，最小生成树，排序）。按权重从小到大选择边，利用并查集检测环，直到选出 V-1 条边。
 *
 * Kruskal算法思路：
 * 1. 将所有边按权重从小到大排序
 * 2. 从最小边开始，依次考虑每条边
 * 3. 若该边连接的两个顶点不在同一连通分量中，则选择该边（加入最小生成树）
 * 4. 否则跳过该边（会形成环）
 * 5. 重复步骤2-4，直到选出 V-1 条边
 *
 * 图算法	平均时间复杂度	空间复杂度	适用场景
 * Kruskal	O(E log E)	O(V + E)	稀疏图
 * 
 * 图示例（6个顶点）：
 * 边集：(0-1,4), (0-2,3), (1-2,1), (1-3,2), (2-3,4), (3-4,2), (4-5,6), (3-5,5)
 * 
 * 按权重排序：
 *   (1-2,1) ← 最小
 *   (1-3,2)
 *   (3-4,2)
 *   (0-2,3)
 *   (0-1,4)
 *   (2-3,4)
 *   (3-5,5)
 *   (4-5,6)
 * 
 * 并查集初始状态：{0}, {1}, {2}, {3}, {4}, {5}
 * 
 * 第1步：选择 (1-2,1)
 *   1和2合并 → {1,2}
 *   已选边数：1
 * 
 * 第2步：选择 (1-3,2)
 *   1和3合并 → {1,2,3}
 *   已选边数：2
 * 
 * 第3步：选择 (3-4,2)
 *   3和4合并 → {1,2,3,4}
 *   已选边数：3
 * 
 * 第4步：选择 (0-2,3)
 *   0和2合并 → {0,1,2,3,4}
 *   已选边数：4
 * 
 * 第5步：考虑 (0-1,4)
 *   0和1已在同一集合 → 跳过，会形成环
 * 
 * 第6步：考虑 (2-3,4)
 *   2和3已在同一集合 → 跳过，会形成环
 * 
 * 第7步：选择 (3-5,5)
 *   3和5合并 → {0,1,2,3,4,5}
 *   已选边数：5 = V-1 → 完成
 * 
 * 最小生成树边集：(1-2,1), (1-3,2), (3-4,2), (0-2,3), (3-5,5)
 * 总权重：1+2+2+3+5 = 13
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== Kruskal算法（并查集实现） ====================

/**
 * @brief 边结构体
 * 
 * @param u      边的起点
 * @param v      边的终点
 * @param weight 边的权重
 */
typedef struct Edge {
    int u, v;
    int weight;
    Edge(int u, int v, int w) : u(u), v(v), weight(w) {}
} Edge;

/**
 * @brief 按权重升序排序
 * 
 * @param a 边a
 * @param b 边b
 * @return true  a的权重小于b
 * @return false a的权重大于等于b
 */
bool CompareEdge(Edge a, Edge b) {
    return a.weight < b.weight;
}

/**
 * @brief 并查集类
 * 
 * @param parent 父节点数组
 * @param rank   秩（树高）
 */
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
    
public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int Find(int x) {
        if (parent[x] != x) {
            parent[x] = Find(parent[x]);
        }
        return parent[x];
    }
    
    bool Union(int x, int y) {
        int rootX = Find(x);
        int rootY = Find(y);
        if (rootX == rootY) return false;
        
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        return true;
    }
};

/**
 * @brief Kruskal算法求最小生成树
 * 
 * @param edges 边集
 * @param n     顶点数
 * @return vector<Edge> 最小生成树的边集
 * 
 * @note 时间复杂度 O(E log E)，空间复杂度 O(V + E)
 * @note 适用于稀疏图
 * @note 使用并查集检测环
 */
vector<Edge> KruskalMST(vector<Edge> &edges, int n) {
    vector<Edge> result;
    UnionFind uf(n);
    
    sort(edges.begin(), edges.end(), CompareEdge);
    
    int mstWeight = 0;
    int edgeCount = 0;
    
    for (Edge &e : edges) {
        if (uf.Union(e.u, e.v)) {
            result.push_back(e);
            mstWeight += e.weight;
            edgeCount++;
            
            if (edgeCount == n - 1) break;
        }
    }
    
    return result;
}