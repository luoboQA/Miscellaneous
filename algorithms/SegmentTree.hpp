/*
 *（线段树，区间查询，懒标记）。通过构建二叉树结构，每个节点存储区间信息，支持高效的区间查询和区间更新。
 *
 * 线段树思路：
 * 1. 将数组区间递归地分成两半，构建一棵完全二叉树
 * 2. 每个节点存储对应区间的信息（如区间和、最大值等）
 * 3. 区间查询：从根节点开始，根据查询区间与节点区间的关系，决定返回节点值或递归查询子节点
 * 4. 单点更新：从根节点找到目标叶子节点，更新后回溯更新祖先节点
 * 5. 区间更新（带懒标记）：当更新区间完全覆盖节点区间时，仅更新当前节点并记录懒标记，不立即更新子节点
 *
 * 数据结构	操作	    时间复杂度	空间复杂度	适用场景
 * 线段树	建树	    O(n)	    O(4n)	    区间查询、区间更新
 *         单点更新	O(log n)	O(4n)	    
 *         区间查询	O(log n)	O(4n)	    
 *         区间更新	O(log n)	O(4n)	    
 * 
 * 线段树示例（数组：[1, 3, 5, 7, 9, 11]）
 * 
 * 线段树结构（区间和）：
 *                    [0,5] sum=36
 *                   /           \
 *              [0,2] sum=9     [3,5] sum=27
 *              /     \          /     \
 *          [0,1]    [2,2]   [3,4]    [5,5]
 *          sum=4     sum=5   sum=16    sum=11
 *          /   \            /   \
 *      [0,0]  [1,1]    [3,3]  [4,4]
 *      sum=1   sum=3    sum=7   sum=9
 * 
 * 建树过程：
 *   叶子节点：arr[0]=1, arr[1]=3, arr[2]=5, arr[3]=7, arr[4]=9, arr[5]=11
 *   父节点 = 左子节点和 + 右子节点和
 *   [0,1] = 1+3=4, [2,2]=5, [3,4]=7+9=16, [5,5]=11
 *   [0,2] = 4+5=9, [3,5] = 16+11=27
 *   [0,5] = 9+27=36
 * 
 * 区间查询 [1, 4]（从根节点开始）：
 *   第1步：节点[0,5] 部分覆盖 → 递归左右子节点
 *   第2步：节点[0,2] 部分覆盖 → 递归左子节点[0,1]和右子节点[2,2]
 *   第3步：节点[0,1] 部分覆盖 → 递归子节点[0,0]和[1,1]
 *   第4步：节点[0,0] 不在范围内 → 返回0
 *          节点[1,1] 完全在范围内 → 返回3
 *          节点[2,2] 完全在范围内 → 返回5
 *   第5步：节点[3,5] 部分覆盖 → 递归左右子节点
 *   第6步：节点[3,4] 完全在范围内 → 返回16
 *          节点[5,5] 不在范围内 → 返回0
 *   结果：3 + 5 + 16 = 24（即 arr[1]+arr[2]+arr[3]+arr[4] = 3+5+7+9 = 24）
 * 
 * 单点更新 Update(2, 10)：
 *   找到叶子节点[2,2]，将值从5改为10
 *   回溯更新：[0,1]不变为4，[0,2] = 4+10 = 14，[0,5] = 14+27 = 41
 * 
 * 区间更新（带懒标记）RangeUpdate(1, 3, 2)：
 *   将区间[1,3]每个元素加2
 *   更新节点[1,1]：3→5
 *   更新节点[2,2]：5→7（若懒标记直接更新，则记录lazy）
 *   更新节点[3,3]：7→9
 *   更新祖先节点和
 * 
 * 懒标记优化：
 *   当更新区间完全覆盖节点区间时，不立即更新子节点
 *   而是记录懒标记，等需要查询子节点时再下推
 *   将区间更新时间复杂度从 O(n) 优化到 O(log n)
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 线段树（区间求和） ====================

/**
 * @brief 线段树类（支持区间求和）
 * 
 * @param tree  线段树数组（存储区间和）
 * @param lazy  懒标记数组（延迟更新的增量）
 * @param n     原始数组大小
 */
class SegmentTree {
private:
    vector<int> tree;
    vector<int> lazy;
    int n;
    
    /**
     * @brief 建树
     * 
     * @param arr   原始数组
     * @param node  当前节点编号
     * @param start 当前节点区间左端点
     * @param end   当前节点区间右端点
     * 
     * @note 时间复杂度 O(n)
     */
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
    
    /**
     * @brief 区间查询
     * 
     * @param node  当前节点编号
     * @param start 当前节点区间左端点
     * @param end   当前节点区间右端点
     * @param l     查询区间左端点
     * @param r     查询区间右端点
     * @return int  查询结果
     * 
     * @note 时间复杂度 O(log n)
     */
    int Query(int node, int start, int end, int l, int r) {
        if (r < start || l > end) return 0;
        if (l <= start && end <= r) return tree[node];
        
        PushDown(node, start, end);
        int mid = (start + end) / 2;
        return Query(node * 2, start, mid, l, r) +
               Query(node * 2 + 1, mid + 1, end, l, r);
    }
    
    /**
     * @brief 单点更新
     * 
     * @param node  当前节点编号
     * @param start 当前节点区间左端点
     * @param end   当前节点区间右端点
     * @param idx   要更新的位置
     * @param val   新值
     * 
     * @note 时间复杂度 O(log n)
     */
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
    
    /**
     * @brief 区间更新（带懒标记）
     * 
     * @param node  当前节点编号
     * @param start 当前节点区间左端点
     * @param end   当前节点区间右端点
     * @param l     更新区间左端点
     * @param r     更新区间右端点
     * @param val   增量
     * 
     * @note 时间复杂度 O(log n)
     * @note 使用懒标记延迟更新，提高效率
     */
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
    
    /**
     * @brief 下推懒标记
     * 
     * @param node  当前节点编号
     * @param start 当前节点区间左端点
     * @param end   当前节点区间右端点
     * 
     * @note 将当前节点的懒标记下推给子节点
     */
    void PushDown(int node, int start, int end) {
        if (lazy[node] == 0) return;
        int mid = (start + end) / 2;
        int leftChild = node * 2;
        int rightChild = node * 2 + 1;
        
        tree[leftChild] += (mid - start + 1) * lazy[node];
        lazy[leftChild] += lazy[node];
        
        tree[rightChild] += (end - mid) * lazy[node];
        lazy[rightChild] += lazy[node];
        
        lazy[node] = 0;
    }
    
public:
    /**
     * @brief 构造函数
     * 
     * @param arr 原始数组
     */
    SegmentTree(vector<int> &arr) {
        n = arr.size();
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
        Build(arr, 1, 0, n - 1);
    }
    
    /**
     * @brief 区间查询接口
     * 
     * @param l 查询区间左端点
     * @param r 查询区间右端点
     * @return int 区间和
     */
    int Query(int l, int r) {
        return Query(1, 0, n - 1, l, r);
    }
    
    /**
     * @brief 单点更新接口
     * 
     * @param idx 要更新的位置
     * @param val 新值
     */
    void Update(int idx, int val) {
        Update(1, 0, n - 1, idx, val);
    }
    
    /**
     * @brief 区间更新接口
     * 
     * @param l   更新区间左端点
     * @param r   更新区间右端点
     * @param val 增量
     */
    void RangeUpdate(int l, int r, int val) {
        RangeUpdate(1, 0, n - 1, l, r, val);
    }
};