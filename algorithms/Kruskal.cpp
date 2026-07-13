#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== Kruskal算法（并查集实现） ====================

// 边结构体
typedef struct Edge {
    int u, v;
    int weight;
    Edge(int u, int v, int w) : u(u), v(v), weight(w) {}
} Edge;

// 按权重排序
bool CompareEdge(Edge a, Edge b) {
    return a.weight < b.weight;
}

// 并查集
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

// Kruskal算法
vector<Edge> KruskalMST(vector<Edge> &edges, int n) {
    vector<Edge> result;
    UnionFind uf(n);
    
    // 按权重排序
    sort(edges.begin(), edges.end(), CompareEdge);
    
    int mstWeight = 0;
    int edgeCount = 0;
    
    printf("边排序结果：\n");
    for (Edge &e : edges) {
        printf("(%d-%d) weight=%d\n", e.u, e.v, e.weight);
    }
    printf("\n");
    
    printf("构建最小生成树：\n");
    for (Edge &e : edges) {
        if (uf.Union(e.u, e.v)) {
            result.push_back(e);
            mstWeight += e.weight;
            edgeCount++;
            printf("选择边 (%d-%d) weight=%d\n", e.u, e.v, e.weight);
            
            if (edgeCount == n - 1) break;
        } else {
            printf("跳过边 (%d-%d) 形成环\n", e.u, e.v);
        }
    }
    
    printf("\n最小生成树总权重：%d\n", mstWeight);
    return result;
}

int main() {
    printf("========================================\n");
    printf("    Kruskal最小生成树\n");
    printf("========================================\n");
    printf("时间复杂度：O(ElogE)\n");
    printf("空间复杂度：O(V+E)\n");
    printf("适用：稀疏图\n\n");
    
    int n = 6;
    vector<Edge> edges = {
        Edge(0, 1, 4),
        Edge(0, 2, 3),
        Edge(1, 2, 1),
        Edge(1, 3, 2),
        Edge(2, 3, 4),
        Edge(3, 4, 2),
        Edge(4, 5, 6),
        Edge(3, 5, 5)
    };
    
    printf("顶点数：%d\n", n);
    printf("边数：%d\n\n", (int)edges.size());
    
    vector<Edge> mst = KruskalMST(edges, n);
    
    printf("\n最小生成树边集：\n");
    for (Edge &e : mst) {
        printf("(%d, %d) weight=%d\n", e.u, e.v, e.weight);
    }
    
    printf("\n【Prim vs Kruskal对比】\n");
    printf("Prim算法：适合稠密图 O(V^2)\n");
    printf("Kruskal算法：适合稀疏图 O(ElogE)\n");
    
    getchar();
    return 0;
}