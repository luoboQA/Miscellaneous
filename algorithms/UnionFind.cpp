#include <iostream>
#include <vector>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 并查集 ====================

class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;      // 按秩合并
    vector<int> size;      // 集合大小
    int count;             // 连通分量个数

public:
    // 初始化
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        size.resize(n, 1);
        count = n;
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    // 查找（路径压缩）
    int Find(int x) {
        if (parent[x] != x) {
            parent[x] = Find(parent[x]);  // 递归路径压缩
        }
        return parent[x];
    }
    
    // 查找（迭代版）
    int Find_Iterative(int x) {
        int root = x;
        while (parent[root] != root) {
            root = parent[root];
        }
        // 路径压缩
        while (parent[x] != x) {
            int next = parent[x];
            parent[x] = root;
            x = next;
        }
        return root;
    }
    
    // 合并（按秩合并）
    void Union(int x, int y) {
        int rootX = Find(x);
        int rootY = Find(y);
        
        if (rootX == rootY) return;
        
        // 按秩合并
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
    
    // 判断是否连通
    bool Connected(int x, int y) {
        return Find(x) == Find(y);
    }
    
    // 获取集合大小
    int GetSize(int x) {
        return size[Find(x)];
    }
    
    // 获取连通分量个数
    int GetCount() {
        return count;
    }
    
    // 打印集合
    void Print() {
        for (int i = 0; i < parent.size(); i++) {
            printf("%d ", parent[i]);
        }
        printf("\n");
    }
};

int main() {
    printf("========================================\n");
    printf("    并查集（Union-Find）\n");
    printf("========================================\n");
    printf("时间复杂度：O(α(n)) ≈ O(1)\n");
    printf("空间复杂度：O(n)\n\n");
    
    UnionFind uf(10);
    
    printf("初始状态：");
    uf.Print();
    printf("连通分量数：%d\n\n", uf.GetCount());
    
    // 合并操作
    uf.Union(2, 3);
    uf.Union(4, 5);
    uf.Union(5, 6);
    uf.Union(7, 8);
    uf.Union(8, 9);
    
    printf("合并后状态：");
    uf.Print();
    printf("连通分量数：%d\n\n", uf.GetCount());
    
    // 查询操作
    printf("查询结果：\n");
    printf("2和3是否连通：%s\n", uf.Connected(2, 3) ? "是" : "否");
    printf("4和6是否连通：%s\n", uf.Connected(4, 6) ? "是" : "否");
    printf("2和5是否连通：%s\n", uf.Connected(2, 5) ? "是" : "否");
    printf("集合{4,5,6}的大小：%d\n", uf.GetSize(4));
    
    getchar();
    return 0;
}