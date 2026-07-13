#include <iostream>
#include <queue>
#include <stack>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef char TElemType;

// 二叉树链式存储结构
typedef struct BiTNode {
    TElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

// ========== 创建二叉树 ==========

// 1. 前序遍历创建二叉树（用'#'表示空）
Status CreateBiTree(BiTree &T) {
    TElemType ch;
    scanf("%c", &ch);
    if (ch == '#') {
        T = NULL;
    } else {
        T = (BiTNode *)malloc(sizeof(BiTNode));
        if (!T) return OVERFLOW;
        T->data = ch;
        CreateBiTree(T->lchild);
        CreateBiTree(T->rchild);
    }
    return OK;
}

// 2. 创建满二叉树示例
BiTree CreateSampleTree() {
    /*
             A
           /   \
          B     C
         / \   / \
        D   E F   G
    */
    BiTree T = (BiTree)malloc(sizeof(BiTNode));
    T->data = 'A';
    
    BiTree B = (BiTree)malloc(sizeof(BiTNode));
    B->data = 'B';
    BiTree C = (BiTree)malloc(sizeof(BiTNode));
    C->data = 'C';
    BiTree D = (BiTree)malloc(sizeof(BiTNode));
    D->data = 'D';
    BiTree E = (BiTree)malloc(sizeof(BiTNode));
    E->data = 'E';
    BiTree F = (BiTree)malloc(sizeof(BiTNode));
    F->data = 'F';
    BiTree G = (BiTree)malloc(sizeof(BiTNode));
    G->data = 'G';
    
    T->lchild = B; T->rchild = C;
    B->lchild = D; B->rchild = E;
    C->lchild = F; C->rchild = G;
    D->lchild = D->rchild = NULL;
    E->lchild = E->rchild = NULL;
    F->lchild = F->rchild = NULL;
    G->lchild = G->rchild = NULL;
    
    return T;
}

// ========== 遍历 ==========

// 3. 前序遍历（递归）
void PreOrder(BiTree T) {
    if (T) {
        printf("%c ", T->data);
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }
}

// 4. 中序遍历（递归）
void InOrder(BiTree T) {
    if (T) {
        InOrder(T->lchild);
        printf("%c ", T->data);
        InOrder(T->rchild);
    }
}

// 5. 后序遍历（递归）
void PostOrder(BiTree T) {
    if (T) {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        printf("%c ", T->data);
    }
}

// 6. 前序遍历（非递归）
void PreOrder_NonRecursive(BiTree T) {
    if (!T) return;
    stack<BiTree> s;
    BiTree p = T;
    while (p || !s.empty()) {
        while (p) {
            printf("%c ", p->data);
            s.push(p);
            p = p->lchild;
        }
        if (!s.empty()) {
            p = s.top();
            s.pop();
            p = p->rchild;
        }
    }
}

// 7. 中序遍历（非递归）
void InOrder_NonRecursive(BiTree T) {
    if (!T) return;
    stack<BiTree> s;
    BiTree p = T;
    while (p || !s.empty()) {
        while (p) {
            s.push(p);
            p = p->lchild;
        }
        if (!s.empty()) {
            p = s.top();
            s.pop();
            printf("%c ", p->data);
            p = p->rchild;
        }
    }
}

// 8. 后序遍历（非递归）
void PostOrder_NonRecursive(BiTree T) {
    if (!T) return;
    stack<BiTree> s1, s2;
    s1.push(T);
    while (!s1.empty()) {
        BiTree p = s1.top();
        s1.pop();
        s2.push(p);
        if (p->lchild) s1.push(p->lchild);
        if (p->rchild) s1.push(p->rchild);
    }
    while (!s2.empty()) {
        printf("%c ", s2.top()->data);
        s2.pop();
    }
}

// 9. 层次遍历
void LevelOrder(BiTree T) {
    if (!T) return;
    queue<BiTree> q;
    q.push(T);
    while (!q.empty()) {
        BiTree p = q.front();
        q.pop();
        printf("%c ", p->data);
        if (p->lchild) q.push(p->lchild);
        if (p->rchild) q.push(p->rchild);
    }
}

// ========== 二叉树性质 ==========

// 10. 求二叉树深度
int TreeDepth(BiTree T) {
    if (!T) return 0;
    int leftDepth = TreeDepth(T->lchild);
    int rightDepth = TreeDepth(T->rchild);
    return (leftDepth > rightDepth) ? leftDepth + 1 : rightDepth + 1;
}

// 11. 求叶子结点数
int LeafCount(BiTree T) {
    if (!T) return 0;
    if (!T->lchild && !T->rchild) return 1;
    return LeafCount(T->lchild) + LeafCount(T->rchild);
}

// 12. 求度为2的结点数
int Degree2Count(BiTree T) {
    if (!T) return 0;
    int count = 0;
    if (T->lchild && T->rchild) count = 1;
    return count + Degree2Count(T->lchild) + Degree2Count(T->rchild);
}

// 13. 求结点总数
int NodeCount(BiTree T) {
    if (!T) return 0;
    return 1 + NodeCount(T->lchild) + NodeCount(T->rchild);
}

// 14. 判断是否满二叉树
bool IsFullBinaryTree(BiTree T) {
    if (!T) return true;
    if (T->lchild && !T->rchild) return false;
    if (!T->lchild && T->rchild) return false;
    return IsFullBinaryTree(T->lchild) && IsFullBinaryTree(T->rchild);
}

// 15. 判断是否完全二叉树
bool IsCompleteBinaryTree(BiTree T) {
    if (!T) return true;
    queue<BiTree> q;
    q.push(T);
    bool hasNullChild = false;
    
    while (!q.empty()) {
        BiTree p = q.front();
        q.pop();
        
        if (!p) {
            hasNullChild = true;
        } else {
            if (hasNullChild) return false;
            q.push(p->lchild);
            q.push(p->rchild);
        }
    }
    return true;
}

// ========== 由遍历序列重建二叉树 ==========

// 16. 由前序+中序重建二叉树
BiTree BuildByPreIn(char pre[], char in[], int preStart, int inStart, int len) {
    if (len <= 0) return NULL;
    
    BiTree T = (BiTNode *)malloc(sizeof(BiTNode));
    T->data = pre[preStart];
    
    // 在中序中找到根的位置
    int rootIdx = inStart;
    while (in[rootIdx] != pre[preStart]) rootIdx++;
    
    int leftLen = rootIdx - inStart;
    int rightLen = len - leftLen - 1;
    
    T->lchild = BuildByPreIn(pre, in, preStart + 1, inStart, leftLen);
    T->rchild = BuildByPreIn(pre, in, preStart + leftLen + 1, rootIdx + 1, rightLen);
    
    return T;
}

// 17. 由中序+后序重建二叉树
BiTree BuildByInPost(char in[], char post[], int inStart, int postStart, int len) {
    if (len <= 0) return NULL;
    
    BiTree T = (BiTNode *)malloc(sizeof(BiTNode));
    T->data = post[postStart + len - 1];
    
    int rootIdx = inStart;
    while (in[rootIdx] != post[postStart + len - 1]) rootIdx++;
    
    int leftLen = rootIdx - inStart;
    int rightLen = len - leftLen - 1;
    
    T->lchild = BuildByInPost(in, post, inStart, postStart, leftLen);
    T->rchild = BuildByInPost(in, post, inStart + leftLen + 1, postStart + leftLen, rightLen);
    
    return T;
}

// ========== 交换左右子树 ==========

void SwapTree(BiTree &T) {
    if (!T) return;
    BiTree temp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = temp;
    SwapTree(T->lchild);
    SwapTree(T->rchild);
}

// ========== 查找结点 ==========

BiTree FindNode(BiTree T, TElemType e) {
    if (!T) return NULL;
    if (T->data == e) return T;
    
    BiTree found = FindNode(T->lchild, e);
    if (found) return found;
    return FindNode(T->rchild, e);
}

// ========== 销毁二叉树 ==========

void DestroyBiTree(BiTree &T) {
    if (T) {
        DestroyBiTree(T->lchild);
        DestroyBiTree(T->rchild);
        free(T);
        T = NULL;
    }
}

int main() {
    printf("========================================\n");
    printf("    二叉树完整实现\n");
    printf("========================================\n\n");
    
    printf("性质验证：\n");
    printf("1. 非空二叉树第i层最多 2^(i-1) 个结点\n");
    printf("2. 深度为k的二叉树最多 2^k - 1 个结点\n");
    printf("3. n0 = n2 + 1\n\n");
    
    BiTree T = CreateSampleTree();
    
    printf("示例二叉树（满二叉树）：\n");
    printf("      A\n");
    printf("    /   \\\n");
    printf("   B     C\n");
    printf("  / \\   / \\\n");
    printf(" D   E F   G\n\n");
    
    printf("遍历结果：\n");
    printf("前序遍历（递归）："); PreOrder(T); printf("\n");
    printf("中序遍历（递归）："); InOrder(T); printf("\n");
    printf("后序遍历（递归）："); PostOrder(T); printf("\n");
    
    printf("\n非递归遍历：\n");
    printf("前序遍历："); PreOrder_NonRecursive(T); printf("\n");
    printf("中序遍历："); InOrder_NonRecursive(T); printf("\n");
    printf("后序遍历："); PostOrder_NonRecursive(T); printf("\n");
    
    printf("\n层次遍历："); LevelOrder(T); printf("\n");
    
    printf("\n基本性质：\n");
    printf("结点总数：%d\n", NodeCount(T));
    printf("叶子结点数：%d\n", LeafCount(T));
    printf("度为2的结点数：%d\n", Degree2Count(T));
    printf("树的高度：%d\n", TreeDepth(T));
    printf("是否满二叉树：%s\n", IsFullBinaryTree(T) ? "是" : "否");
    printf("是否完全二叉树：%s\n", IsCompleteBinaryTree(T) ? "是" : "否");
    
    printf("\n验证性质 n0 = n2 + 1：\n");
    printf("%d = %d + 1\n", LeafCount(T), Degree2Count(T));
    
    printf("\n由遍历序列重建二叉树：\n");
    char pre[] = {'A', 'B', 'D', 'E', 'C', 'F', 'G'};
    char in[] = {'D', 'B', 'E', 'A', 'F', 'C', 'G'};
    BiTree T2 = BuildByPreIn(pre, in, 0, 0, 7);
    printf("重建后中序遍历："); InOrder(T2); printf("\n");
    
    DestroyBiTree(T);
    DestroyBiTree(T2);
    
    getchar();
    return 0;
}