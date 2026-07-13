// 查找算法	平均时间复杂度	空间复杂度	查找条件
// 红黑树	O(log2n)
#include <iostream>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    ElemType data;
    Color color;
    struct RBNode *lchild;
    struct RBNode *rchild;
    struct RBNode *parent;
} RBNode, *RBTree;

// 红黑树查找
RBNode* SearchRBTree(RBTree T, ElemType key) {
    while (T && T->data != key) {
        if (key < T->data)
            T = T->lchild;
        else
            T = T->rchild;
    }
    return T;
}

// 左旋
void LeftRotate(RBTree &T, RBNode *x) {
    RBNode *y = x->rchild;
    x->rchild = y->lchild;
    if (y->lchild) y->lchild->parent = x;
    y->parent = x->parent;
    if (!x->parent) T = y;
    else if (x == x->parent->lchild) x->parent->lchild = y;
    else x->parent->rchild = y;
    y->lchild = x;
    x->parent = y;
}

// 右旋
void RightRotate(RBTree &T, RBNode *x) {
    RBNode *y = x->lchild;
    x->lchild = y->rchild;
    if (y->rchild) y->rchild->parent = x;
    y->parent = x->parent;
    if (!x->parent) T = y;
    else if (x == x->parent->lchild) x->parent->lchild = y;
    else x->parent->rchild = y;
    y->rchild = x;
    x->parent = y;
}

// 插入修复
void InsertFixup(RBTree &T, RBNode *z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->lchild) {
            RBNode *y = z->parent->parent->rchild;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->rchild) {
                    z = z->parent;
                    LeftRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(T, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->lchild;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->lchild) {
                    z = z->parent;
                    RightRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(T, z->parent->parent);
            }
        }
    }
    T->color = BLACK;
}

// 插入红黑树
Status InsertRBTree(RBTree &T, ElemType e) {
    RBNode *z = (RBNode *)malloc(sizeof(RBNode));
    if (NULL == z) return OVERFLOW;
    z->data = e;
    z->color = RED;
    z->lchild = NULL;
    z->rchild = NULL;
    z->parent = NULL;

    RBNode *y = NULL;
    RBNode *x = T;
    while (x) {
        y = x;
        if (e < x->data) x = x->lchild;
        else if (e > x->data) x = x->rchild;
        else return ERROR;
    }
    z->parent = y;
    if (!y) T = z;
    else if (e < y->data) y->lchild = z;
    else y->rchild = z;

    InsertFixup(T, z);
    return OK;
}

// 中序遍历
void InOrderRB(RBTree T) {
    if (T) {
        InOrderRB(T->lchild);
        printf("%d(%s)\t", T->data, T->color == RED ? "红" : "黑");
        InOrderRB(T->rchild);
    }
}

int main() {
    RBTree T = NULL;
    int i;

    printf("---【红黑树查找】---\n");
    printf("平均时间复杂度：O(log2n)\n");
    printf("空间复杂度：O(n)\n");
    printf("查找条件：平衡二叉查找树\n\n");

    int rbArray[] = {50, 30, 70, 20, 40, 60, 80};
    for (i = 0; i < 7; i++) {
        InsertRBTree(T, rbArray[i]);
    }
    printf("中序遍历（颜色）：");
    InOrderRB(T);
    printf("\n");

    int target = 40;
    RBNode *result = SearchRBTree(T, target);
    printf("查找%d：%s\n", target, result ? "找到" : "未找到");

    target = 90;
    result = SearchRBTree(T, target);
    printf("查找%d：%s\n", target, result ? "找到" : "未找到");

    getchar();
    return 0;
}