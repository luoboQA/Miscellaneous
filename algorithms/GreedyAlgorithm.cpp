#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 1. 活动选择问题 ====================

struct Activity {
    int start;
    int end;
    int index;
};

bool CompareActivity(Activity a, Activity b) {
    return a.end < b.end;
}

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

struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode *left;
    HuffmanNode *right;
    
    HuffmanNode(char d, int f) : data(d), freq(f), left(NULL), right(NULL) {}
};

struct CompareNode {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

void PrintHuffmanCodes(HuffmanNode* root, string code) {
    if (!root) return;
    if (root->data != '\0') {
        printf("%c: %s\n", root->data, code.c_str());
    }
    PrintHuffmanCodes(root->left, code + "0");
    PrintHuffmanCodes(root->right, code + "1");
}

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

void PrimMST(int graph[V][V]) {
    int parent[V];
    int key[V];
    bool mstSet[V];
    
    for (int i = 0; i < V; i++) {
        key[i] = 99999;
        mstSet[i] = false;
    }
    
    key[0] = 0;
    parent[0] = -1;
    
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
    
    printf("最小生成树（Prim算法）：\n");
    printf("边\t权重\n");
    for (int i = 1; i < V; i++) {
        printf("%d-%d\t%d\n", parent[i], i, graph[i][parent[i]]);
    }
}

// ==================== 4. 最短路径（Dijkstra算法） ====================

void Dijkstra(int graph[V][V], int src) {
    int dist[V];
    bool visited[V];
    
    for (int i = 0; i < V; i++) {
        dist[i] = 99999;
        visited[i] = false;
    }
    
    dist[src] = 0;
    
    for (int count = 0; count < V - 1; count++) {
        int u = MinKey(dist, visited);
        visited[u] = true;
        
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != 99999 &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    
    printf("最短路径（Dijkstra算法，起点0）：\n");
    printf("顶点\t距离\n");
    for (int i = 0; i < V; i++) {
        printf("%d\t%d\n", i, dist[i]);
    }
}

// ==================== 5. 找零钱问题 ====================

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

int main() {
    printf("========================================\n");
    printf("    贪心法（Greedy Algorithm）\n");
    printf("========================================\n");
    printf("思想：每一步选择当前最优解\n");
    printf("适用：贪心选择性质 + 最优子结构\n\n");
    
    // 1. 活动选择问题
    printf("【1. 活动选择】\n");
    vector<Activity> activities = {
        {1, 4, 1}, {3, 5, 2}, {0, 6, 3}, {5, 7, 4}, {3, 8, 5},
        {5, 9, 6}, {6, 10, 7}, {8, 11, 8}, {8, 12, 9}, {2, 13, 10}
    };
    ActivitySelection(activities);
    printf("\n");
    
    // 2. 哈夫曼编码
    printf("【2. 哈夫曼编码】\n");
    vector<char> chars = {'a', 'b', 'c', 'd', 'e', 'f'};
    vector<int> freqs = {5, 9, 12, 13, 16, 45};
    HuffmanCoding(chars, freqs);
    printf("\n");
    
    // 3. 最小生成树
    printf("【3. 最小生成树】\n");
    int graph[V][V] = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}
    };
    PrimMST(graph);
    printf("\n");
    
    // 4. 最短路径
    printf("【4. 最短路径】\n");
    Dijkstra(graph, 0);
    printf("\n");
    
    // 5. 找零钱问题
    printf("【5. 找零钱问题】\n");
    vector<int> coins = {25, 10, 5, 1};
    CoinChange(coins, 63);
    
    getchar();
    return 0;
}