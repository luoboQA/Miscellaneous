/*
 *（分治，合并，有序子序列）。将数组递归分为两半，分别排序后合并为有序序列。
 *
 * 归并排序思路：
 * 1. 将数组分成两半
 * 2. 分别对左半部分和右半部分进行排序（递归）
 * 3. 将两个有序子数组合并成一个有序数组
 *
 * 排序算法	平均时间复杂度	最差时间复杂度	最好时间复杂度	空间复杂度	数据对象稳定性
 * 归并排序	O(n*log₂n)	O(n*log₂n)	O(n*log₂n)	O(n)	    稳定
 * 
 * 数组：[8, 4, 5, 7, 1, 3, 6, 2]
 * 
 * 分解阶段：
 *   [8,4,5,7,1,3,6,2]
 *    /            \
 *   [8,4,5,7]    [1,3,6,2]
 *    /    \        /    \
 *  [8,4]  [5,7]  [1,3]  [6,2]
 *  /  \   /  \   /  \   /  \
 * [8][4] [5][7] [1][3] [6][2]
 * 
 * 合并阶段（自底向上）：
 *  [4,8] [5,7] [1,3] [2,6]
 *    \    /      \    /
 *  [4,5,7,8]    [1,2,3,6]
 *       \          /
 *    [1,2,3,4,5,6,7,8]
 * 
 * 结果：[1, 2, 3, 4, 5, 6, 7, 8]
 * 
 * 迭代版（自底向上）：
 *   seg=1：合并相邻单个元素 → [4,8,5,7,1,3,2,6]
 *   seg=2：合并相邻2个元素 → [4,5,7,8,1,2,3,6]
 *   seg=4：合并相邻4个元素 → [1,2,3,4,5,6,7,8]
 */

/**
 * @brief 归并排序（迭代版，自底向上）
 * 
 * @tparam T 元素类型（需支持 < 比较操作）
 * @param arr 待排序数组
 * @param len 数组长度
 * 
 * @note 时间复杂度 O(n*log₂n)，空间复杂度 O(n)
 * @note 稳定排序算法
 */
template<typename T>
void merge_sort_iterative(T arr[], int len) {
    T* a = arr;
    T* b = new T[len];
    
    for (int seg = 1; seg < len; seg += seg) {
        for (int start = 0; start < len; start += seg + seg) {
            int low = start;
            int mid = std::min(start + seg, len);
            int high = std::min(start + seg + seg, len);
            
            int k = low;
            int start1 = low, end1 = mid;
            int start2 = mid, end2 = high;
            
            while (start1 < end1 && start2 < end2) {
                b[k++] = a[start1] < a[start2] ? a[start1++] : a[start2++];
            }
            while (start1 < end1) {
                b[k++] = a[start1++];
            }
            while (start2 < end2) {
                b[k++] = a[start2++];
            }
        }
        
        T* temp = a;
        a = b;
        b = temp;
    }
    
    if (a != arr) {
        for (int i = 0; i < len; i++) {
            b[i] = a[i];
        }
        b = a;
    }
    
    delete[] b;
}

/**
 * @brief 归并排序（递归版，自顶向下）内部递归函数
 * 
 * @tparam T 元素类型
 * @param arr 待排序数组
 * @param reg 辅助数组
 * @param start 区间起始下标
 * @param end 区间结束下标
 */
template<typename T>
void merge_sort_recursive(T arr[], T reg[], int start, int end) {
    if (start >= end) {
        return;
    }
    
    int len = end - start;
    int mid = (len >> 1) + start;
    int start1 = start, end1 = mid;
    int start2 = mid + 1, end2 = end;
    
    merge_sort_recursive(arr, reg, start1, end1);
    merge_sort_recursive(arr, reg, start2, end2);
    
    int k = start;
    while (start1 <= end1 && start2 <= end2) {
        reg[k++] = arr[start1] < arr[start2] ? arr[start1++] : arr[start2++];
    }
    while (start1 <= end1) {
        reg[k++] = arr[start1++];
    }
    while (start2 <= end2) {
        reg[k++] = arr[start2++];
    }
    
    for (k = start; k <= end; k++) {
        arr[k] = reg[k];
    }
}

/**
 * @brief 归并排序（递归版，自顶向下）
 * 
 * @tparam T 元素类型（需支持 < 比较操作）
 * @param arr 待排序数组
 * @param len 数组长度
 * 
 * @note 时间复杂度 O(n*log₂n)，空间复杂度 O(n)
 * @note 稳定排序算法
 */
template<typename T>
void merge_sort_recursive_wrapper(T arr[], const int len) {
    T* reg = new T[len];
    merge_sort_recursive(arr, reg, 0, len - 1);
    delete[] reg;
}