/*
 *（斐波那契数列，黄金分割，有序数组）。利用斐波那契数列确定分割点，在有序数组中快速定位目标值。
 *
 * 斐波那契查找思路：
 * 1. 构造斐波那契数列 F[0]=0, F[1]=1, F[i]=F[i-1]+F[i-2]
 * 2. 找到第一个满足 F[k]-1 >= n 的 k，将数组扩展到 F[k]-1 长度
 * 3. 计算分割点 mid = low + F[k-1] - 1
 * 4. 若 key < temp[mid]，在左半部分继续查找，k -= 1
 * 5. 若 key > temp[mid]，在右半部分继续查找，k -= 2
 * 6. 若 key == temp[mid]，返回位置（注意处理扩展元素）
 *
 * 查找算法	    平均时间复杂度	空间复杂度	查找条件
 * 斐波那契查找	O(log₂n)	O(1)	    有序（升序）
 * 
 * 斐波那契查找示例（数组：[0, 16, 24, 35, 47, 59, 62, 73, 88, 99]）
 * 
 * 斐波那契数列：F[0]=0, F[1]=1, F[2]=1, F[3]=2, F[4]=3, F[5]=5, F[6]=8, F[7]=13, ...
 * 
 * n=10，找到第一个 F[k]-1 >= 10 → F[6]-1=7 < 10, F[7]-1=12 >= 10 → k=7
 * 扩展数组到 F[7]-1 = 12 长度，末尾填充最大值99
 * temp = [0, 16, 24, 35, 47, 59, 62, 73, 88, 99, 99, 99]
 * 
 * 查找目标值 47：
 *   low=0, high=9, k=7
 *   mid = low + F[6] - 1 = 0 + 8 - 1 = 7
 *   47 < temp[7]=73 → high=6, k=6
 * 
 *   low=0, high=6, k=6
 *   mid = low + F[5] - 1 = 0 + 5 - 1 = 4
 *   47 == temp[4]=47 → 找到！返回索引4
 * 
 * 查找目标值 100：
 *   low=0, high=9, k=7
 *   mid = 7, 100 > temp[7]=73 → low=8, k=5
 * 
 *   low=8, high=9, k=5
 *   mid = 8 + F[4] - 1 = 8 + 3 - 1 = 10
 *   100 == temp[10]=99 → mid=10 >= n → 返回 n-1=9
 *   实际数组中最大值在索引9
 * 
 * 斐波那契查找 vs 二分查找：
 *   斐波那契查找只使用加减运算，不使用除法，适合CPU不支持除法的情况
 *   二分查找每次将区间对半分割，效率略高于斐波那契查找
 */

#include "stdafx.h"
#include <memory>
#include <iostream>
#include <cstring>
using namespace std;

const int max_size = 20;  // 斐波那契数组的长度

/**
 * @brief 构造斐波那契数组
 * 
 * @param F 斐波那契数组
 * 
 * @note F[0]=0, F[1]=1, F[i]=F[i-1]+F[i-2]
 */
void Fibonacci(int * F)
{
    F[0] = 0;
    F[1] = 1;
    for (int i = 2; i < max_size; ++i)
        F[i] = F[i - 1] + F[i - 2];
}

/**
 * @brief 斐波那契查找
 * 
 * @param a   待查找的有序数组
 * @param n   数组长度
 * @param key 要查找的关键字
 * @return int 找到返回索引，未找到返回-1
 * 
 * @note 时间复杂度 O(log n)，空间复杂度 O(1)
 * @note 要求数组已按升序排列
 * @note 使用斐波那契数列确定分割点，只使用加减运算
 */
int FibonacciSearch(int *a, int n, int key)
{
    int low = 0;
    int high = n - 1;
  
    int F[max_size];
    Fibonacci(F);  // 构造斐波那契数组

    int k = 0;
    while (n > F[k] - 1)  // 计算n位于斐波那契数列的位置
        ++k;

    int *temp;  // 将数组a扩展到F[k]-1的长度
    temp = new int[F[k] - 1];
    memcpy(temp, a, n * sizeof(int));

    for (int i = n; i < F[k] - 1; ++i)
        temp[i] = a[n - 1];
  
    while (low <= high) {
        int mid = low + F[k - 1] - 1;
        if (key < temp[mid]) {
            high = mid - 1;
            k -= 1;
        } else if (key > temp[mid]) {
            low = mid + 1;
            k -= 2;
        } else {
            if (mid < n)
                return mid;  // 若相等则说明mid即为查找到的位置
            else
                return n - 1;  // 若mid>=n则说明是扩展的数值，返回n-1
        }
    }  
    delete[] temp;
    return -1;
}