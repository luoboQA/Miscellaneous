/* 图搜索算法	数据结构	遍历时间复杂度	空间复杂度
BFS广度优先搜索	邻接矩阵      O(|v|^2)	   O(|v|^2)
               邻接链表	     O(|v|+|E|)  O(|v|+|E|)
*/
#include <iostream>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define MAX_VERTEX_NUM 20

typedef int Status;
typedef int ElemType;

// 邻接矩阵图
typedef struct {
    int vexs[MAX_VERTEX_NUM];
    int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int vexnum;
    int arcnum;
} MGraph;

// 队列（用于BFS）
typedef struct {
    int data[MAX_VERTEX_NUM];
    int front;
    int rear;
} Queue;

// 初始化队列
Status InitQueue(Queue &Q) {
    Q.front = 0;
    Q.rear = 0;
    return OK;
}

// 入队
Status EnQueue(Queue &Q, int e) {
    if (Q.rear >= MAX_VERTEX_NUM) return ERROR;
    Q.data[Q.rear++] = e;
    return OK;
}

// 出队
Status DeQueue(Queue &Q, int &e) {
    if (Q.front == Q.rear) return ERROR;
    e = Q.data[Q.front++];
    return OK;
}

// 判断队列空
Status QueueEmpty(Queue Q) {
    if (Q.front == Q.rear) return TRUE;
    return FALSE;
}

// 定位顶点
int LocateVex(MGraph G, int v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vexs[i] == v) return i;
    }
    return -1;
}

// 创建图
Status CreateMGraph(MGraph &G) {
    printf("请输入顶点数和边数：");
    scanf("%d %d", &G.vexnum, &G.arcnum);
    
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

// BFS（邻接矩阵）
void BFS_MGraph(MGraph G, int v) {
    int visited[MAX_VERTEX_NUM] = {0};
    Queue Q;
    InitQueue(Q);
    
    int vIndex = LocateVex(G, v);
    printf("%d ", G.vexs[vIndex]);
    visited[vIndex] = 1;
    EnQueue(Q, vIndex);
    
    while (!QueueEmpty(Q)) {
        int i;
        DeQueue(Q, i);
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[i][j] == 1 && !visited[j]) {
                printf("%d ", G.vexs[j]);
                visited[j] = 1;
                EnQueue(Q, j);
            }
        }
    }
}

// DFS（邻接矩阵）
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

void DFSTraverse_MGraph(MGraph G) {
    int visited[MAX_VERTEX_NUM] = {0};
    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) {
            DFS_MGraph(G, G.vexs[i], visited);
        }
    }
}

int main() {
    MGraph G;
    
    printf("---【广度优先搜索BFS-邻接矩阵】---\n");
    printf("时间复杂度：O(|v|^2)\n");
    printf("空间复杂度：O(|v|^2)\n\n");
    
    CreateMGraph(G);
    
    printf("\nBFS遍历（从顶点0开始）：");
    BFS_MGraph(G, 0);
    printf("\n");
    
    printf("DFS遍历（从顶点0开始）：");
    int visited[MAX_VERTEX_NUM] = {0};
    DFS_MGraph(G, 0, visited);
    printf("\n");
    
    getchar();
    getchar();
    return 0;
}