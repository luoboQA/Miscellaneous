/*
 *（红黑性质，平衡，BST）。通过颜色标记和旋转操作保持平衡，确保查找、插入、删除都在O(log n)时间内完成。
 *
 * 红黑树查找思路：
 * 1. 从根节点开始，比较目标值与当前节点关键字
 * 2. 若相等则查找成功
 * 3. 若小于当前节点，进入左子树继续查找
 * 4. 若大于当前节点，进入右子树继续查找
 * 5. 若到达空节点，则查找失败
 *
 * 查找算法	平均时间复杂度	空间复杂度	查找条件
 * 红黑树	O(log₂n)	O(n)	    平衡二叉查找树
 * 
 * 红黑树性质：
 *   1. 每个节点要么是红色，要么是黑色
 *   2. 根节点是黑色
 *   3. 每个叶子节点（NIL）是黑色
 *   4. 红色节点的两个子节点都是黑色（不能有连续红节点）
 *   5. 从任一节点到其每个叶子的所有简单路径都包含相同数目的黑色节点
 * 
 * 红黑树示例（插入序列：[50, 30, 70, 20, 40, 60, 80]）：
 * 
 * 插入50（根，黑色）：
 *     [50黑]
 * 
 * 插入30（红色，50左子）：
 *     [50黑]
 *     /
 *   [30红]
 * 
 * 插入70（红色，50右子）：
 *     [50黑]
 *     /    \
 *   [30红] [70红]
 * 
 * 插入20（红色，30左子）：
 *      [50黑]
 *      /    \
 *    [30红] [70红]
 *    /
 *  [20红]
 *   30和20连续红 → 需要调整
 * 
 * 调整后：
 *      [50黑]
 *      /    \
 *    [30黑] [70红]
 *    /    \
 *  [20红] [40红]
 * 
 * 最终红黑树：
 *         [50黑]
 *        /      \
 *     [30黑]    [70黑]
 *     /    \    /    \
 *   [20红][40红][60红][80红]
 * 
 * 查找40：
 *   第1步：50 > 40 → 进入左子树
 *   第2步：30 < 40 → 进入右子树
 *   第3步：40 == 40 → 找到
 * 
 * 查找90：
 *   第1步：50 < 90 → 进入右子树
 *   第2步：70 < 90 → 进入右子树
 *   第3步：80 < 90 → 进入右子树
 *   第4步：空节点 → 查找失败
 */


using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

typedef enum { RED, BLACK } Color;

/**
 * @brief 红黑树节点结构体
 * 
 * @param data   数据域
 * @param color  颜色（RED/BLACK）
 * @param lchild 左子树指针
 * @param rchild 右子树指针
 * @param parent 父节点指针
 */
typedef struct RBNode {
    ElemType data;
    Color color;
    struct RBNode *lchild;
    struct RBNode *rchild;
    struct RBNode *parent;
} RBNode, *RBTree;

/**
 * @brief 红黑树查找
 * 
 * @param T   红黑树根节点
 * @param key 目标关键字
 * @return RBNode* 找到返回节点指针，未找到返回NULL
 * 
 * @note 时间复杂度 O(log₂n)
 */
RBNode* SearchRBTree(RBTree T, ElemType key) {
    while (T && T->data != key) {
        if (key < T->data)
            T = T->lchild;
        else
            T = T->rchild;
    }
    return T;
}

/**
 * @brief 左旋
 * 
 * @param T 红黑树根节点
 * @param x 旋转的支点
 */
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

/**
 * @brief 右旋
 * 
 * @param T 红黑树根节点
 * @param x 旋转的支点
 */
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

/**
 * @brief 插入修复（保持红黑树性质）
 * 
 * @param T 红黑树根节点
 * @param z 新插入的红色节点
 */
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

/**
 * @brief 插入红黑树
 * 
 * @param T 红黑树根节点
 * @param e 要插入的元素
 * @return Status 插入成功返回OK，已存在返回ERROR
 */
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