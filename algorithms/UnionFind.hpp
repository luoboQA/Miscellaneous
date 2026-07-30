/*
 *（父节点，路径压缩，按秩合并）。通过树形结构维护不相交集合，支持高效的合并和查询操作。
 *
 * 并查集思路：
 * 1. 每个元素初始时自成一个集合，父节点指向自己
 * 2. 查找：沿着父节点链找到根节点，路径压缩优化
 * 3. 合并：将两个集合的根节点相连，按秩合并保持树平衡
 * 4. 判断连通：两个元素是否在同一集合中
 *
 * 数据结构	操作	    时间复杂度	空间复杂度	适用场景
 * 并查集	查找	    O(α(n))≈O(1)	O(n)	动态连通性、最小生成树
 *         合并	    O(α(n))≈O(1)	O(n)	
 *         判断连通	O(α(n))≈O(1)	O(n)	
 * 
 * 并查集示例（10个元素，初始状态）：
 *   parent: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
 *   每个元素自成一个集合，连通分量数=10
 * 
 * 合并操作 Union(2, 3)：
 *   root2=2, root3=3
 *   2和3秩相同，将3指向2
 *   parent: [0, 1, 2, 2, 4, 5, 6, 7, 8, 9]
 *   集合{2,3}
 * 
 * 合并操作 Union(4, 5)：
 *   root4=4, root5=5
 *   将5指向4
 *   parent: [0, 1, 2, 2, 4, 4, 6, 7, 8, 9]
 *   集合{4,5}
 * 
 * 合并操作 Union(5, 6)：
 *   root5=4, root6=6
 *   将6指向4
 *   parent: [0, 1, 2, 2, 4, 4, 4, 7, 8, 9]
 *   集合{4,5,6}
 * 
 * 合并操作 Union(7, 8)：
 *   parent: [0, 1, 2, 2, 4, 4, 4, 7, 7, 9]
 * 
 * 合并操作 Union(8, 9)：
 *   root8=7, root9=9
 *   将9指向7
 *   parent: [0, 1, 2, 2, 4, 4, 4, 7, 7, 7]
 *   集合{7,8,9}
 * 
 * 查找 Find(5)（路径压缩）：
 *   root5=5 → parent[5]=4 → parent[4]=4 → 根=4
 *   将5直接指向4
 * 
 * 查找 Find(9)（路径压缩）：
 *   root9=9 → parent[9]=7 → parent[7]=7 → 根=7
 *   将9直接指向7
 * 
 * 连通分量数 = 5
 *   集合：{0}, {1}, {2,3}, {4,5,6}, {7,8,9}
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 并查集 ====================

/**
 * @brief 并查集类
 * 
 * @param parent 父节点数组
 * @param rank   秩（树高）
 * @param size   集合大小
 * @param count  连通分量个数
 */
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
    vector<int> size;
    int count;

public:
    /**
     * @brief 构造函数
     * 
     * @param n 元素个数
     */
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        size.resize(n, 1);
        count = n;
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    /**
     * @brief 查找根节点（递归路径压缩）
     * 
     * @param x 元素
     * @return int 根节点
     * 
     * @note 时间复杂度 O(α(n)) ≈ O(1)
     * @note 路径压缩使树更扁平
     */
    int Find(int x) {
        if (parent[x] != x) {
            parent[x] = Find(parent[x]);
        }
        return parent[x];
    }
    
    /**
     * @brief 查找根节点（迭代版）
     * 
     * @param x 元素
     * @return int 根节点
     * 
     * @note 避免递归栈溢出
     */
    int Find_Iterative(int x) {
        int root = x;
        while (parent[root] != root) {
            root = parent[root];
        }
        while (parent[x] != x) {
            int next = parent[x];
            parent[x] = root;
            x = next;
        }
        return root;
    }
    
    /**
     * @brief 合并两个集合
     * 
     * @param x 元素x
     * @param y 元素y
     * 
     * @note 按秩合并，将矮树合并到高树下
     * @note 时间复杂度 O(α(n)) ≈ O(1)
     */
    void Union(int x, int y) {
        int rootX = Find(x);
        int rootY = Find(y);
        
        if (rootX == rootY) return;
        
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
            size[rootX] += size[rootY];
        }
        count--;
    }
    
    /**
     * @brief 判断是否连通
     * 
     * @param x 元素x
     * @param y 元素y
     * @return true  连通
     * @return false 不连通
     */
    bool Connected(int x, int y) {
        return Find(x) == Find(y);
    }
    
    /**
     * @brief 获取集合大小
     * 
     * @param x 元素
     * @return int 集合大小
     */
    int GetSize(int x) {
        return size[Find(x)];
    }
    
    /**
     * @brief 获取连通分量个数
     * 
     * @return int 连通分量个数
     */
    int GetCount() {
        return count;
    }
    
    /**
     * @brief 打印父节点数组
     */
    void Print() {
        for (int i = 0; i < parent.size(); i++) {
            printf("%d ", parent[i]);
        }
        printf("\n");
    }
};