#include <iostream>
#include <vector>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 多维数组（动态分配） ====================

// 一维数组
int* Create1DArray(int n) {
    return new int[n];
}

// 二维数组（连续存储）
int** Create2DArray(int rows, int cols) {
    int **arr = new int*[rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[cols];
    }
    return arr;
}

// 二维数组（一维模拟，行优先存储）
int* Create2DArrayFlat(int rows, int cols) {
    return new int[rows * cols];
}

int GetElement2D(int *arr, int rows, int cols, int i, int j) {
    return arr[i * cols + j];
}

void SetElement2D(int *arr, int rows, int cols, int i, int j, int val) {
    arr[i * cols + j] = val;
}

// 三维数组
int*** Create3DArray(int dim1, int dim2, int dim3) {
    int ***arr = new int**[dim1];
    for (int i = 0; i < dim1; i++) {
        arr[i] = new int*[dim2];
        for (int j = 0; j < dim2; j++) {
            arr[i][j] = new int[dim3];
        }
    }
    return arr;
}

// 释放三维数组
void Delete3DArray(int ***arr, int dim1, int dim2) {
    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim2; j++) {
            delete[] arr[i][j];
        }
        delete[] arr[i];
    }
    delete[] arr;
}

// ==================== 矩阵的压缩存储 ====================

// 对称矩阵压缩（下三角存储）
class SymmetricMatrix {
private:
    int *data;
    int n;
    
    int GetIndex(int i, int j) {
        if (i < j) swap(i, j);  // 对称，取较大为行
        return i * (i + 1) / 2 + j;
    }
    
public:
    SymmetricMatrix(int n) : n(n) {
        int size = n * (n + 1) / 2;
        data = new int[size];
        for (int i = 0; i < size; i++) data[i] = 0;
    }
    
    ~SymmetricMatrix() {
        delete[] data;
    }
    
    void Set(int i, int j, int val) {
        data[GetIndex(i, j)] = val;
    }
    
    int Get(int i, int j) {
        return data[GetIndex(i, j)];
    }
    
    void Print() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%4d", Get(i, j));
            }
            printf("\n");
        }
    }
};

// ==================== 广义表 ====================

typedef enum { ATOM, LIST } ElemTag;

typedef struct GLNode {
    ElemTag tag;
    union {
        int atom;
        struct GLNode *hp;  // 表头指针
    };
    struct GLNode *tp;      // 表尾指针
} GLNode, *GList;

// 创建原子节点
GList MakeAtom(int val) {
    GLNode *p = (GLNode *)malloc(sizeof(GLNode));
    p->tag = ATOM;
    p->atom = val;
    p->tp = NULL;
    return p;
}

// 创建列表节点
GList MakeList(GList head, GList tail) {
    GLNode *p = (GLNode *)malloc(sizeof(GLNode));
    p->tag = LIST;
    p->hp = head;
    p->tp = tail;
    return p;
}

// 求广义表深度
int GListDepth(GList L) {
    if (NULL == L) return 1;
    if (L->tag == ATOM) return 0;
    
    int maxDepth = 0;
    GLNode *p = L;
    while (p) {
        if (p->tag == LIST) {
            int depth = GListDepth(p->hp) + 1;
            if (depth > maxDepth) maxDepth = depth;
        }
        p = p->tp;
    }
    return maxDepth;
}

// 求广义表长度
int GListLength(GList L) {
    if (NULL == L) return 0;
    if (L->tag == ATOM) return 1;
    
    int len = 0;
    GLNode *p = L;
    while (p) {
        len++;
        p = p->tp;
    }
    return len;
}

// 打印广义表
void PrintGList(GList L) {
    if (NULL == L) {
        printf("()");
        return;
    }
    
    if (L->tag == ATOM) {
        printf("%d", L->atom);
        return;
    }
    
    printf("(");
    GLNode *p = L;
    while (p) {
        if (p->tag == LIST) {
            PrintGList(p->hp);
        } else {
            printf("%d", p->atom);
        }
        if (p->tp) printf(",");
        p = p->tp;
    }
    printf(")");
}

int main() {
    printf("========================================\n");
    printf("    多维数组与广义表\n");
    printf("========================================\n\n");
    
    // 二维数组
    printf("【二维数组】\n");
    int rows = 3, cols = 4;
    int *arr = Create2DArrayFlat(rows, cols);
    
    printf("填充数组：\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            SetElement2D(arr, rows, cols, i, j, i * cols + j);
            printf("%4d", GetElement2D(arr, rows, cols, i, j));
        }
        printf("\n");
    }
    delete[] arr;
    printf("\n");
    
    // 对称矩阵压缩
    printf("【对称矩阵压缩存储】\n");
    SymmetricMatrix sm(4);
    sm.Set(0, 0, 1);
    sm.Set(0, 1, 2);
    sm.Set(0, 2, 3);
    sm.Set(0, 3, 4);
    sm.Set(1, 1, 5);
    sm.Set(1, 2, 6);
    sm.Set(1, 3, 7);
    sm.Set(2, 2, 8);
    sm.Set(2, 3, 9);
    sm.Set(3, 3, 10);
    
    printf("对称矩阵：\n");
    sm.Print();
    printf("\n");
    
    // 广义表
    printf("【广义表】\n");
    // 构建广义表: (1, (2, 3), 4)
    GList atom1 = MakeAtom(1);
    GList atom2 = MakeAtom(2);
    GList atom3 = MakeAtom(3);
    GList list23 = MakeList(atom2, MakeList(atom3, NULL));
    GList atom4 = MakeAtom(4);
    
    GList list = MakeList(atom1, MakeList(list23, MakeList(atom4, NULL)));
    
    printf("广义表：");
    PrintGList(list);
    printf("\n");
    printf("长度：%d\n", GListLength(list));
    printf("深度：%d\n", GListDepth(list));
    
    printf("\n========================================\n");
    printf("【存储对比】\n");
    printf("普通矩阵：O(n^2)\n");
    printf("对称矩阵压缩：O(n(n+1)/2)\n");
    printf("稀疏矩阵三元组：O(nums)\n");
    
    getchar();
    return 0;
}