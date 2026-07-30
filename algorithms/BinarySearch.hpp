/*
 *（左边界，右边界，中间值）。通过不断缩小搜索区间，在有序数组中快速定位目标值。
 *
 * 二分查找思路：
 * 1. 初始化左边界 low = 0，右边界 high = n - 1
 * 2. 计算中间位置 mid = low + (high - low) / 2
 * 3. 比较 v[mid] 与目标值 value：
 *    - 若相等，找到目标，返回 mid
 *    - 若 v[mid] > value，目标在左半区间，high = mid - 1
 *    - 若 v[mid] < value，目标在右半区间，low = mid + 1
 * 4. 重复步骤2-3，直到 low > high（未找到）
 *
 * 查找算法	平均时间复杂度	空间复杂度	查找条件
 * 二分查找	O(log₂n)	O(1)或O(log n)	有序（升序）
 * 数组：[1, 3, 5, 7, 9, 11, 13]，查找目标值 7
 * 
 * 第1步：low=0, high=6
 *   mid = 0 + (6-0)/2 = 3
 *   v[3] = 7 == 7 → 找到！返回索引 3
 * 
 * 数组：[1, 3, 5, 7, 9, 11, 13]，查找目标值 6
 * 
 * 第1步：low=0, high=6
 *   mid = 0 + (6-0)/2 = 3
 *   v[3] = 7 > 6 → 目标在左半区间，high = 2
 * 
 * 第2步：low=0, high=2
 *   mid = 0 + (2-0)/2 = 1
 *   v[1] = 3 < 6 → 目标在右半区间，low = 2
 * 
 * 第3步：low=2, high=2
 *   mid = 2 + (2-2)/2 = 2
 *   v[2] = 5 < 6 → 目标在右半区间，low = 3
 * 
 * 第4步：low=3, high=2
 *   low > high → 未找到，返回 -1
 * 
 * 数组：[1, 3, 5, 7, 9, 11, 13]，查找目标值 1（边界值）
 * 
 * 第1步：low=0, high=6
 *   mid = 0 + (6-0)/2 = 3
 *   v[3] = 7 > 1 → 目标在左半区间，high = 2
 * 
 * 第2步：low=0, high=2
 *   mid = 0 + (2-0)/2 = 1
 *   v[1] = 3 > 1 → 目标在左半区间，high = 0
 * 
 * 第3步：low=0, high=0
 *   mid = 0 + (0-0)/2 = 0
 *   v[0] = 1 == 1 → 找到！返回索引 0
 * 
 * 数组：[1, 3, 5, 7, 9, 11, 13]，查找目标值 13（边界值）
 * 
 * 第1步：low=0, high=6
 *   mid = 0 + (6-0)/2 = 3
 *   v[3] = 7 < 13 → 目标在右半区间，low = 4
 * 
 * 第2步：low=4, high=6
 *   mid = 4 + (6-4)/2 = 5
 *   v[5] = 11 < 13 → 目标在右半区间，low = 6
 * 
 * 第3步：low=6, high=6
 *   mid = 6 + (6-6)/2 = 6
 *   v[6] = 13 == 13 → 找到！返回索引 6
 * 
 * 结果：找到返回索引，未找到返回 -1
 */

#include <vector>
using namespace std;

/**
 * @brief 二分查找算法（非递归版本）
 * 
 * @param v        有序数组（常量引用，避免拷贝）
 * @param value    要查找的目标值
 * @param low      搜索区间左边界（默认为 0）
 * @param high     搜索区间右边界（默认为 v.size() - 1）
 * 
 * @return int     找到返回索引；未找到返回 -1
 * 
 * @note  时间复杂度 O(log n)，空间复杂度 O(1)
 * @note  要求数组已按升序排列
 * 
 */
int BinarySearch(const vector<int>& v, int value, 
                 int low = 0, int high = -1) {
    // 处理空数组和默认参数
    if (v.empty()) {
        return -1;
    }
    if (high == -1) {
        high = v.size() - 1;
    }
    
    while (low <= high) {
        int mid = low + (high - low) / 2;  // 防止溢出
        if (v[mid] == value) {
            return mid;
        }
        else if (v[mid] > value) { // 目标值在左半区间 [low, mid-1]
            high = mid - 1;
        }
        else { // 目标值在右半区间 [mid+1, high]
            low = mid + 1;
        }
    }
    return -1;
}

/**
 * @brief 二分查找算法（递归版本）
 * 
 * @param v        有序数组（常量引用，不拷贝）
 * @param value    要查找的目标值
 * @param low      搜索区间左边界索引（闭区间）
 * @param high     搜索区间右边界索引（闭区间）
 * 
 * @return int     找到返回目标值的索引；未找到返回 -1
 * 
 * @note  时间复杂度 O(log n)
 * @note  空间复杂度 O(log n)（仅递归调用栈）
 * @note  要求数组已按升序排列
 *
 */
int BinarySearch2(const vector<int>& v, int value, int low, int high) {
 
    if (low > high) {
        return -1;
    }
    
   
    int mid = low + (high - low) / 2; // 防止溢出
    
  
    if (v[mid] == value) {
        return mid;
    }
    else if (v[mid] > value) {
        // 目标值在左半区间
        return BinarySearch2(v, value, low, mid - 1);
    }
    else {
        // 目标值在右半区间
        return BinarySearch2(v, value, mid + 1, high);
    }
}