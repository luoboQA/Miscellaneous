/*
 *（栈，递归，邻接矩阵）。通过深度优先策略，沿着一个分支一直向下搜索，直到无法继续再回溯。
 *
 * 深度优先搜索（DFS）思路：
 * 1. 从起始顶点开始，标记为已访问并输出
 * 2. 递归地访问当前顶点的一个未被访问的邻接点
 * 3. 沿着该分支一直向下搜索，直到没有未访问的邻接点
 * 4. 回溯到上一个顶点，继续搜索其他分支
 * 5. 重复步骤2-4，直到所有可达顶点都被访问
 *
 * 搜索算法	数据结构	遍历时间复杂度	空间复杂度
 * DFS深度优先搜索	邻接矩阵	O(|V|²)	O(|V|²)
 *                 邻接链表	O(|V|+|E|)	O(|V|+|E|)
 * 图示例（无向图）：5个顶点（0,1,2,3,4），边：0-1, 0-2, 0-3, 1-4, 2-4, 3-4
 * 
 * 邻接矩阵：
 *       0  1  2  3  4
 *   0  [0, 1, 1, 1, 0]
 *   1  [1, 0, 0, 0, 1]
 *   2  [1, 0, 0, 0, 1]
 *   3  [1, 0, 0, 0, 1]
 *   4  [0, 1, 1, 1, 0]
 * 
 * DFS递归过程（从顶点0开始，按邻接点从小到大顺序）：
 * 
 *   第1步：访问0，标记visited[0]=1
 *     输出：0
 *     从邻接矩阵第0行找第一个未访问邻接点：
 *       j=1：arcs[0][1]=1 且 visited[1]=0 → 递归访问1
 * 
 *   第2步：访问1，标记visited[1]=1
 *     输出：0 1
 *     从邻接矩阵第1行找第一个未访问邻接点：
 *       j=0：arcs[1][0]=1 但 visited[0]=1 → 跳过
 *       j=4：arcs[1][4]=1 且 visited[4]=0 → 递归访问4
 * 
 *   第3步：访问4，标记visited[4]=1
 *     输出：0 1 4
 *     从邻接矩阵第4行找第一个未访问邻接点：
 *       j=0：arcs[4][0]=1 但 visited[0]=1 → 跳过
 *       j=1：arcs[4][1]=1 但 visited[1]=1 → 跳过
 *       j=2：arcs[4][2]=1 且 visited[2]=0 → 递归访问2
 * 
 *   第4步：访问2，标记visited[2]=1
 *     输出：0 1 4 2
 *     从邻接矩阵第2行找第一个未访问邻接点：
 *       j=0：arcs[2][0]=1 但 visited[0]=1 → 跳过
 *       j=4：arcs[2][4]=1 但 visited[4]=1 → 跳过
 *       没有未访问邻接点 → 回溯到顶点4
 * 
 *   第5步：回溯到顶点4
 *     从邻接矩阵第4行继续找下一个未访问邻接点：
 *       j=3：arcs[4][3]=1 且 visited[3]=0 → 递归访问3
 * 
 *   第6步：访问3，标记visited[3]=1
 *     输出：0 1 4 2 3
 *     从邻接矩阵第3行找第一个未访问邻接点：
 *       j=0：arcs[3][0]=1 但 visited[0]=1 → 跳过
 *       j=4：arcs[3][4]=1 但 visited[4]=1 → 跳过
 *       没有未访问邻接点 → 回溯到顶点4
 * 
 *   第7步：回溯到顶点4 → 继续回溯到顶点1 → 继续回溯到顶点0
 *     所有顶点已访问，结束
 * 
 * 最终DFS序列（递归）：0 1 4 2 3
 * 
 * DFS非递归过程（使用栈，从顶点0开始）：
 * 
 *   第1步：访问0，标记visited[0]=1，入栈 → 栈：[0]
 *     输出：0
 * 
 *   第2步：栈顶0，找第一个未访问邻接点：1 → 访问1，入栈 → 栈：[0,1]
 *     输出：0 1
 * 
 *   第3步：栈顶1，找第一个未访问邻接点：4 → 访问4，入栈 → 栈：[0,1,4]
 *     输出：0 1 4
 * 
 *   第4步：栈顶4，找第一个未访问邻接点：2 → 访问2，入栈 → 栈：[0,1,4,2]
 *     输出：0 1 4 2
 * 
 *   第5步：栈顶2，无未访问邻接点 → 出栈 → 栈：[0,1,4]
 * 
 *   第6步：栈顶4，继续找下一个未访问邻接点：3 → 访问3，入栈 → 栈：[0,1,4,3]
 *     输出：0 1 4 2 3
 * 
 *   第7步：栈顶3，无未访问邻接点 → 出栈 → 栈：[0,1,4]
 *   第8步：栈顶4，无未访问邻接点 → 出栈 → 栈：[0,1]
 *   第9步：栈顶1，无未访问邻接点 → 出栈 → 栈：[0]
 *   第10步：栈顶0，无未访问邻接点 → 出栈 → 栈：[]
 * 
 * 最终DFS序列（非递归）：0 1 4 2 3
 * 
 * DFS与BFS对比：
 *   DFS（深度优先）：沿着一条路走到黑，使用栈（递归），适合寻找路径
 *   BFS（广度优先）：逐层扩展，使用队列，适合寻找最短路径
 */


