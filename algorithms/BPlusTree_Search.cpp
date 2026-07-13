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

// B+树节点
typedef struct BPNode {
    int keynum;
    ElemType key[M + 1];        // 只有key，没有data！
    struct BPNode *child[M + 1];
    struct BPNode *parent;
    struct BPNode *next;        // 叶子节点链表指针
    int isLeaf;
    ElemType *data;             // 只有叶子节点才有data数组
} BPNode, *BPlusTree;

// 创建B+树节点
BPNode* CreateBPNode(int isLeaf) {
    BPNode *p = (BPNode *)malloc(sizeof(BPNode));
    if (NULL == p) return NULL;
    p->keynum = 0;
    p->isLeaf = isLeaf;
    p->parent = NULL;
    p->next = NULL;
    p->data = NULL;
    for (int i = 0; i <= M; i++) {
        p->child[i] = NULL;
        p->key[i] = 0;
    }
    if (isLeaf) {
        p->data = (ElemType *)malloc((M + 1) * sizeof(ElemType));
        for (int i = 0; i <= M; i++) p->data[i] = 0;
    }
    return p;
}

// B+树查找（必须到叶子节点才能找到data）
Status SearchBPlusTree(BPlusTree T, ElemType key, ElemType &data) {
    if (NULL == T) return ERROR;
    
    BPNode *p = T;
    while (!p->isLeaf) {
        int i = 1;
        while (i <= p->keynum && key >= p->key[i]) i++;
        p = p->child[i - 1];
    }
    
    // 到叶子节点查找
    int i = 1;
    while (i <= p->keynum && key > p->key[i]) i++;
    if (i <= p->keynum && key == p->key[i]) {
        data = p->data[i];  // 叶子节点才有data
        return OK;
    }
    return ERROR;
}

// B+树范围查找（利用叶子节点链表）
Status SearchRangeBPlusTree(BPlusTree T, ElemType startKey, ElemType endKey) {
    if (NULL == T) return ERROR;
    
    BPNode *p = T;
    while (!p->isLeaf) {
        int i = 1;
        while (i <= p->keynum && startKey >= p->key[i]) i++;
        p = p->child[i - 1];
    }
    
    printf("范围查找 [%d, %d]：", startKey, endKey);
    while (p) {
        for (int i = 1; i <= p->keynum; i++) {
            if (p->key[i] >= startKey && p->key[i] <= endKey) {
                printf("%d:%d\t", p->key[i], p->data[i]);
            }
            if (p->key[i] > endKey) {
                printf("\n");
                return OK;
            }
        }
        p = p->next;  // 通过链表跳转到下一个叶子节点
    }
    printf("\n");
    return OK;
}

// 中序遍历B+树（只遍历叶子节点）
void InOrderBPlusTree(BPlusTree T) {
    if (NULL == T) return;
    if (T->isLeaf) {
        for (int i = 1; i <= T->keynum; i++) {
            printf("[%d:%d]\t", T->key[i], T->data[i]);
        }
        return;
    }
    for (int i = 0; i <= T->keynum; i++) {
        InOrderBPlusTree(T->child[i]);
    }
}

int main() {
    BPlusTree T = NULL;
    
    printf("---【B+树：只有叶子节点存数据】---\n");
    printf("特点：内部节点只存key（索引），叶子节点存key+data\n");
    printf("查找：必须到叶子节点才能找到data\n");
    printf("优势：适合范围查找，叶子节点链表相连\n\n");
    
    // 构建演示B+树
    T = CreateBPNode(0);
    T->keynum = 2;
    T->key[1] = 30;
    T->key[2] = 60;
    
    BPNode *child1 = CreateBPNode(1);
    child1->keynum = 2;
    child1->key[1] = 10; child1->data[1] = 10;
    child1->key[2] = 20; child1->data[2] = 20;
    child1->parent = T;
    
    BPNode *child2 = CreateBPNode(1);
    child2->keynum = 2;
    child2->key[1] = 30; child2->data[1] = 30;
    child2->key[2] = 40; child2->data[2] = 40;
    child2->parent = T;
    
    BPNode *child3 = CreateBPNode(1);
    child3->keynum = 2;
    child3->key[1] = 50; child3->data[1] = 50;
    child3->key[2] = 60; child3->data[2] = 60;
    child3->parent = T;
    
    // 连接叶子节点链表
    child1->next = child2;
    child2->next = child3;
    
    T->child[0] = child1;
    T->child[1] = child2;
    T->child[2] = child3;
    
    printf("叶子节点（key:data）：");
    InOrderBPlusTree(T);
    printf("\n\n");
    
    int key = 40;
    int data;
    if (SearchBPlusTree(T, key, data) == OK) {
        printf("查找key=%d，对应的data=%d\n", key, data);
        printf("注意：必须遍历到叶子节点才能获取data\n");
    }
    
    key = 30;
    if (SearchBPlusTree(T, key, data) == OK) {
        printf("查找key=%d，对应的data=%d\n", key, data);
        printf("注意：虽然内部节点有key=30，但数据在叶子节点\n\n");
    }
    
    // 范围查找
    SearchRangeBPlusTree(T, 15, 45);
    printf("\nB+树优势：范围查找高效，叶子节点形成有序链表\n");
    
    getchar();
    return 0;
}