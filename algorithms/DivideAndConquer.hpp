/*
 *（分解，解决，合并）。将大问题递归地分解为相同的小问题，分别解决后再合并结果。
 *
 * 分治法思路：
 * 1. 分解（Divide）：将原问题分解为若干个规模较小的子问题
 * 2. 解决（Conquer）：递归地解决每个子问题，若子问题足够小则直接求解
 * 3. 合并（Combine）：将子问题的解合并成原问题的解
 *
 * 分治算法	          时间复杂度	        空间复杂度	          适用场景
 * 快速排序	          O(n log n)平均	O(log n)	        大规模无序数据排序
 * 归并排序	          O(n log n)	        O(n)	            大规模数据排序（稳定）
 * 循环赛日程安排	  O(n²)	            O(n²)	            比赛赛程安排
 * 二分查找	          O(log n)	        O(log n)递归/ O(1)迭代	有序数组查找
 * 最大子数组和	  O(n log n)	        O(log n)	        寻找最大连续子数组
 * 
 * 分治法示例：
 * 
 * 【快速排序】数组：[64, 34, 25, 12, 22, 11, 90]
 *   第1层：选基准64 → [34,25,12,22,11] 64 [90]
 *   第2层：左[34,25,12,22,11]选基准34 → [25,12,22,11] 34 []
 *          右[90]已有序
 *   第3层：左[25,12,22,11]选基准25 → [12,22,11] 25 []
 *   第4层：左[12,22,11]选基准12 → [11] 12 [22]
 *   合并结果：[11, 12, 22, 25, 34, 64, 90]
 * 
 * 【归并排序】数组：[38, 27, 43, 3, 9, 82, 10]
 *   分解：
 *     [38,27,43,3,9,82,10] → [38,27,43,3] + [9,82,10]
 *     [38,27,43,3] → [38,27] + [43,3]
 *     [38,27] → [38] + [27]
 *     [43,3] → [43] + [3]
 *     [9,82,10] → [9] + [82,10]
 *     [82,10] → [82] + [10]
 *   合并：
 *     [38] + [27] → [27,38]
 *     [43] + [3] → [3,43]
 *     [27,38] + [3,43] → [3,27,38,43]
 *     [82] + [10] → [10,82]
 *     [9] + [10,82] → [9,10,82]
 *     [3,27,38,43] + [9,10,82] → [3,9,10,27,38,43,82]
 * 
 * 【循环赛日程安排】8人参赛
 *   分治过程：8人 → 4人 → 2人 → 1人（直接安排）
 *   合并：将两个4人赛程表合并为8人赛程表
 *   最终生成7天赛程，每人每天比赛一场
 * 
 * 【二分查找】数组：[1,3,5,7,9,11,13,15]，查找7
 *   第1层：mid=3 → v[3]=7 == 7 找到
 * 
 * 【最大子数组和】数组：[-2,-5,6,-2,-3,1,5,-6]
 *   第1层：mid=3 → 左[-2,-5,6,-2] 右[-3,1,5,-6] 跨中[6,-2,-3,1,5]和=7
 *   第2层：左最大=6，右最大=6
 *   第3层：跨中最大=7
 *   结果：最大子数组和 = 7（子数组：[6,-2,-3,1,5]）
 */

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

// ==================== 1. 快速排序 ====================

/**
 * @brief 快速排序分区函数
 * 
 * @param v    待排序向量
 * @param low  左边界
 * @param high 右边界
 * @return int 基准元素最终位置
 * 
 * @note 选择第一个元素作为基准
 * @note 将小于基准的放左边，大于基准的放右边
 * @note 时间复杂度 O(n)
 */
int Partition(vector<int> &v, int low, int high) {
    int pivot = v[low];
    while (low < high) {
        while (low < high && v[high] >= pivot) high--;
        v[low] = v[high];
        while (low < high && v[low] <= pivot) low++;
        v[high] = v[low];
    }
    v[low] = pivot;
    return low;
}

/**
 * @brief 快速排序
 * 
 * @param v    待排序向量
 * @param low  左边界
 * @param high 右边界
 * 
 * @note 平均时间复杂度 O(n log n)，最坏 O(n²)
 * @note 空间复杂度 O(log n)（递归调用栈）
 * @note 不稳定排序
 */
void QuickSort(vector<int> &v, int low, int high) {
    if (low < high) {
        int pivotPos = Partition(v, low, high);
        QuickSort(v, low, pivotPos - 1);
        QuickSort(v, pivotPos + 1, high);
    }
}

// ==================== 2. 归并排序 ====================

/**
 * @brief 归并两个有序子数组
 * 
 * @param v    待合并向量
 * @param low  左边界
 * @param mid  中间位置
 * @param high 右边界
 * 
 * @note 合并 [low, mid] 和 [mid+1, high] 两个有序子数组
 * @note 时间复杂度 O(n)，空间复杂度 O(n)
 */
