/*
 *（选择，最优子结构，贪心性质）。每一步选择当前最优解，期望通过局部最优达到全局最优。
 *
 * 贪心算法思路：
 * 1. 将问题分解为一系列子问题
 * 2. 在每一步选择当前最优的解
 * 3. 将选择加入解集
 * 4. 更新问题状态
 * 5. 重复直到问题解决
 *
 * 贪心算法	    问题描述	            贪心策略	        时间复杂度	是否最优
 * 活动选择	    选择最多互不冲突的活动	按结束时间排序选择	O(n log n)	是
 * 哈夫曼编码	构造最优前缀编码	选择频率最小的两个节点合并	O(n log n)	是
 * Prim算法	    最小生成树	        选择到已选集合最近的点	O(V²)	    是
 * Dijkstra算法	单源最短路径（无负权）	选择距离最近的未访问点	O(V²)	    是
 * 找零钱问题	用最少硬币找零	        优先使用大面额硬币	O(n)	    不一定（取决于货币体系）
 * 
 * 贪心算法示例：
 * 
 * 【活动选择】活动列表：(1,4),(3,5),(0,6),(5,7),(3,8),(5,9),(6,10),(8,11),(8,12),(2,13)
 *   按结束时间排序：(1,4),(3,5),(5,7),(0,6),(3,8),(5,9),(6,10),(8,11),(8,12),(2,13)
 *   选择活动1(1,4) → 结束时间=4
 *   活动2(3,5)开始3<4 → 跳过
 *   活动4(5,7)开始5>=4 → 选择，结束时间=7
 *   活动5(0,6)开始0<7 → 跳过
 *   活动6(3,8)开始3<7 → 跳过
 *   活动7(5,9)开始5<7 → 跳过
 *   活动8(6,10)开始6<7 → 跳过
 *   活动9(8,11)开始8>=7 → 选择，结束时间=11
 *   活动10(8,12)开始8<11 → 跳过
 *   最终选择：1,4,9
 * 
 * 【哈夫曼编码】字符频率：a:5, b:9, c:12, d:13, e:16, f:45
 *   合并a+b(14) → 合并c+d(25) → 合并e+14(30) → 合并25+30(55) → 合并45+55(100)
 *   编码：a:1100, b:1101, c:100, d:101, e:111, f:0
 * 
 * 【Prim算法】构建最小生成树
 *   从顶点0开始，逐步选择最近的顶点加入树
 *   最终生成树包含所有顶点，总权重最小
 * 
 * 【Dijkstra算法】单源最短路径（不含负权边）
 *   从起点0开始，逐步确定到各顶点的最短距离
 * 
 * 【找零钱问题】找零63元，硬币面额：25,10,5,1
 *   63/25=2张25元 → 剩余13元
 *   13/10=1张10元 → 剩余3元
 *   3/5=0张 → 剩余3元
 *   3/1=3张1元
 *   结果：2×25 + 1×10 + 3×1 = 63（共6枚硬币）
 */


using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 1. 活动选择问题 ====================

/**
 * @brief 活动结构体
 * 
 * @param start 开始时间
 * @param end   结束时间
 * @param index 活动编号
 */
struct Activity {
    int start;
    int end;
    int index;
};

/**
 * @brief 按结束时间升序排序
 */
bool CompareActivity(Activity a, Activity b) {
    return a.end < b.end;
}

/**
 * @brief 活动选择问题（贪心算法）
 * 
 * @param activities 活动列表
 * 
 * @note 时间复杂度 O(n log n)
 * @note 贪心策略：每次选择结束时间最早的活动
 * @note 要求活动互不冲突，即选择的活动时间区间不重叠
 */
void ActivitySelection(vector<Activity> activities) {
    sort(activities.begin(), activities.end(), CompareActivity);
    
    printf("选择的活动：");
    printf("%d ", activities[0].index);
    int lastEnd = activities[0].end;
    
    for (int i = 1; i < activities.size(); i++) {
        if (activities[i].start >= lastEnd) {
            printf("%d ", activities[i].index);
            lastEnd = activities[i].end;
        }
    }
    printf("\n");
}

// ==================== 2. 哈夫曼编码 ====================

/**
 * @brief 哈夫曼树节点
 * 
 * @param data  字符
 * @param freq  频率
 * @param left  左子树
 * @param right 右子树
 */
struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode *left;
    HuffmanNode *right;
    
    HuffmanNode(char d, int f) : data(d), freq(f), left(NULL), right(NULL) {}
};

/**
 * @brief 优先队列比较器（最小堆）
 */
