/*
 *（优先队列，上界，剪枝）。通过优先队列扩展节点，利用上界剪枝，在搜索树中快速找到最优解。
 *
 * 分支限界法思路（以0/1背包为例）：
 * 1. 按单位价值将物品排序
 * 2. 创建根节点（level=-1, profit=0, weight=0）
 * 3. 计算根节点的上界 bound
 * 4. 将根节点加入优先队列（按上界降序）
 * 5. 取出队列中上界最大的节点：
 *    - 若上界 ≤ 当前最优解，剪枝
 *    - 若到达叶子节点，更新最优解
 *    - 否则生成左右子节点：
 *      - 左子节点：选择当前物品（若不超过容量）
 *      - 右子节点：不选择当前物品
 * 6. 计算子节点的上界，若 > 当前最优解则入队
 * 7. 重复步骤5-6，直到队列为空
 *
 * 搜索算法	平均时间复杂度	最差时间复杂度	空间复杂度	数据对象稳定性
 * 分支限界法	O(2^n)（有剪枝）	O(2^n)	        O(2^n)	    求最优解
 * 示例：3个物品，背包容量50
 *   物品0：重量10，价值60，单位价值6.0
 *   物品1：重量20，价值100，单位价值5.0
 *   物品2：重量30，价值120，单位价值4.0
 * 
 * 按单位价值排序后（已排序）：
 *   物品0：重量10，价值60，单位价值6.0
 *   物品1：重量20，价值100，单位价值5.0
 *   物品2：重量30，价值120，单位价值4.0
 * 
 * 根节点（level=-1, profit=0, weight=0）
 *   上界 = 0 + 60 + 100 + (50-30)*4 = 160 + 80 = 240
 *   队列：[根节点(240)]
 * 
 * 第1步：取出根节点（level=-1）
 *   生成左子节点（选择物品0）：
 *     level=0, profit=60, weight=10
 *     上界 = 60 + 100 + (50-30)*4 = 160 + 80 = 240
 *     入队
 *   生成右子节点（不选物品0）：
 *     level=0, profit=0, weight=0
 *     上界 = 0 + 100 + (50-20)*4 = 100 + 120 = 220
 *     入队
 *   队列：[左(240), 右(220)]
 * 
 * 第2步：取出左节点（level=0, 选择物品0）
 *   生成左子节点（选择物品1）：
 *     level=1, profit=160, weight=30
 *     上界 = 160 + (50-30)*4 = 160 + 80 = 240
 *     入队
 *   生成右子节点（不选物品1）：
 *     level=1, profit=60, weight=10
 *     上界 = 60 + (50-10)*4 = 60 + 160 = 220
 *     入队
 *   队列：[左左(240), 左右(220), 右(220)]
 * 
 * 第3步：取出左左节点（level=1, 选择物品0和1）
 *   生成左子节点（选择物品2）：
 *     level=2, profit=280, weight=60
 *     重量60 > 容量50 → 不可行，不生成
 *   生成右子节点（不选物品2）：
 *     level=2, profit=160, weight=30
 *     上界 = 160（叶子节点）
 *     入队，更新最优解 = 160
 *   队列：[左右(220), 右(220), 左右右(160)]
 * 
 * 第4步：取出左右节点（level=1, 选择物品0不选物品1）
 *   上界 = 220 > 当前最优解160
 *   生成左子节点（选择物品2）：
 *     level=2, profit=180, weight=40
 *     上界 = 180（叶子节点）
 *     入队，更新最优解 = 180
 *   生成右子节点（不选物品2）：
 *     level=2, profit=60, weight=10
 *     上界 = 60（叶子节点）
 *   队列：[右(220), 左右左(180), 左右右(60), ...]
 * 
 * 第5步：取出右节点（level=0, 不选物品0）
 *   上界 = 220 > 当前最优解180
 *   生成左子节点（选择物品1）：
 *     level=1, profit=100, weight=20
 *     上界 = 100 + (50-20)*4 = 100 + 120 = 220
 *     入队
 *   生成右子节点（不选物品1）：
 *     level=1, profit=0, weight=0
 *     上界 = 0 + (50-0)*4 = 200
 *     入队
 *   队列：[右左(220), 右右(200), 左右左(180), ...]
 * 
 * 第6步：取出右左节点（level=1, 不选物品0选物品1）
 *   生成左子节点（选择物品2）：
 *     level=2, profit=220, weight=50
 *     上界 = 220（叶子节点）
 *     入队，更新最优解 = 220
 *   生成右子节点（不选物品2）：
 *     level=2, profit=100, weight=20
 *     上界 = 100（叶子节点）
 *   队列：[右右(200), 左右左(180), 右左左(220), ...]
 * 
 * 后续节点上界都 ≤ 220，最优解保持220
 * 
 * 最终最优解：220
 *   选择物品1（重量20，价值100）和物品2（重量30，价值120）
 *   总重量50，总价值220
 */
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 分支限界法-0/1背包 ====================