void Merge(vector<int> &v, int low, int mid, int high) {
    vector<int> temp(high - low + 1);
    int i = low, j = mid + 1, k = 0;
    
    while (i <= mid && j <= high) {
        if (v[i] <= v[j]) temp[k++] = v[i++];
        else temp[k++] = v[j++];
    }
    while (i <= mid) temp[k++] = v[i++];
    while (j <= high) temp[k++] = v[j++];
    
    for (i = low, k = 0; i <= high; i++, k++) {
        v[i] = temp[k];
    }
}

/**
 * @brief 归并排序
 * 
 * @param v    待排序向量
 * @param low  左边界
 * @param high 右边界
 * 
 * @note 时间复杂度 O(n log n)，空间复杂度 O(n)
 * @note 稳定排序
 */
void MergeSort(vector<int> &v, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSort(v, low, mid);
        MergeSort(v, mid + 1, high);
        Merge(v, low, mid, high);
    }
}

// ==================== 3. 循环赛日程安排 ====================

/**
 * @brief 生成循环赛日程表
 * 
 * @param table 日程表二维数组
 * @param n     参赛人数（必须为2的幂）
 * 
 * @note 分治思想：n人 → n/2人 → n/4人 → ... → 1人
 * @note 时间复杂度 O(n²)，空间复杂度 O(n²)
 * @note table[i][j] 表示第i名选手在第j天对阵的选手编号（从1开始）
 */
void Schedule(int **table, int n) {
    if (n == 1) {
        table[0][0] = 1;
        return;
    }
    
    int half = n / 2;
    Schedule(table, half);
    
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            table[i + half][j] = table[i][j] + half;
            table[i][j + half] = table[i][j] + half;
            table[i + half][j + half] = table[i][j];
        }
    }
}

/**
 * @brief 打印循环赛日程表
 * 
 * @param n 参赛人数
 * 
 * @note 仅用于测试和演示
 */
void PrintSchedule(int n) {
    int **table = new int*[n];
    for (int i = 0; i < n; i++) {
        table[i] = new int[n];
    }
    
    Schedule(table, n);
    
    printf("循环赛日程安排表（%d人）：\n", n);
    printf("选手\\天");
    for (int i = 0; i < n; i++) {
        printf("%4d", i + 1);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%4d", i + 1);
        for (int j = 0; j < n; j++) {
            printf("%4d", table[i][j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < n; i++) {
        delete[] table[i];
    }
    delete[] table;
}

// ==================== 4. 二分查找（分治法） ====================

/**
 * @brief 二分查找（分治递归版本）
 * 
 * @param v     有序向量
 * @param value 目标值
 * @param low   左边界
 * @param high  右边界
 * @return int  找到返回索引，未找到返回-1
 * 
 * @note 时间复杂度 O(log n)，空间复杂度 O(log n)（递归调用栈）
 * @note 要求向量已按升序排列
 */
int BinarySearch_DC(vector<int> v, int value, int low, int high) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (v[mid] == value) return mid;
    else if (v[mid] > value) return BinarySearch_DC(v, value, low, mid - 1);
    else return BinarySearch_DC(v, value, mid + 1, high);
}

// ==================== 5. 最大子数组和 ====================

/**
 * @brief 计算跨越中点的最大子数组和
 * 
 * @param v    向量
 * @param low  左边界
 * @param mid  中点
 * @param high 右边界
 * @return int 跨越中点的最大子数组和
 * 
 * @note 时间复杂度 O(n)
 * @note 必须包含中点 mid 和 mid+1 之间的边
 */
int MaxCrossingSum(vector<int> &v, int low, int mid, int high) {
    int leftSum = -99999;
    int sum = 0;
    for (int i = mid; i >= low; i--) {
        sum += v[i];
        if (sum > leftSum) leftSum = sum;
    }
    
    int rightSum = -99999;
    sum = 0;
    for (int i = mid + 1; i <= high; i++) {
        sum += v[i];
        if (sum > rightSum) rightSum = sum;
    }
    
    return leftSum + rightSum;
}

/**
 * @brief 最大子数组和（分治递归版本）
 * 
 * @param v    向量
 * @param low  左边界
 * @param high 右边界
 * @return int 最大子数组和
 * 
 * @note 时间复杂度 O(n log n)，空间复杂度 O(log n)
 * @note 分治思想：最大子数组出现在左半、右半或跨越中点
 */
int MaxSubArraySum(vector<int> &v, int low, int high) {
    if (low == high) return v[low];
    
    int mid = (low + high) / 2;
    int leftSum = MaxSubArraySum(v, low, mid);
    int rightSum = MaxSubArraySum(v, mid + 1, high);
    int crossSum = MaxCrossingSum(v, low, mid, high);
    
    return max(max(leftSum, rightSum), crossSum);
}