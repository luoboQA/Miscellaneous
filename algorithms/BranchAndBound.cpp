#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 分支限界法-0/1背包 ====================

struct Item {
    int weight;
    int value;
    double ratio;  // 单位价值
};

struct Node {
    int level;        // 当前层
    int profit;       // 当前价值
    int weight;       // 当前重量
    double bound;     // 上界
    int *items;       // 选择情况
    
    Node(int lvl = 0, int pro = 0, int w = 0, int n = 0) {
        level = lvl;
        profit = pro;
        weight = w;
        bound = 0;
        items = new int[n];
        for (int i = 0; i < n; i++) items[i] = 0;
    }
    
    // 复制构造
    Node(const Node &other, int n) {
        level = other.level;
        profit = other.profit;
        weight = other.weight;
        bound = other.bound;
        items = new int[n];
        for (int i = 0; i < n; i++) items[i] = other.items[i];
    }
    
    ~Node() {
        delete[] items;
    }
};

// 按单位价值排序
bool CompareItem(Item a, Item b) {
    return a.ratio > b.ratio;
}

// 计算上界
double Bound(Node u, int n, int capacity, vector<Item> &items) {
    if (u.weight >= capacity) return 0;
    
    double profitBound = u.profit;
    int j = u.level + 1;
    int totalWeight = u.weight;
    
    while (j < n && totalWeight + items[j].weight <= capacity) {
        totalWeight += items[j].weight;
        profitBound += items[j].value;
        j++;
    }
    
    if (j < n) {
        profitBound += (capacity - totalWeight) * items[j].ratio;
    }
    
    return profitBound;
}

// 分支限界法求解0/1背包
int BranchAndBound(vector<Item> &items, int capacity) {
    int n = items.size();
    
    // 按单位价值排序
    sort(items.begin(), items.end(), CompareItem);
    
    // 优先队列（按上界降序）
    auto cmp = [](Node *a, Node *b) { return a->bound < b->bound; };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> pq(cmp);
    
    // 根节点
    Node *root = new Node(-1, 0, 0, n);
    root->bound = Bound(*root, n, capacity, items);
    pq.push(root);
    
    int maxProfit = 0;
    Node *bestNode = nullptr;
    
    printf("分支限界搜索过程：\n");
    printf("节点\t价值\t重量\t上界\t操作\n");
    
    while (!pq.empty()) {
        Node *u = pq.top();
        pq.pop();
        
        printf("(%d)\t%d\t%d\t%.0f\t", u->level, u->profit, u->weight, u->bound);
        
        // 剪枝
        if (u->bound <= maxProfit) {
            printf("剪枝\n");
            continue;
        }
        
        // 到达叶子节点
        if (u->level == n - 1) {
            if (u->profit > maxProfit) {
                maxProfit = u->profit;
                bestNode = u;
                printf("更新最优解=%d\n", maxProfit);
            }
            continue;
        }
        
        int nextLevel = u->level + 1;
        
        // 左子树：选择当前物品
        if (u->weight + items[nextLevel].weight <= capacity) {
            Node *left = new Node(*u, n);
            left->level = nextLevel;
            left->weight = u->weight + items[nextLevel].weight;
            left->profit = u->profit + items[nextLevel].value;
            left->items[nextLevel] = 1;
            left->bound = Bound(*left, n, capacity, items);
            
            if (left->bound > maxProfit) {
                pq.push(left);
                printf("左分支(选%d)\n", nextLevel);
            } else {
                printf("左分支(选%d)剪枝\n", nextLevel);
            }
        }
        
        // 右子树：不选择当前物品
        Node *right = new Node(*u, n);
        right->level = nextLevel;
        right->bound = Bound(*right, n, capacity, items);
        
        if (right->bound > maxProfit) {
            pq.push(right);
            printf("右分支(不选%d)\n", nextLevel);
        } else {
            printf("右分支(不选%d)剪枝\n", nextLevel);
        }
    }
    
    // 输出最优解
    if (bestNode) {
        printf("\n最优解：\n");
        printf("总价值：%d\n", maxProfit);
        printf("选择物品：");
        for (int i = 0; i < n; i++) {
            if (bestNode->items[i]) {
                printf("%d(重量%d,价值%d) ", i, items[i].weight, items[i].value);
            }
        }
        printf("\n");
    }
    
    return maxProfit;
}

int main() {
    printf("========================================\n");
    printf("    分支限界法-0/1背包问题\n");
    printf("========================================\n");
    printf("时间复杂度：最坏O(2^n)\n");
    printf("空间复杂度：O(2^n)\n");
    printf("适用：求最优解，剪枝提高效率\n\n");
    
    vector<Item> items = {
        {10, 60, 6.0},
        {20, 100, 5.0},
        {30, 120, 4.0}
    };
    int capacity = 50;
    
    printf("物品列表：\n");
    for (int i = 0; i < items.size(); i++) {
        printf("%d: 重量%d, 价值%d, 单位价值%.2f\n", 
               i, items[i].weight, items[i].value, items[i].ratio);
    }
    printf("背包容量：%d\n\n", capacity);
    
    int result = BranchAndBound(items, capacity);
    
    printf("\n========================================\n");
    printf("【回溯法 vs 分支限界法对比】\n");
    printf("回溯法：深度优先，找所有解\n");
    printf("分支限界法：广度优先/最佳优先，找最优解\n");
    printf("分支限界法通过上界剪枝，效率更高\n");
    
    getchar();
    return 0;
}