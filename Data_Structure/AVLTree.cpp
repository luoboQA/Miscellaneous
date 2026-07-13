#include <iostream>
#include <algorithm>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== AVL树 ====================

typedef struct AVLNode {
    ElemType data;
    int height;
    struct AVLNode *lchild;
    struct AVLNode *rchild;
} AVLNode, *AVLTree;

// 获取高度
int GetHeight(AVLTree T) {
    if (NULL == T) return 0;
    return T->height;
}

// 更新高度
void UpdateHeight(AVLTree T) {
    if (T) {
        T->height = max(GetHeight(T->lchild), GetHeight(T->rchild)) + 1;
    }
}

// 计算平衡因子
int BalanceFactor(AVLTree T) {
    if (NULL == T) return 0;
    return GetHeight(T->lchild) - GetHeight(T->rchild);
}

// 左旋（LL旋转）
AVLTree LeftRotate(AVLTree T) {
    AVLNode *y = T->rchild;
    T->rchild = y->lchild;
    y->lchild = T;
    
    UpdateHeight(T);
    UpdateHeight(y);
    return y;
}

// 右旋（RR旋转）
AVLTree RightRotate(AVLTree T) {
    AVLNode *y = T->lchild;
    T->lchild = y->rchild;
    y->rchild = T;
    
    UpdateHeight(T);
    UpdateHeight(y);
    return y;
}

// 插入节点
AVLTree InsertAVL(AVLTree T, ElemType e) {
    if (NULL == T) {
        AVLNode *p = (AVLNode *)malloc(sizeof(AVLNode));
        p->data = e;
        p->height = 1;
        p->lchild = NULL;
        p->rchild = NULL;
        return p;
    }
    
    if (e < T->data) {
        T->lchild = InsertAVL(T->lchild, e);
        // 检查平衡
        if (BalanceFactor(T) == 2) {
            if (BalanceFactor(T->lchild) == 1) {
                // LL型：右旋
                T = RightRotate(T);
            } else if (BalanceFactor(T->lchild) == -1) {
                // LR型：先左旋再右旋
                T->lchild = LeftRotate(T->lchild);
                T = RightRotate(T);
            }
        }
    } else if (e > T->data) {
        T->rchild = InsertAVL(T->rchild, e);
        if (BalanceFactor(T) == -2) {
            if (BalanceFactor(T->rchild) == -1) {
                // RR型：左旋
                T = LeftRotate(T);
            } else if (BalanceFactor(T->rchild) == 1) {
                // RL型：先右旋再左旋
                T->rchild = RightRotate(T->rchild);
                T = LeftRotate(T);
            }
        }
    } else {
        printf("元素 %d 已存在\n", e);
    }
    
    UpdateHeight(T);
    return T;
}

// 查找最小节点
AVLTree FindMin(AVLTree T) {
    if (NULL == T) return NULL;
    while (T->lchild) T = T->lchild;
    return T;
}

// 删除节点
AVLTree DeleteAVL(AVLTree T, ElemType e) {
    if (NULL == T) return NULL;
    
    if (e < T->data) {
        T->lchild = DeleteAVL(T->lchild, e);
        if (BalanceFactor(T) == -2) {
            if (BalanceFactor(T->rchild) <= 0) {
                T = LeftRotate(T);
            } else {
                T->rchild = RightRotate(T->rchild);
                T = LeftRotate(T);
            }
        }
    } else if (e > T->data) {
        T->rchild = DeleteAVL(T->rchild, e);
        if (BalanceFactor(T) == 2) {
            if (BalanceFactor(T->lchild) >= 0) {
                T = RightRotate(T);
            } else {
                T->lchild = LeftRotate(T->lchild);
                T = RightRotate(T);
            }
        }
    } else {
        // 找到要删除的节点
        if (NULL == T->lchild || NULL == T->rchild) {
            AVLNode *temp = T->lchild ? T->lchild : T->rchild;
            free(T);
            return temp;
        } else {
            AVLNode *temp = FindMin(T->rchild);
            T->data = temp->data;
            T->rchild = DeleteAVL(T->rchild, temp->data);
            if (BalanceFactor(T) == 2) {
                if (BalanceFactor(T->lchild) >= 0) {
                    T = RightRotate(T);
                } else {
                    T->lchild = LeftRotate(T->lchild);
                    T = RightRotate(T);
                }
            }
        }
    }
    
    if (T) UpdateHeight(T);
    return T;
}

// 中序遍历
void InOrderAVL(AVLTree T) {
    if (T) {
        InOrderAVL(T->lchild);
        printf("%d(h=%d) ", T->data, T->height);
        InOrderAVL(T->rchild);
    }
}

// 查找
AVLTree SearchAVL(AVLTree T, ElemType key) {
    if (NULL == T || T->data == key) return T;
    if (key < T->data) return SearchAVL(T->lchild, key);
    return SearchAVL(T->rchild, key);
}

int main() {
    printf("========================================\n");
    printf("    AVL平衡二叉树\n");
    printf("========================================\n");
    printf("时间复杂度：O(logn)\n");
    printf("空间复杂度：O(n)\n\n");
    
    AVLTree T = NULL;
    int arr[] = {10, 20, 30, 40, 50, 25};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("插入序列：");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
        T = InsertAVL(T, arr[i]);
    }
    printf("\n\n");
    
    printf("中序遍历（带高度）：");
    InOrderAVL(T);
    printf("\n");
    
    printf("根节点：%d，高度：%d\n", T->data, T->height);
    printf("平衡因子：%d\n\n", BalanceFactor(T));
    
    int key = 30;
    AVLNode *found = SearchAVL(T, key);
    printf("查找%d：%s\n", key, found ? "找到" : "未找到");
    
    key = 20;
    T = DeleteAVL(T, key);
    printf("删除%d后中序：", key);
    InOrderAVL(T);
    printf("\n");
    
    getchar();
    return 0;
}