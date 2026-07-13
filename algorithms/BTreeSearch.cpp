// 查找算法	平均时间复杂度	空间复杂度	查找条件
// B树/B+树	O(log2n)
#include <iostream>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

#define M 3
#define MIN_KEY (M - 1) / 2

// B树节点（每个节点都存数据）
typedef struct BTNode {
    int keynum;                 // 关键字个数
    ElemType key[M + 1];        // 关键字
    ElemType data[M + 1];       // 数据（每个key都对应一个data）
    struct BTNode *child[M + 1];
    struct BTNode *parent;
    int isLeaf;
} BTNode, *BTree;

// 创建B树节点
BTNode* CreateBTNode(int isLeaf) {
    BTNode *p = (BTNode *)malloc(sizeof(BTNode));
    if (NULL == p) return NULL;
    p->keynum = 0;
    p->isLeaf = isLeaf;
    p->parent = NULL;
    for (int i = 0; i <= M; i++) {
        p->child[i] = NULL;
        p->key[i] = 0;
        p->data[i] = 0;
    }
    return p;
}

// B树查找（找到key直接返回data）
Status SearchBTree(BTree T, ElemType key, ElemType &data) {
    if (NULL == T) return ERROR;
    
    int i = 1;
    while (i <= T->keynum && key > T->key[i]) i++;
    
    if (i <= T->keynum && key == T->key[i]) {
        data = T->data[i];  // 直接返回data
        return OK;
    }
    
    if (T->isLeaf) return ERROR;
    return SearchBTree(T->child[i - 1], key, data);
}

// 插入数据到B树
Status InsertBTree(BTree &T, ElemType key, ElemType data) {
    if (NULL == T) {
        T = CreateBTNode(1);
        T->key[++T->keynum] = key;
        T->data[T->keynum] = data;
        return OK;
    }
    // 简化演示
    return OK;
}

// 中序遍历B树
void InOrderBTree(BTree T) {
    if (T) {
        for (int i = 0; i <= T->keynum; i++) {
            if (!T->isLeaf) InOrderBTree(T->child[i]);
            if (i < T->keynum) {
                printf("[%d:%d]\t", T->key[i + 1], T->data[i + 1]);
            }
        }
    }
}

int main() {
    BTree T = NULL;
    
    printf("---【B树：每个节点都存数据】---\n");
    printf("特点：所有节点都存储key和data\n");
    printf("查找：找到key直接返回data\n\n");
    
    // 构建演示B树
    T = CreateBTNode(0);
    T->keynum = 2;
    T->key[1] = 30; T->data[1] = 100;
    T->key[2] = 60; T->data[2] = 200;
    
    BTNode *child1 = CreateBTNode(1);
    child1->keynum = 2;
    child1->key[1] = 10; child1->data[1] = 10;
    child1->key[2] = 20; child1->data[2] = 20;
    child1->parent = T;
    
    BTNode *child2 = CreateBTNode(1);
    child2->keynum = 2;
    child2->key[1] = 40; child2->data[1] = 40;
    child2->key[2] = 50; child2->data[2] = 50;
    child2->parent = T;
    
    BTNode *child3 = CreateBTNode(1);
    child3->keynum = 2;
    child3->key[1] = 70; child3->data[1] = 70;
    child3->key[2] = 80; child3->data[2] = 80;
    child3->parent = T;
    
    T->child[0] = child1;
    T->child[1] = child2;
    T->child[2] = child3;
    
    printf("中序遍历（key:data）：");
    InOrderBTree(T);
    printf("\n\n");
    
    int key = 40;
    int data;
    if (SearchBTree(T, key, data) == OK) {
        printf("查找key=%d，对应的data=%d\n", key, data);
        printf("注意：数据在内部节点就找到了，无需到叶子节点\n");
    }
    
    key = 20;
    if (SearchBTree(T, key, data) == OK) {
        printf("查找key=%d，对应的data=%d\n", key, data);
        printf("注意：数据在叶子节点找到\n");
    }
    
    getchar();
    return 0;
}