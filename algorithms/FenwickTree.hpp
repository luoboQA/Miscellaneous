/*
 *（树状数组，lowbit，前缀和）。通过维护树状结构，支持单点更新和前缀查询操作。
 *
 * 树状数组（Fenwick Tree）思路：
 * 1. 树状数组tree[i]维护原数组以i结尾的区间和，区间长度为 lowbit(i)
 * 2. 单点更新：从idx开始，不断加上lowbit(idx)，更新沿途节点
 * 3. 前缀查询：从idx开始，不断减去lowbit(idx)，累加沿途节点值
 * 4. 区间查询：Sum(r) - Sum(l-1)
 *
 * 数据结构	操作	    时间复杂度	空间复杂度	适用场景
 * 树状数组	单点更新	O(log n)	O(n)	    动态前缀和、区间求和
 *         前缀查询	O(log n)	O(n)	    
 *         区间查询	O(log n)	O(n)	    
 * 
 * 树状数组示例（原数组：[1, 3, 5, 7, 9, 11, 13, 15]）
 * 
 * 注意：C++ 代码中 arr 下标从 0 开始，tree 下标从 1 开始
 * 
 * 树状数组构建：
 *   tree[1] = arr[0] = 1
 *   tree[2] = arr[0] + arr[1] = 1 + 3 = 4
 *   tree[3] = arr[2] = 5
 *   tree[4] = arr[0] + arr[1] + arr[2] + arr[3] = 1+3+5+7 = 16
 *   tree[5] = arr[4] = 9
 *   tree[6] = arr[4] + arr[5] = 9 + 11 = 20
 *   tree[7] = arr[6] = 13
 *   tree[8] = arr[0] + arr[1] + arr[2] + arr[3] + arr[4] + arr[5] + arr[6] + arr[7] = 64
 * 
 * tree数组： [1, 4, 5, 16, 9, 20, 13, 64]
 *          （下标从1开始，tree[0]不用）
 * 
 * 
  * 树状数组逻辑结构（tree 下标从 1 开始，arr 下标从 0 开始）：
 *
 *                          tree[8]
 *                         [1, 8]
 *                        /      \
 *                   tree[4]    tree[6]
 *                  [1, 4]     [5, 6]
 *                 /      \    /      \
 *            tree[2]  tree[3] tree[5] tree[7]
 *           [1, 2]   [3, 3]  [5, 5]  [7, 7]
 *           /     \
 *      tree[1]   (无)
 *     [1, 1]
 *
 * 对应关系（arr 下标从 0 开始）：
 *   tree[1] 覆盖 arr[0]                 → [1, 1]
 *   tree[2] 覆盖 arr[0] + arr[1]        → [1, 2]
 *   tree[3] 覆盖 arr[2]                 → [3, 3]
 *   tree[4] 覆盖 arr[0] + arr[1] + arr[2] + arr[3] → [1, 4]
 *   tree[5] 覆盖 arr[4]                 → [5, 5]
 *   tree[6] 覆盖 arr[4] + arr[5]        → [5, 6]
 *   tree[7] 覆盖 arr[6]                 → [7, 7]
 *   tree[8] 覆盖 arr[0] 到 arr[7]       → [1, 8]
 * 注意：
 *   图中 [l, r] 表示该 tree 节点覆盖原数组 arr[l..r] 的区间和
 *   这里的 l, r 是 0-based 下标
 *   [1, 1] 表示 tree[1] 覆盖的是原数组的第 1 个位置（下标从 1 开始计数的第 1 个元素）
 * 
 * 前缀查询 Sum(7)：
 *   idx=7: result += tree[7]=13, idx -= lowbit(7)=1 → idx=6
 *   idx=6: result += tree[6]=20, idx -= lowbit(6)=2 → idx=4
 *   idx=4: result += tree[4]=16, idx -= lowbit(4)=4 → idx=0
 *   result = 13 + 20 + 16 = 49（即 1+3+5+7+9+11+13）
 * 
 * 区间查询 RangeSum(3, 7)：
 *   Sum(7) - Sum(2) = 49 - (1+3=4) = 45（即 5+7+9+11+13）
 * 
 * 单点更新 Set(4, 10)（原值7改为10，delta=3）：
 *   idx=4: tree[4]+=3 → 16+3=19, idx += lowbit(4)=4 → idx=8
 *   idx=8: tree[8]+=3 → 64+3=67, idx += lowbit(8)=8 → idx=16
 *   tree变为：[1, 4, 5, 19, 9, 20, 13, 67]
 * 
 * 二维树状数组：
 *   在一维的基础上扩展，支持二维前缀和查询
 *   单点更新：(x,y) → 两层循环更新
 *   前缀查询：(x,y) → 两层循环累加
 *   区间查询：容斥原理 Sum(x2,y2) - Sum(x1-1,y2) - Sum(x2,y1-1) + Sum(x1-1,y1-1)
 */


using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 树状数组（Fenwick Tree） ====================