/**
 * @brief 物品结构体
 * 
 * @param weight 物品重量
 * @param value  物品价值
 * @param ratio  单位价值（value/weight）
 */
struct Item {
    int weight;
    int value;
    double ratio;
};

/**
 * @brief 搜索树节点结构体
 * 
 * @param level  当前层（对应物品索引）
 * @param profit 当前已选物品的总价值
 * @param weight 当前已选物品的总重量
 * @param bound  上界（当前节点的最大可能价值）
 * @param items  选择情况数组（1表示选择，0表示不选）
 */
struct Node {
    int level;
    int profit;
    int weight;
    double bound;
    int *items;
    
    Node(int lvl = 0, int pro = 0, int w = 0, int n = 0) {
        level = lvl;
        profit = pro;
        weight = w;
        bound = 0;
        items = new int[n];
        for (int i = 0; i < n; i++) items[i] = 0;
    }
    
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

/**
 * @brief 按单位价值降序排序
 * 
 * @param a 物品a
 * @param b 物品b
 * @return true  a的单位价值大于b
 * @return false a的单位价值小于等于b
 */
bool CompareItem(Item a, Item b) {
    return a.ratio > b.ratio;
}

/**
 * @brief 计算节点的上界
 * 
 * @param u        当前节点
 * @param n        物品总数
 * @param capacity 背包容量
 * @param items    物品数组
 * @return double  上界值
 * 
 * @note 贪心策略：先按单位价值从高到低装入，最后一个物品按比例装入
 * @note 上界是当前节点及其子节点可能达到的最大价值
 */
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

/**
 * @brief 分支限界法求解0/1背包
 * 
 * @param items    物品列表（引用传递，会被排序）
 * @param capacity 背包容量
 * @return int     最大总价值
 * 
 * @note 使用优先队列（最大堆）按上界降序扩展节点
 * @note 剪枝条件：节点上界 ≤ 当前最优解
 * @note 时间复杂度：最坏 O(2^n)，空间复杂度：O(2^n)
 * @note 适用于求解最优解问题，通过剪枝大幅提高搜索效率
 */
int BranchAndBound(vector<Item> &items, int capacity) {
    int n = items.size();
    
    sort(items.begin(), items.end(), CompareItem);
    
    auto cmp = [](Node *a, Node *b) { return a->bound < b->bound; };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> pq(cmp);
    
    Node *root = new Node(-1, 0, 0, n);
    root->bound = Bound(*root, n, capacity, items);
    pq.push(root);
    
    int maxProfit = 0;
    
    while (!pq.empty()) {
        Node *u = pq.top();
        pq.pop();
        
        // 剪枝
        if (u->bound <= maxProfit) {
            delete u;
            continue;
        }
        
        // 叶子节点
        if (u->level == n - 1) {
            if (u->profit > maxProfit) {
                maxProfit = u->profit;
            }
            delete u;
            continue;
        }
        
        int nextLevel = u->level + 1;
        
        // 左子节点：选
        if (u->weight + items[nextLevel].weight <= capacity) {
            Node *left = new Node(*u, n);
            left->level = nextLevel;
            left->weight = u->weight + items[nextLevel].weight;
            left->profit = u->profit + items[nextLevel].value;
            left->items[nextLevel] = 1;
            left->bound = Bound(*left, n, capacity, items);
            
            if (left->bound > maxProfit) {
                pq.push(left);
            } else {
                delete left;
            }
        }
        
        // 右子节点：不选
        Node *right = new Node(*u, n);
        right->level = nextLevel;
        right->bound = Bound(*right, n, capacity, items);
        
        if (right->bound > maxProfit) {
            pq.push(right);
        } else {
            delete right;
        }
        
        delete u;
    }
    
    return maxProfit;
}