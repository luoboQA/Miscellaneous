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

void QuickSort(vector<int> &v, int low, int high) {
    if (low < high) {
        int pivotPos = Partition(v, low, high);
        QuickSort(v, low, pivotPos - 1);
        QuickSort(v, pivotPos + 1, high);
    }
}

// ==================== 2. 归并排序 ====================

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

void MergeSort(vector<int> &v, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSort(v, low, mid);
        MergeSort(v, mid + 1, high);
        Merge(v, low, mid, high);
    }
}

// ==================== 3. 循环赛日程安排 ====================

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

int BinarySearch_DC(vector<int> v, int value, int low, int high) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (v[mid] == value) return mid;
    else if (v[mid] > value) return BinarySearch_DC(v, value, low, mid - 1);
    else return BinarySearch_DC(v, value, mid + 1, high);
}

// ==================== 5. 最大子数组和 ====================

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

int MaxSubArraySum(vector<int> &v, int low, int high) {
    if (low == high) return v[low];
    
    int mid = (low + high) / 2;
    int leftSum = MaxSubArraySum(v, low, mid);
    int rightSum = MaxSubArraySum(v, mid + 1, high);
    int crossSum = MaxCrossingSum(v, low, mid, high);
    
    return max(max(leftSum, rightSum), crossSum);
}

int main() {
    printf("========================================\n");
    printf("    分治法（Divide and Conquer）\n");
    printf("========================================\n");
    printf("思想：将大问题分解为相同的小问题\n");
    printf("步骤：分解 -> 解决 -> 合并\n\n");
    
    // 1. 快速排序
    printf("【1. 快速排序】\n");
    vector<int> v1 = {64, 34, 25, 12, 22, 11, 90};
    printf("排序前：");
    for (int x : v1) printf("%d ", x);
    QuickSort(v1, 0, v1.size() - 1);
    printf("\n排序后：");
    for (int x : v1) printf("%d ", x);
    printf("\n\n");
    
    // 2. 归并排序
    printf("【2. 归并排序】\n");
    vector<int> v2 = {38, 27, 43, 3, 9, 82, 10};
    printf("排序前：");
    for (int x : v2) printf("%d ", x);
    MergeSort(v2, 0, v2.size() - 1);
    printf("\n排序后：");
    for (int x : v2) printf("%d ", x);
    printf("\n\n");
    
    // 3. 循环赛日程安排
    printf("【3. 循环赛日程安排】\n");
    PrintSchedule(8);
    printf("\n");
    
    // 4. 二分查找
    printf("【4. 二分查找】\n");
    vector<int> v4 = {1, 3, 5, 7, 9, 11, 13, 15};
    printf("数组：");
    for (int x : v4) printf("%d ", x);
    int pos = BinarySearch_DC(v4, 7, 0, v4.size() - 1);
    printf("\n查找7的位置：%d\n", pos);
    pos = BinarySearch_DC(v4, 8, 0, v4.size() - 1);
    printf("查找8的位置：%d\n\n", pos);
    
    // 5. 最大子数组和
    printf("【5. 最大子数组和】\n");
    vector<int> v5 = {-2, -5, 6, -2, -3, 1, 5, -6};
    printf("数组：");
    for (int x : v5) printf("%d ", x);
    int maxSum = MaxSubArraySum(v5, 0, v5.size() - 1);
    printf("\n最大子数组和：%d\n", maxSum);
    
    getchar();
    return 0;
}