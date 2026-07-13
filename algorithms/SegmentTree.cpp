#include <iostream>
#include <vector>
#include <climits>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 线段树（区间求和） ====================

class SegmentTree {
private:
    vector<int> tree;
    vector<int> lazy;
    int n;
    
    // 建树
    void Build(vector<int> &arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = (start + end) / 2;
        Build(arr, node * 2, start, mid);
        Build(arr, node * 2 + 1, mid + 1, end);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }
    
    // 区间查询
    int Query(int node, int start, int end, int l, int r) {
        if (r < start || l > end) return 0;
        if (l <= start && end <= r) return tree[node];
        
        PushDown(node, start, end);
        int mid = (start + end) / 2;
        return Query(node * 2, start, mid, l, r) +
               Query(node * 2 + 1, mid + 1, end, l, r);
    }
    
    // 单点更新
    void Update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) {
            Update(node * 2, start, mid, idx, val);
        } else {
            Update(node * 2 + 1, mid + 1, end, idx, val);
        }
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }
    
    // 区间更新（带懒标记）
    void RangeUpdate(int node, int start, int end, int l, int r, int val) {
        if (r < start || l > end) return;
        if (l <= start && end <= r) {
            tree[node] += (end - start + 1) * val;
            lazy[node] += val;
            return;
        }
        
        PushDown(node, start, end);
        int mid = (start + end) / 2;
        RangeUpdate(node * 2, start, mid, l, r, val);
        RangeUpdate(node * 2 + 1, mid + 1, end, l, r, val);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }
    
    // 下推懒标记
    void PushDown(int node, int start, int end) {
        if (lazy[node] == 0) return;
        int mid = (start + end) / 2;
        int leftChild = node * 2;
        int rightChild = node * 2 + 1;
        
        // 更新左子树
        tree[leftChild] += (mid - start + 1) * lazy[node];
        lazy[leftChild] += lazy[node];
        
        // 更新右子树
        tree[rightChild] += (end - mid) * lazy[node];
        lazy[rightChild] += lazy[node];
        
        lazy[node] = 0;
    }
    
public:
    SegmentTree(vector<int> &arr) {
        n = arr.size();
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
        Build(arr, 1, 0, n - 1);
    }
    
    // 区间查询
    int Query(int l, int r) {
        return Query(1, 0, n - 1, l, r);
    }
    
    // 单点更新
    void Update(int idx, int val) {
        Update(1, 0, n - 1, idx, val);
    }
    
    // 区间更新
    void RangeUpdate(int l, int r, int val) {
        RangeUpdate(1, 0, n - 1, l, r, val);
    }
};

int main() {
    printf("========================================\n");
    printf("    线段树（Segment Tree）\n");
    printf("========================================\n");
    printf("时间复杂度：O(logn)\n");
    printf("空间复杂度：O(4n)\n");
    printf("适用：区间查询、区间更新\n\n");
    
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    printf("原数组：");
    for (int x : arr) printf("%d ", x);
    printf("\n\n");
    
    SegmentTree st(arr);
    
    printf("区间查询 [1, 4] 的和：%d\n", st.Query(1, 4));
    printf("区间查询 [0, 5] 的和：%d\n", st.Query(0, 5));
    
    st.Update(2, 10);
    printf("\n将下标2的值改为10\n");
    printf("区间查询 [1, 4] 的和：%d\n", st.Query(1, 4));
    
    st.RangeUpdate(1, 3, 2);
    printf("\n将区间 [1, 3] 每个元素加2\n");
    printf("区间查询 [0, 5] 的和：%d\n", st.Query(0, 5));
    
    getchar();
    return 0;
}