struct CompareNode {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

/**
 * @brief 递归打印哈夫曼编码
 * 
 * @param root 根节点
 * @param code 当前编码
 */
void PrintHuffmanCodes(HuffmanNode* root, string code) {
    if (!root) return;
    if (root->data != '\0') {
        printf("%c: %s\n", root->data, code.c_str());
    }
    PrintHuffmanCodes(root->left, code + "0");
    PrintHuffmanCodes(root->right, code + "1");
}

/**
 * @brief 哈夫曼编码（贪心算法）
 * 
 * @param chars 字符数组
 * @param freqs 频率数组
 * 
 * @note 时间复杂度 O(n log n)
 * @note 贪心策略：每次选择频率最小的两个节点合并
 * @note 构造最优前缀编码
 */
void HuffmanCoding(vector<char> chars, vector<int> freqs) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNode> pq;
    
    for (int i = 0; i < chars.size(); i++) {
        pq.push(new HuffmanNode(chars[i], freqs[i]));
    }
    
    while (pq.size() > 1) {
        HuffmanNode *left = pq.top(); pq.pop();
        HuffmanNode *right = pq.top(); pq.pop();
        
        HuffmanNode *parent = new HuffmanNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    
    HuffmanNode *root = pq.top();
    printf("哈夫曼编码：\n");
    PrintHuffmanCodes(root, "");
}

// ==================== 3. 最小生成树（Prim算法） ====================

#define V 5

/**
 * @brief 选择未访问的最近顶点
 * 
 * @param key    距离数组
 * @param mstSet 已访问标记数组
 * @return int   最近顶点索引
 */
int MinKey(int key[], bool mstSet[]) {
    int min = 99999, minIndex;
    for (int v = 0; v < V; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            minIndex = v;
        }
    }
    return minIndex;
}

/**
 * @brief Prim算法求最小生成树
 * 
 * @param graph 图的邻接矩阵
 * @param start 起始顶点
 * 
 * @note 时间复杂度 O(V²)，空间复杂度 O(V)
 * @note 贪心策略：每次选择到已选集合最近的顶点
 * @note 适用于稠密图
 */
void PrimMST(int graph[V][V], int start) {
    int parent[V];
    int key[V];
    bool mstSet[V];
    
    for (int i = 0; i < V; i++) {
        key[i] = 99999;
        mstSet[i] = false;
    }
    
    key[start] = 0;        // 从指定顶点 start 开始
    parent[start] = -1;    // 根节点没有父节点
    
    for (int count = 0; count < V - 1; count++) {
        int u = MinKey(key, mstSet);
        mstSet[u] = true;
        
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }
    
    printf("最小生成树（Prim算法，起点%d）：\n", start);
    printf("边\t权重\n");
    for (int i = 0; i < V; i++) {
        if (i != start) {
            printf("%d-%d\t%d\n", parent[i], i, graph[i][parent[i]]);
        }
    }
}

// ==================== 4. 最短路径（Dijkstra算法） ====================

/**
 * @brief Dijkstra算法求单源最短路径（支持提前停止）
 * 
 * @param graph  图的邻接矩阵
 * @param src    源点（起点）
 * @param target 目标点（终点），传入 -1 表示计算所有顶点
 * 
 * @note 时间复杂度 O(V²)，空间复杂度 O(V)
 * @note 贪心策略：每次选择距离最近的未访问顶点
 * @note 不能处理负权边
 * @note 如果指定 target，找到后提前停止，提高效率
 */
void Dijkstra(int graph[V][V], int src, int target) {
    int dist[V];
    bool visited[V];
    
    for (int i = 0; i < V; i++) {
        dist[i] = 99999;
        visited[i] = false;
    }
    
    dist[src] = 0;
    
    for (int count = 0; count < V - 1; count++) {
        int u = MinKey(dist, visited);
        
        if (dist[u] == 99999) break;
        if (u == target) break;
        
        visited[u] = true;
        
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    
    if (target != -1) {
        printf("起点%d → 终点%d %s\n", src, target,
               dist[target] == 99999 ? "不可达" : to_string(dist[target]).c_str());
        return;
    }
    
    printf("起点%d\n顶点\t距离\n", src);
    for (int i = 0; i < V; i++) {
        printf("%d\t%s\n", i, dist[i] == 99999 ? "不可达" : to_string(dist[i]).c_str());
    }
}

// ==================== 5. 找零钱问题 ====================

/**
 * @brief 找零钱问题（贪心算法）
 * 
 * @param coins  硬币面额数组（降序排列）
 * @param amount 需要找零的金额
 * 
 * @note 时间复杂度 O(n)，空间复杂度 O(1)
 * @note 贪心策略：优先使用大面额硬币
 * @note 在标准硬币体系（如人民币）下可得到最优解
 * @note 并非所有货币体系都能通过贪心得到最优解
 */
void CoinChange(vector<int> coins, int amount) {
    sort(coins.begin(), coins.end(), greater<int>());
    
    printf("找零%d元：\n", amount);
    for (int i = 0; i < coins.size(); i++) {
        int count = amount / coins[i];
        if (count > 0) {
            printf("%d元 x %d张\n", coins[i], count);
            amount -= count * coins[i];
        }
    }
    if (amount > 0) {
        printf("剩余%d元无法找零\n", amount);
    }
}