using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define MAX_VERTEX_NUM 20

typedef int Status;
typedef int ElemType;

/**
 * @brief 邻接矩阵图结构
 * 
 * @param vexs  顶点数组
 * @param arcs  邻接矩阵（arcs[i][j]=1 表示 i→j 有边）
 * @param vexnum 顶点数量
 * @param arcnum 边数量
 */
typedef struct {
    int vexs[MAX_VERTEX_NUM];
    int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int vexnum;
    int arcnum;
} MGraph;

/**
 * @brief 在图中定位顶点
 * 
 * @param G 图
 * @param v 顶点值
 * @return int 顶点在数组中的下标，未找到返回-1
 */
int LocateVex(MGraph G, int v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vexs[i] == v) return i;
    }
    return -1;
}

/**
 * @brief 创建图（邻接矩阵）
 * 
 * @param G 图引用
 * @return Status 成功返回OK
 */
Status CreateMGraph(MGraph &G) {
    printf("请输入顶点数和边数：");
    scanf("%d %d", &G.vexnum, &G.arcnum);
    
    printf("请输入%d个顶点：", G.vexnum);
    for (int i = 0; i < G.vexnum; i++) {
        scanf("%d", &G.vexs[i]);
    }
    
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            G.arcs[i][j] = 0;
        }
    }
    
    printf("请输入%d条边（vi vj）：\n", G.arcnum);
    for (int k = 0; k < G.arcnum; k++) {
        int vi, vj;
        scanf("%d %d", &vi, &vj);
        int i = LocateVex(G, vi);
        int j = LocateVex(G, vj);
        G.arcs[i][j] = 1; // 有向图只要设置 G.arcs[i][j] = 1
        G.arcs[j][i] = 1;
    }
    return OK;
}

/**
 * @brief 深度优先搜索（DFS）- 递归版本
 * 
 * @param G       图
 * @param v       起始顶点值
 * @param visited 访问标记数组
 * 
 * @note 时间复杂度 O(|V|²)，空间复杂度 O(|V|²)
 * @note 递归实现，使用系统调用栈
 * @note 打印遍历序列
 */
void DFS_MGraph(MGraph G, int v, int visited[]) {
    int vIndex = LocateVex(G, v);
    printf("%d ", G.vexs[vIndex]);
    visited[vIndex] = 1;
    
    for (int j = 0; j < G.vexnum; j++) {
        if (G.arcs[vIndex][j] == 1 && !visited[j]) {
            DFS_MGraph(G, G.vexs[j], visited);
        }
    }
}

/**
 * @brief 深度优先搜索（DFS）- 非递归版本
 * 
 * @param G 图
 * @param v 起始顶点值
 * 
 * @note 使用栈模拟递归，避免递归调用栈溢出
 * @note 时间复杂度 O(|V|²)，空间复杂度 O(|V|)
 * @note 打印遍历序列
 */
void DFS_MGraph_NonRecursive(MGraph G, int v) {
    int visited[MAX_VERTEX_NUM] = {0};
    int stack[MAX_VERTEX_NUM];
    int top = -1;
    
    int vIndex = LocateVex(G, v);
    printf("%d ", G.vexs[vIndex]);
    visited[vIndex] = 1;
    stack[++top] = vIndex;
    
    while (top != -1) {
        int i = stack[top];
        int found = 0;
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[i][j] == 1 && !visited[j]) {
                printf("%d ", G.vexs[j]);
                visited[j] = 1;
                stack[++top] = j;
                found = 1;
                break;
            }
        }
        if (!found) {
            top--;
        }
    }
}

/**
 * @brief 广度优先搜索（BFS）
 * 
 * @param G 图
 * @param v 起始顶点值
 * 
 * @note 使用队列实现
 * @note 时间复杂度 O(|V|²)，空间复杂度 O(|V|)
 * @note 打印遍历序列
 */
void BFS_MGraph(MGraph G, int v) {
    int visited[MAX_VERTEX_NUM] = {0};
    int queue[MAX_VERTEX_NUM];
    int front = 0, rear = 0;
    
    int vIndex = LocateVex(G, v);
    printf("%d ", G.vexs[vIndex]);
    visited[vIndex] = 1;
    queue[rear++] = vIndex;
    
    while (front != rear) {
        int i = queue[front++];
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[i][j] == 1 && !visited[j]) {
                printf("%d ", G.vexs[j]);
                visited[j] = 1;
                queue[rear++] = j;
            }
        }
    }
}