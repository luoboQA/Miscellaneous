// 图搜索算法	数据结构	遍历时间复杂度	空间复杂度
// DFS深度优先搜索	邻接矩阵      O(|v|^2)	   O(|v|^2)
//                邻接链表	     O(|v|+|E|)  O(|v|+|E|)
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
        G.arcs[j][i] = 1;
    }
    return OK;
}

// DFS递归（邻接矩阵）
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

// DFS非递归（邻接矩阵）
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

// BFS（邻接矩阵）
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

int main() {
    MGraph G;
    
    printf("---【深度优先搜索DFS-邻接矩阵】---\n");
    printf("时间复杂度：O(|v|^2)\n");
    printf("空间复杂度：O(|v|^2)\n\n");
    
    CreateMGraph(G);
    
    printf("\nDFS递归遍历（从顶点0开始）：");
    int visited[MAX_VERTEX_NUM] = {0};
    DFS_MGraph(G, 0, visited);
    printf("\n");
    
    printf("DFS非递归遍历（从顶点0开始）：");
    DFS_MGraph_NonRecursive(G, 0);
    printf("\n");
    
    printf("BFS遍历（从顶点0开始）：");
    BFS_MGraph(G, 0);
    printf("\n");
    
    getchar();
    getchar();
    return 0;
}