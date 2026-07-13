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

// ==================== 稀疏矩阵的三元组表示 ====================

typedef struct {
    int row;
    int col;
    int value;
} Triple;

typedef struct {
    Triple *data;
    int rows;
    int cols;
    int nums;  // 非零元素个数
} TSMatrix;

// 创建稀疏矩阵
Status CreateSMatrix(TSMatrix &M) {
    printf("请输入矩阵行数、列数、非零元素个数：");
    scanf("%d %d %d", &M.rows, &M.cols, &M.nums);
    
    M.data = (Triple *)malloc((M.nums + 1) * sizeof(Triple));
    if (NULL == M.data) return OVERFLOW;
    
    printf("请输入%d个非零元素（行 列 值）：\n", M.nums);
    for (int i = 1; i <= M.nums; i++) {
        scanf("%d %d %d", &M.data[i].row, &M.data[i].col, &M.data[i].value);
    }
    return OK;
}

// 打印稀疏矩阵
void PrintSMatrix(TSMatrix M) {
    int k = 1;
    for (int i = 1; i <= M.rows; i++) {
        for (int j = 1; j <= M.cols; j++) {
            if (k <= M.nums && M.data[k].row == i && M.data[k].col == j) {
                printf("%4d", M.data[k].value);
                k++;
            } else {
                printf("%4d", 0);
            }
        }
        printf("\n");
    }
}

// 打印三元组表
void PrintTriple(TSMatrix M) {
    printf("行\t列\t值\n");
    for (int i = 1; i <= M.nums; i++) {
        printf("%d\t%d\t%d\n", M.data[i].row, M.data[i].col, M.data[i].value);
    }
}

// 稀疏矩阵转置（普通转置 O(rows*cols)）
TSMatrix TransposeSMatrix(TSMatrix M) {
    TSMatrix T;
    T.rows = M.cols;
    T.cols = M.rows;
    T.nums = M.nums;
    T.data = (Triple *)malloc((T.nums + 1) * sizeof(Triple));
    
    if (T.nums > 0) {
        int q = 1;
        for (int col = 1; col <= M.cols; col++) {
            for (int p = 1; p <= M.nums; p++) {
                if (M.data[p].col == col) {
                    T.data[q].row = M.data[p].col;
                    T.data[q].col = M.data[p].row;
                    T.data[q].value = M.data[p].value;
                    q++;
                }
            }
        }
    }
    return T;
}

// 稀疏矩阵快速转置（O(nums + cols)）
TSMatrix FastTransposeSMatrix(TSMatrix M) {
    TSMatrix T;
    T.rows = M.cols;
    T.cols = M.rows;
    T.nums = M.nums;
    T.data = (Triple *)malloc((T.nums + 1) * sizeof(Triple));
    
    int *num = (int *)malloc((M.cols + 1) * sizeof(int));
    int *cpot = (int *)malloc((M.cols + 1) * sizeof(int));
    
    for (int col = 1; col <= M.cols; col++) num[col] = 0;
    
    for (int p = 1; p <= M.nums; p++) {
        num[M.data[p].col]++;
    }
    
    cpot[1] = 1;
    for (int col = 2; col <= M.cols; col++) {
        cpot[col] = cpot[col - 1] + num[col - 1];
    }
    
    for (int p = 1; p <= M.nums; p++) {
        int col = M.data[p].col;
        int q = cpot[col];
        T.data[q].row = M.data[p].col;
        T.data[q].col = M.data[p].row;
        T.data[q].value = M.data[p].value;
        cpot[col]++;
    }
    
    free(num);
    free(cpot);
    return T;
}

// 稀疏矩阵加法
TSMatrix AddSMatrix(TSMatrix A, TSMatrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        printf("矩阵维度不匹配！\n");
        TSMatrix empty = {NULL, 0, 0, 0};
        return empty;
    }
    
    TSMatrix C;
    C.rows = A.rows;
    C.cols = A.cols;
    C.data = (Triple *)malloc((A.nums + B.nums + 1) * sizeof(Triple));
    
    int i = 1, j = 1, k = 1;
    while (i <= A.nums && j <= B.nums) {
        if (A.data[i].row < B.data[j].row ||
            (A.data[i].row == B.data[j].row && A.data[i].col < B.data[j].col)) {
            C.data[k++] = A.data[i++];
        } else if (A.data[i].row > B.data[j].row ||
                   (A.data[i].row == B.data[j].row && A.data[i].col > B.data[j].col)) {
            C.data[k++] = B.data[j++];
        } else {
            int sum = A.data[i].value + B.data[j].value;
            if (sum != 0) {
                C.data[k].row = A.data[i].row;
                C.data[k].col = A.data[i].col;
                C.data[k].value = sum;
                k++;
            }
            i++;
            j++;
        }
    }
    
    while (i <= A.nums) C.data[k++] = A.data[i++];
    while (j <= B.nums) C.data[k++] = B.data[j++];
    
    C.nums = k - 1;
    return C;
}

int main() {
    printf("========================================\n");
    printf("    稀疏矩阵（Sparse Matrix）\n");
    printf("========================================\n\n");
    
    TSMatrix M;
    CreateSMatrix(M);
    
    printf("\n原矩阵：\n");
    PrintSMatrix(M);
    
    printf("\n三元组表：\n");
    PrintTriple(M);
    
    TSMatrix T = TransposeSMatrix(M);
    printf("\n转置后矩阵：\n");
    PrintSMatrix(T);
    
    TSMatrix TF = FastTransposeSMatrix(M);
    printf("\n快速转置后矩阵：\n");
    PrintSMatrix(TF);
    
    printf("\n【矩阵加法测试】\n");
    TSMatrix A, B;
    // 创建两个矩阵用于测试
    A.rows = 3; A.cols = 3; A.nums = 2;
    A.data = (Triple *)malloc(3 * sizeof(Triple));
    A.data[1] = {1, 1, 3};
    A.data[2] = {2, 3, 5};
    
    B.rows = 3; B.cols = 3; B.nums = 2;
    B.data = (Triple *)malloc(3 * sizeof(Triple));
    B.data[1] = {1, 1, 2};
    B.data[2] = {3, 2, 4};
    
    printf("矩阵A：\n");
    PrintSMatrix(A);
    printf("矩阵B：\n");
    PrintSMatrix(B);
    
    TSMatrix C = AddSMatrix(A, B);
    printf("A+B：\n");
    PrintSMatrix(C);
    
    getchar();
    getchar();
    return 0;
}