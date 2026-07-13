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

// ==================== 树状数组（Fenwick Tree） ====================

class FenwickTree {
private:
    vector<int> tree;
    vector<int> original;  // 原始数组
    int n;
    
public:
    FenwickTree(vector<int> &arr) {
        n = arr.size();
        original = arr;
        tree.resize(n + 1, 0);
        
        // 初始化树状数组
        for (int i = 0; i < n; i++) {
            Add(i + 1, arr[i]);
        }
    }
    
    // 单点更新：将位置i增加delta
    void Add(int idx, int delta) {
        while (idx <= n) {
            tree[idx] += delta;
            idx += idx & -idx;
        }
    }
    
    // 前缀查询：[1, idx] 的和
    int Sum(int idx) {
        int result = 0;
        while (idx > 0) {
            result += tree[idx];
            idx -= idx & -idx;
        }
        return result;
    }
    
    // 区间查询：[l, r] 的和
    int RangeSum(int l, int r) {
        return Sum(r) - Sum(l - 1);
    }
    
    // 单点查询：获取原数组第i个元素
    int Get(int idx) {
        return original[idx - 1];
    }
    
    // 单点修改：将第i个元素改为val
    void Set(int idx, int val) {
        int delta = val - original[idx - 1];
        original[idx - 1] = val;
        Add(idx, delta);
    }
    
    // 打印树状数组结构
    void Print() {
        printf("树状数组：");
        for (int i = 1; i <= n; i++) {
            printf("%d ", tree[i]);
        }
        printf("\n");
    }
};

// ==================== 二维树状数组 ====================

class FenwickTree2D {
private:
    vector<vector<int>> tree;
    int n, m;
    
public:
    FenwickTree2D(int n, int m) : n(n), m(m) {
        tree.resize(n + 1, vector<int>(m + 1, 0));
    }
    
    void Add(int x, int y, int delta) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                tree[i][j] += delta;
            }
        }
    }
    
    int Sum(int x, int y) {
        int result = 0;
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) {
                result += tree[i][j];
            }
        }
        return result;
    }
    
    int RangeSum(int x1, int y1, int x2, int y2) {
        return Sum(x2, y2) - Sum(x1 - 1, y2) - Sum(x2, y1 - 1) + Sum(x1 - 1, y1 - 1);
    }
};

int main() {
    printf("========================================\n");
    printf("    树状数组（Fenwick Tree）\n");
    printf("========================================\n");
    printf("时间复杂度：O(logn)\n");
    printf("空间复杂度：O(n)\n");
    printf("适用：前缀和、动态更新\n\n");
    
    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15};
    printf("原数组：");
    for (int x : arr) printf("%d ", x);
    printf("\n\n");
    
    FenwickTree ft(arr);
    ft.Print();
    
    printf("\n查询结果：\n");
    printf("前缀和 [1, 5]：%d\n", ft.Sum(5));
    printf("区间和 [3, 7]：%d\n", ft.RangeSum(3, 7));
    
    ft.Set(4, 10);
    printf("\n将第4个元素改为10\n");
    printf("区间和 [3, 7]：%d\n", ft.RangeSum(3, 7));
    
    printf("\n【二维树状数组】\n");
    FenwickTree2D ft2d(4, 4);
    ft2d.Add(2, 3, 5);
    ft2d.Add(3, 2, 7);
    printf("二维树状数组查询 [1,1] 到 [3,3] 的和：%d\n", ft2d.RangeSum(1, 1, 3, 3));
    
    getchar();
    return 0;
}