/**
 * @brief 树状数组类
 * 
 * @param tree    树状数组存储结构
 * @param original 原始数组
 * @param n       数组大小
 * 
 * @note tree下标从1开始，0号位置不使用,因为lowbit(0) = 0 & -0 = 0,如果 idx=0，while (idx <= n) 会死循环
 */
class FenwickTree {
private:
    vector<int> tree;
    vector<int> original;
    int n;
    
public:
    /**
     * @brief 构造函数，从数组构建树状数组
     * 
     * @param arr 原始数组
     * 
     * @note 时间复杂度 O(n log n)
     */
    FenwickTree(vector<int> &arr) {
        n = arr.size();
        original = arr;
        tree.resize(n + 1, 0);
        
        for (int i = 0; i < n; i++) {
            Add(i + 1, arr[i]);
        }
    }
    
    /**
     * @brief 单点更新：将位置idx增加delta
     * 
     * @param idx   位置（从1开始）
     * @param delta 增量
     * 
     * @note 时间复杂度 O(log n)
     */
    
    void Add(int idx, int delta) {
        while (idx <= n) {
            tree[idx] += delta;
            idx += idx & -idx; 
            /* lowbit(idx)  Add(3, 5) 变化 x=3 x=4 x=8
            x=1  (001) → lowbit=1  (001)
            x=2  (010) → lowbit=2  (010)
            x=3  (011) → lowbit=1  (001)
            x=4  (100) → lowbit=4  (100)
            x=5  (101) → lowbit=1  (001)    
            x=6  (110) → lowbit=2  (010)
            x=7  (111) → lowbit=1  (001)
            x=8  (1000) → lowbit=8 (1000)*/                 
        }
    }
    
    /**
     * @brief 前缀查询：[1, idx] 的和
     * 
     * @param idx 位置（从1开始）
     * @return int 前缀和
     * 
     * @note 时间复杂度 O(log n)
     */
    int Sum(int idx) {
        int result = 0;
        while (idx > 0) {
            result += tree[idx];
            idx -= idx & -idx;
        }
        return result;
    }
    
    /**
     * @brief 区间查询：[l, r] 的和
     * 
     * @param l 左边界（从1开始）
     * @param r 右边界（从1开始）
     * @return int 区间和
     * 
     * @note 时间复杂度 O(log n)
     */
    int RangeSum(int l, int r) {
        return Sum(r) - Sum(l - 1);
    }
    
    /**
     * @brief 单点查询：获取原数组第idx个元素
     * 
     * @param idx 位置（从1开始）
     * @return int 元素值
     */
    int Get(int idx) {
        return original[idx - 1];
    }
    
    /**
     * @brief 单点修改：将第idx个元素改为val
     * 
     * @param idx 位置（从1开始）
     * @param val 新值
     * 
     * @note 时间复杂度 O(log n)
     */
    void Set(int idx, int val) {
        int delta = val - original[idx - 1];
        original[idx - 1] = val;
        Add(idx, delta);
    }
    
    /**
     * @brief 打印树状数组结构
     */
    void Print() {
        printf("树状数组：");
        for (int i = 1; i <= n; i++) {
            printf("%d ", tree[i]);
        }
        printf("\n");
    }
};

// ==================== 二维树状数组 ====================

/**
 * @brief 二维树状数组类
 * 
 * @param tree 二维树状数组
 * @param n    行数
 * @param m    列数
 */
class FenwickTree2D {
private:
    vector<vector<int>> tree;
    int n, m;
    
public:
    /**
     * @brief 构造函数
     * 
     * @param n 行数
     * @param m 列数
     */
    FenwickTree2D(int n, int m) : n(n), m(m) {
        tree.resize(n + 1, vector<int>(m + 1, 0));
    }
    
    /**
     * @brief 二维单点更新
     * 
     * @param x     行坐标
     * @param y     列坐标
     * @param delta 增量
     * 
     * @note 时间复杂度 O(log n * log m)
     */
    void Add(int x, int y, int delta) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                tree[i][j] += delta;
            }
        }
    }
    
    /**
     * @brief 二维前缀查询
     * 
     * @param x 行坐标
     * @param y 列坐标
     * @return int 前缀和
     * 
     * @note 时间复杂度 O(log n * log m)
     */
    int Sum(int x, int y) {
        int result = 0;
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) {
                result += tree[i][j];
            }
        }
        return result;
    }
    
    /**
     * @brief 二维区间查询（容斥原理）
     * 
     * @param x1 左上角行
     * @param y1 左上角列
     * @param x2 右下角行
     * @param y2 右下角列
     * @return int 区间和
     * 
     * @note 时间复杂度 O(log n * log m)
     */
    int RangeSum(int x1, int y1, int x2, int y2) {
        return Sum(x2, y2) - Sum(x1 - 1, y2) - Sum(x2, y1 - 1) + Sum(x1 - 1, y1 - 1);
    }
};