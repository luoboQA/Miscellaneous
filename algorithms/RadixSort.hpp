/*
 *（最低位优先，计数排序，分配收集）。从个位开始逐位进行计数排序，经过d次排序后完全有序。
 *
 * 基数排序思路（LSD）：
 * 1. 找出数组中最大数的位数 d
 * 2. 从个位开始，按当前位进行计数排序
 * 3. 依次处理十位、百位...直到最高位
 * 4. 经过 d 次排序后，数组完全有序
 *
 * 排序算法	平均时间复杂度	最差时间复杂度	空间复杂度	数据对象稳定性
 * 基数排序	O(d*n)	        O(d*n)	        O(n+r)	    稳定
 * 
 * 数组：[329, 457, 657, 839, 436, 720, 355]
 * 
 * 第1次排序（个位）：
 *   按个位分配：
 *     0: 720
 *     5: 355
 *     6: 436
 *     7: 457, 657
 *     9: 329, 839
 *   收集后：[720, 355, 436, 457, 657, 329, 839]
 * 
 * 第2次排序（十位）：
 *   按十位分配：
 *     2: 329
 *     3: 436, 839
 *     5: 355, 457, 657
 *     7: 720
 *   收集后：[329, 436, 839, 355, 457, 657, 720]
 * 
 * 第3次排序（百位）：
 *   按百位分配：
 *     3: 329, 355
 *     4: 436, 457
 *     6: 657
 *     7: 720
 *     8: 839
 *   收集后：[329, 355, 436, 457, 657, 720, 839]
 * 
 * 结果：[329, 355, 436, 457, 657, 720, 839]
 * 
 * 计数排序在每一位的应用：
 *   以个位为例：[329, 457, 657, 839, 436, 720, 355]
 *   count[0]=1, count[5]=1, count[6]=1, count[7]=2, count[9]=2
 *   累加：count[0]=1, count[5]=2, count[6]=3, count[7]=5, count[9]=7
 *   反向填充：720→355→436→657→457→839→329
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

/**
 * @brief 求数组中最大数的位数
 * 
 * @param data 待排序数组
 * @param n    数组长度
 * @return int 最大数的位数
 * 
 * @note 时间复杂度 O(n)
 */
int maxbit(int data[], int n) {
    int maxData = data[0];
    for (int i = 1; i < n; ++i) {
        if (maxData < data[i]) {
            maxData = data[i];
        }
    }
    
    int d = 1;
    while (maxData >= 10) {
        maxData /= 10;
        ++d;
    }
    return d;
}

/**
 * @brief 基数排序主函数（LSD，最低位优先）
 * 
 * @param data 待排序数组
 * @param n    数组长度
 * 
 * @note 时间复杂度 O(d*n)，空间复杂度 O(n+10)
 * @note 稳定排序算法
 * @note 要求所有元素为非负整数
 */
void radixsort(int data[], int n) {
    int d = maxbit(data, n);
    
    int* tmp = new int[n];
    int* count = new int[10];
    
    int radix = 1;
    
    for (int i = 1; i <= d; i++) {
        for (int j = 0; j < 10; j++) {
            count[j] = 0;
        }
        
        for (int j = 0; j < n; j++) {
            int k = (data[j] / radix) % 10;
            count[k]++;
        }
        
        for (int j = 1; j < 10; j++) {
            count[j] += count[j - 1];
        }
        
        for (int j = n - 1; j >= 0; j--) {
            int k = (data[j] / radix) % 10;
            tmp[count[k] - 1] = data[j];
            count[k]--;
        }
        
        for (int j = 0; j < n; j++) {
            data[j] = tmp[j];
        }
        
        radix *= 10;
    }
    
    delete[] tmp;
    delete[] count;
}