/*
 *（行优先存储，压缩存储，广义表）。多维数组按行优先或列优先方式存储，对称矩阵可压缩节省空间。
 *
 * 多维数组存储思路：
 * 1. 一维数组：连续线性存储
 * 2. 二维数组：行优先存储（C/C++默认）或列优先存储（Fortran默认）
 * 3. 多维数组：通过多层指针或一维模拟实现
 * 4. 对称矩阵：只存储下三角部分，节省一半空间
 * 5. 广义表：递归结构，可表示任意层次的列表
 *
 * 数据结构	存储方式	        空间复杂度	访问时间复杂度	特点
 * 普通矩阵	全存储	           O(n²)	    O(1)	        所有元素完整存储
 * 对称矩阵	下三角压缩	       O(n(n+1)/2)	O(1)	        利用对称性节省空间
 * 广义表	递归链表/数组结构	O(元素总数)	O(深度)	        可表示任意嵌套结构
 * 
 * 对称矩阵压缩示例（4×4对称矩阵）：
 *   原矩阵：
 *     1  2  3  4
 *     2  5  6  7
 *     3  6  8  9
 *     4  7  9 10
 * 
 *   下三角存储（一维数组，行优先）：
 *     下标：0   1   2   3   4   5   6   7   8   9
 *     值：  1   2   5   3   6   8   4   7   9  10
 *     对应：(0,0)(1,0)(1,1)(2,0)(2,1)(2,2)(3,0)(3,1)(3,2)(3,3)
 * 
 *   访问a[i][j]（i >= j）：
 *     索引 = i*(i+1)/2 + j
 *     例：a[3][1] = 3*4/2 + 1 = 7 → 值为7
 * 
 * 广义表示例：(1, (2, 3), 4)
 *   结构：
 *     LIST(头→1, 尾→LIST(头→LIST(头→2, 尾→LIST(头→3, 尾→NULL)), 尾→LIST(头→4, 尾→NULL)))
 *   深度：2（最深层嵌套为2）
 *   长度：3（第一层有3个元素）
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 多维数组（动态分配） ====================

/**
 * @brief 创建一维数组
 * 
 * @param n 数组长度
 * @return int* 数组指针
 */
int* Create1DArray(int n) {
    return new int[n];
}

/**
 * @brief 创建二维数组（指针数组）
 * 
 * @param rows 行数
 * @param cols 列数
 * @return int** 二维数组指针
 * 
 * @note 每行单独分配，内存不连续
 */
int** Create2DArray(int rows, int cols) {
    int **arr = new int*[rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[cols];
    }
    return arr;
}

/**
 * @brief 创建二维数组（一维模拟，行优先存储）
 * 
 * @param rows 行数
 * @param cols 列数
 * @return int* 一维数组指针
 * 
 * @note 内存连续，访问效率高
 */
int* Create2DArrayFlat(int rows, int cols) {
    return new int[rows * cols];
}

/**
 * @brief 获取二维数组元素（行优先存储）
 * 
 * @param arr  数组指针
 * @param rows 行数
 * @param cols 列数
 * @param i    行索引
 * @param j    列索引
 * @return int 元素值
 */
int GetElement2D(int *arr, int rows, int cols, int i, int j) {
    return arr[i * cols + j];
}

/**
 * @brief 设置二维数组元素（行优先存储）
 * 
 * @param arr  数组指针
 * @param rows 行数
 * @param cols 列数
 * @param i    行索引
 * @param j    列索引
 * @param val  值
 */
void SetElement2D(int *arr, int rows, int cols, int i, int j, int val) {
    arr[i * cols + j] = val;
}

/**
 * @brief 创建三维数组
 * 
 * @param dim1 第一维大小
 * @param dim2 第二维大小
 * @param dim3 第三维大小
 * @return int*** 三维数组指针
 */
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

/**
 * @brief 释放三维数组
 * 
 * @param arr  三维数组指针
 * @param dim1 第一维大小
 * @param dim2 第二维大小
 */
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

/**
 * @brief 对称矩阵压缩类（下三角存储）
 * 
 * @param data 压缩存储的一维数组
 * @param n    矩阵阶数
 */
class SymmetricMatrix {
private:
    int *data;
    int n;
    
    int GetIndex(int i, int j) {
        if (i < j) swap(i, j);
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

/**
 * @brief 广义表节点类型
 */
typedef enum { ATOM, LIST } ElemTag;

/**
 * @brief 广义表节点结构体
 * 
 * @param tag  节点类型（ATOM原子 / LIST列表）
 * @param atom 原子值（tag=ATOM时使用）
 * @param hp   表头指针（tag=LIST时使用）
 * @param tp   表尾指针
 */
typedef struct GLNode {
    ElemTag tag;
    union {
        int atom;
        struct GLNode *hp;
    };
    struct GLNode *tp;
} GLNode, *GList;

/**
 * @brief 创建原子节点
 * 
 * @param val 原子值
 * @return GList 节点指针
 */
GList MakeAtom(int val) {
    GLNode *p = (GLNode *)malloc(sizeof(GLNode));
    p->tag = ATOM;
    p->atom = val;
    p->tp = NULL;
    return p;
}

/**
 * @brief 创建列表节点
 * 
 * @param head 表头
 * @param tail 表尾
 * @return GList 节点指针
 */
GList MakeList(GList head, GList tail) {
    GLNode *p = (GLNode *)malloc(sizeof(GLNode));
    p->tag = LIST;
    p->hp = head;
    p->tp = tail;
    return p;
}

/**
 * @brief 求广义表深度
 * 
 * @param L 广义表
 * @return int 深度
 * 
 * @note 深度定义为最大嵌套层数
 * @note 原子深度为0，空表深度为1
 */
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

/**
 * @brief 求广义表长度
 * 
 * @param L 广义表
 * @return int 长度
 * 
 * @note 长度定义为第一层的元素个数
 */
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

/**
 * @brief 打印广义表
 * 
 * @param L 广义表
 */
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