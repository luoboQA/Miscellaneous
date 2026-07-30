/*
 *（队列，邻接矩阵）。通过队列逐层访问顶点，先访问距离源点近的顶点。
 *
 * 广度优先搜索（BFS）思路：
 * 1. 从源点开始，将其入队并标记为已访问
 * 2. 从队列中取出一个顶点，访问其所有未被访问的邻接点
 * 3. 将每个未被访问的邻接点入队并标记为已访问
 * 4. 重复步骤2-3，直到队列为空
 *
 * 搜索算法	平均时间复杂度	最差时间复杂度	空间复杂度	数据对象稳定性
 * BFS(邻接矩阵)	O(V²)	         O(V²)	        O(V²)	    适用于无权图
 * 图示例：5个顶点（0,1,2,3,4），无向边：0-1, 0-2, 0-3, 1-4, 2-4, 3-4
 * 
 * 邻接矩阵：
 *       0  1  2  3  4
 *   0  [0, 1, 1, 1, 0]
 *   1  [1, 0, 0, 0, 1]
 *   2  [1, 0, 0, 0, 1]
 *   3  [1, 0, 0, 0, 1]
 *   4  [0, 1, 1, 1, 0]
 * 
 * BFS过程（从顶点0开始）：
 *   初始状态：队列空，visited全0
 * 
 *   第1步：访问0，标记visited[0]=1，入队 → 队列：[0]
 *   输出：0
 * 
 *   第2步：出队0 → 查看邻接点1,2,3（按顺序）
 *     访问1，标记visited[1]=1，入队 → 队列：[1,2,3]
 *     访问2，标记visited[2]=1，入队 → 队列：[1,2,3]
 *     访问3，标记visited[3]=1，入队 → 队列：[1,2,3]
 *   输出：0 1
 * 
 *   第3步：出队1 → 查看邻接点0,4
 *     0已访问，跳过
 *     4未访问，标记visited[4]=1，入队 → 队列：[2,3,4]
 *   输出：0 1 2
 * 
 *   第4步：出队2 → 查看邻接点0,4
 *     0已访问，跳过
 *     4已访问，跳过
 *   输出：0 1 2 3
 * 
 *   第5步：出队3 → 查看邻接点0,4
 *     0已访问，跳过
 *     4已访问，跳过
 *   输出：0 1 2 3 4
 * 
 *   第6步：出队4 → 查看邻接点1,2,3
 *     全部已访问，跳过
 *   队列空，结束
 * 
 * 最终BFS序列：0 1 2 3 4
 * 
 * 特点：BFS从源点开始逐层访问，先访问距离源点近的顶点
 *       距离源点1的顶点：1,2,3
 *       距离源点2的顶点：4
 */

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
    int vexs[MAX_VERTEX_NUM];                    // 顶点数组
    int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];    // 邻接矩阵
    int vexnum;                                  // 顶点数
    int arcnum;                                  // 边数
} MGraph;

/**
 * @brief 队列结构（用于BFS）
 */
typedef struct {
    int data[MAX_VERTEX_NUM];
    int front;
    int rear;
} Queue;

/**
 * @brief 初始化队列
 * 
 * @param Q 队列引用
 * @return Status 初始化成功返回OK
 */
Status InitQueue(Queue& Q) {
    Q.front = 0;
    Q.rear = 0;
    return OK;
}

/**
 * @brief 入队操作
 * 
 * @param Q 队列引用
 * @param e 入队元素
 * @return Status 成功返回OK，队列满返回ERROR
 */
Status EnQueue(Queue& Q, int e) {
    if (Q.rear >= MAX_VERTEX_NUM) {
        return ERROR;
    }
    Q.data[Q.rear++] = e;
    return OK;
}

/**
 * @brief 出队操作
 * 
 * @param Q 队列引用
 * @param e 出队元素（输出）
 * @return Status 成功返回OK，队列空返回ERROR
 */
Status DeQueue(Queue& Q, int& e) {
    if (Q.front == Q.rear) {
        return ERROR;
    }
    e = Q.data[Q.front++];
    return OK;
}

/**
 * @brief 判断队列是否为空
 * 
 * @param Q 队列
 * @return Status 空返回TRUE，非空返回FALSE
 */
Status QueueEmpty(Queue Q) {
    return (Q.front == Q.rear) ? TRUE : FALSE;
}

/**
 * @brief 在图中定位顶点
 * 
 * @param G 图
 * @param v 顶点值
 * @return int 顶点在数组中的下标，未找到返回-1
 * 
 * @note 时间复杂度 O(V)
 */
int LocateVex(MGraph G, int v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vexs[i] == v) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief 创建图（邻接矩阵）
 * 
 * @param G 图引用
 * @return Status 成功返回OK
 * 
 * @note 从标准输入读取图数据
 * @note 无向图：arcs[i][j] = arcs[j][i] = 1
 */
Status CreateMGraph(MGraph& G) {
    printf("请输入顶点数和边数：");
    scanf("%d %d", &G.vexnum, &G.arcnum);
    
    // 输入顶点
    printf("请输入%d个顶点：", G.vexnum);
    for (int i = 0; i < G.vexnum; i++) {
        scanf("%d", &G.vexs[i]);
    }
    
    // 初始化邻接矩阵
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            G.arcs[i][j] = 0;
        }
    }
    
    // 输入边
    printf("请输入%d条边（vi vj）：\n", G.arcnum);
    for (int k = 0; k < G.arcnum; k++) {
        int vi, vj;
        scanf("%d %d", &vi, &vj);
        int i = LocateVex(G, vi);
        int j = LocateVex(G, vj);
        G.arcs[i][j] = 1;
        G.arcs[j][i] = 1;  // 无向图
    }
    return OK;
}

/**
 * @brief 广度优先搜索（BFS）- 从指定顶点开始
 * 
 * @param G 图
 * @param v 起始顶点值
 * 
 * @note 使用队列实现
 * @note 时间复杂度 O(V²)，空间复杂度 O(V)
 * @note 打印遍历序列
 * 
 * @example
 *   MGraph G;
 *   CreateMGraph(G);
 *   BFS_MGraph(G, 0);  // 从顶点0开始BFS
 */
void BFS_MGraph(MGraph G, int v) {
    int visited[MAX_VERTEX_NUM] = {0};  // 访问标记数组
    Queue Q;
    InitQueue(Q);
    
    // 从起始顶点开始
    int vIndex = LocateVex(G, v);
    printf("%d ", G.vexs[vIndex]);
    visited[vIndex] = 1;
    EnQueue(Q, vIndex);
    
    // BFS主循环
    while (!QueueEmpty(Q)) {
        int i;
        DeQueue(Q, i);
        
        // 遍历所有邻接点
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[i][j] == 1 && !visited[j]) {
                printf("%d ", G.vexs[j]);
                visited[j] = 1;
                EnQueue(Q, j);
            }
        }
    }
}

/**
 * @brief 深度优先搜索（DFS）- 从指定顶点开始
 * 
 * @param G       图
 * @param v       起始顶点值
 * @param visited 访问标记数组
 * 
 * @note 递归实现
 * @note 时间复杂度 O(V²)，空间复杂度 O(V)
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
 * @brief 深度优先搜索（DFS）- 遍历整个图
 * 
 * @param G 图
 * 
 * @note 处理非连通图
 * @note 遍历所有连通分量
 */
void DFSTraverse_MGraph(MGraph G) {
    int visited[MAX_VERTEX_NUM] = {0};
    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) {
            DFS_MGraph(G, G.vexs[i], visited);
        }
    }
}