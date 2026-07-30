/*
 *（三元组，压缩存储，矩阵转置）。只存储非零元素的行、列和值，大幅节省稀疏矩阵的存储空间。
 *
 * 稀疏矩阵压缩思路：
 * 1. 用三元组 (row, col, value) 表示每个非零元素
 * 2. 按行优先顺序存储所有非零元素
 * 3. 普通转置：按列扫描原矩阵，每次找到一列的所有非零元素
 * 4. 快速转置：预先统计每列的非零元素个数，确定每列在转置矩阵中的起始位置
 * 5. 矩阵加法：按行优先顺序合并两个矩阵的三元组
 *
 * 数据结构	存储方式	空间复杂度	访问时间复杂度	适用场景
 * 普通矩阵	全存储	    O(rows*cols)	O(1)	        稠密矩阵
 * 三元组	压缩存储	O(nums)	        O(log nums)  	稀疏矩阵
 * 
 * 稀疏矩阵示例（3×4矩阵，非零元素3个）：
 *   原矩阵：
 *      0  0  3  0
 *      0  5  0  0
 *      0  0  0  7
 * 
 *   三元组表（行优先）：
 *     行  列  值
 *     1   3   3
 *     2   2   5
 *     3   4   7
 * 
 * 普通转置过程（按列扫描）：
 *   第1列：无元素
 *   第2列：元素(2,2,5) → 转置为(2,2,5)
 *   第3列：元素(1,3,3) → 转置为(3,1,3)
 *   第4列：元素(3,4,7) → 转置为(4,3,7)
 * 
 *   转置结果：
 *      0  0  0  0
 *      0  0  0  0
 *      3  5  0  0
 *      0  0  7  0
 * 
 * 快速转置过程：
 *   第1步：统计每列非零元素个数
 *     col 1:0, col 2:1, col 3:1, col 4:1
 *   
 *   第2步：计算每列起始位置
 *     cpot[1]=1, cpot[2]=2, cpot[3]=3, cpot[4]=4
 *   
 *   第3步：遍历原三元组，直接放到目标位置
 *     (1,3,3) → col=3, cpot[3]=3 → 放第3位 → cpot[3]++=4
 *     (2,2,5) → col=2, cpot[2]=2 → 放第2位 → cpot[2]++=3
 *     (3,4,7) → col=4, cpot[4]=4 → 放第4位 → cpot[4]++=5
 *   
 *   转置结果三元组：
 *     行  列  值
 *     2   2   5
 *     3   1   3
 *     4   3   7
 * 
 * 矩阵加法 A+B：
 *   A: (1,3,3), (2,2,5)
 *   B: (1,3,2), (3,4,4)
 *   比较三元组：
 *     (1,3,3) vs (1,3,2) → 相同位置，值相加 3+2=5 → (1,3,5)
 *     (2,2,5) vs (3,4,4) → 位置不同，按行优先顺序合并
 *   C: (1,3,5), (2,2,5), (3,4,4)
 */


using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 稀疏矩阵的三元组表示 ====================

/**
 * @brief 三元组结构体
 * 
 * @param row   行号（从1开始）
 * @param col   列号（从1开始）
 * @param value 非零元素值
 */
typedef struct {
    int row;
    int col;
    int value;
} Triple;

/**
 * @brief 三元组稀疏矩阵结构体
 * 
 * @param data 三元组数组（从下标1开始存储）
 * @param rows 矩阵行数
 * @param cols 矩阵列数
 * @param nums 非零元素个数
 */
typedef struct {
    Triple *data;
    int rows;
    int cols;
    int nums;
} TSMatrix;

/**
 * @brief 创建稀疏矩阵
 * 
 * @param M 矩阵引用
 * @return Status 成功返回OK
 * 
 * @note 从标准输入读取矩阵信息
 */
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

/**
 * @brief 打印稀疏矩阵（完整形式）
 * 
 * @param M 矩阵
 */
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

/**
 * @brief 打印三元组表
 * 
 * @param M 矩阵
 */
void PrintTriple(TSMatrix M) {
    printf("行\t列\t值\n");
    for (int i = 1; i <= M.nums; i++) {
        printf("%d\t%d\t%d\n", M.data[i].row, M.data[i].col, M.data[i].value);
    }
}

/**
 * @brief 稀疏矩阵普通转置
 * 
 * @param M 原矩阵
 * @return TSMatrix 转置后的矩阵
 * 
 * @note 时间复杂度 O(rows * nums)，空间复杂度 O(nums)
 * @note 按列扫描原矩阵，效率较低
 */
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

/**
 * @brief 稀疏矩阵快速转置
 * 
 * @param M 原矩阵
 * @return TSMatrix 转置后的矩阵
 * 
 * @note 时间复杂度 O(nums + cols)，空间复杂度 O(nums + cols)
 * @note 使用辅助数组统计每列非零元素个数，效率更高
 */
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

/**
 * @brief 稀疏矩阵加法
 * 
 * @param A 矩阵A
 * @param B 矩阵B
 * @return TSMatrix 矩阵A+B
 * 
 * @note 时间复杂度 O(numsA + numsB)，空间复杂度 O(numsA + numsB)
 * @note 要求矩阵维度相同
 */
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