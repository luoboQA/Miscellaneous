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

// ========== 最大堆 ==========

class MaxHeap {
private:
    vector<ElemType> heap;  // 从下标1开始存储
    int capacity;
    int size;
    
    // 向上调整（插入时使用）
    void ShiftUp(int i) {
        while (i > 1 && heap[i] > heap[i / 2]) {
            swap(heap[i], heap[i / 2]);
            i /= 2;
        }
    }
    
    // 向下调整（删除时使用）
    void ShiftDown(int i) {
        while (i * 2 <= size) {
            int larger = i * 2;
            if (larger + 1 <= size && heap[larger + 1] > heap[larger]) {
                larger = larger + 1;
            }
            if (heap[i] >= heap[larger]) break;
            swap(heap[i], heap[larger]);
            i = larger;
        }
    }
    
public:
    MaxHeap(int cap = 100) {
        capacity = cap;
        size = 0;
        heap.resize(cap + 1);
        heap[0] = 0;  // 哨兵
    }
    
    // 从数组构建堆
    MaxHeap(vector<ElemType> &arr) {
        size = arr.size();
        capacity = size * 2;
        heap.resize(capacity + 1);
        heap[0] = 0;
        
        for (int i = 0; i < size; i++) {
            heap[i + 1] = arr[i];
        }
        
        // 从最后一个非叶子节点开始向下调整
        for (int i = size / 2; i >= 1; i--) {
            ShiftDown(i);
        }
    }
    
    // 插入元素
    void Insert(ElemType e) {
        if (size >= capacity) {
            capacity *= 2;
            heap.resize(capacity + 1);
        }
        heap[++size] = e;
        ShiftUp(size);
    }
    
    // 删除最大值
    ElemType DeleteMax() {
        if (size == 0) return -1;
        ElemType maxVal = heap[1];
        heap[1] = heap[size--];
        ShiftDown(1);
        return maxVal;
    }
    
    // 获取最大值（不删除）
    ElemType GetMax() {
        if (size == 0) return -1;
        return heap[1];
    }
    
    // 获取堆大小
    int GetSize() {
        return size;
    }
    
    // 是否为空
    bool IsEmpty() {
        return size == 0;
    }
    
    // 打印堆
    void Print() {
        for (int i = 1; i <= size; i++) {
            printf("%d ", heap[i]);
        }
        printf("\n");
    }
    
    // 堆排序（非降序）
    void HeapSort() {
        vector<ElemType> result;
        int originalSize = size;
        while (size > 0) {
            result.push_back(DeleteMax());
        }
        size = originalSize;
        // 恢复原堆
        for (int i = 0; i < result.size(); i++) {
            heap[i + 1] = result[i];
        }
        // 重新构建堆
        for (int i = size / 2; i >= 1; i--) {
            ShiftDown(i);
        }
        // 输出排序结果（从大到小）
        printf("堆排序结果：");
        for (int i = 0; i < result.size(); i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    }
};

// ========== 最小堆 ==========

class MinHeap {
private:
    vector<ElemType> heap;
    int size;
    int capacity;
    
    void ShiftUp(int i) {
        while (i > 1 && heap[i] < heap[i / 2]) {
            swap(heap[i], heap[i / 2]);
            i /= 2;
        }
    }
    
    void ShiftDown(int i) {
        while (i * 2 <= size) {
            int smaller = i * 2;
            if (smaller + 1 <= size && heap[smaller + 1] < heap[smaller]) {
                smaller = smaller + 1;
            }
            if (heap[i] <= heap[smaller]) break;
            swap(heap[i], heap[smaller]);
            i = smaller;
        }
    }
    
public:
    MinHeap(int cap = 100) {
        capacity = cap;
        size = 0;
        heap.resize(cap + 1);
        heap[0] = 0;
    }
    
    void Insert(ElemType e) {
        if (size >= capacity) {
            capacity *= 2;
            heap.resize(capacity + 1);
        }
        heap[++size] = e;
        ShiftUp(size);
    }
    
    ElemType DeleteMin() {
        if (size == 0) return -1;
        ElemType minVal = heap[1];
        heap[1] = heap[size--];
        ShiftDown(1);
        return minVal;
    }
    
    ElemType GetMin() {
        if (size == 0) return -1;
        return heap[1];
    }
    
    int GetSize() { return size; }
    bool IsEmpty() { return size == 0; }
    
    void Print() {
        for (int i = 1; i <= size; i++) {
            printf("%d ", heap[i]);
        }
        printf("\n");
    }
};

int main() {
    printf("========================================\n");
    printf("    堆（Heap）数据结构\n");
    printf("========================================\n\n");
    
    printf("【最大堆】\n");
    MaxHeap maxHeap;
    int arr[] = {3, 5, 1, 4, 2, 8, 6};
    printf("插入序列：");
    for (int i = 0; i < 7; i++) {
        printf("%d ", arr[i]);
        maxHeap.Insert(arr[i]);
    }
    printf("\n");
    
    printf("堆内元素：");
    maxHeap.Print();
    printf("堆顶（最大值）：%d\n", maxHeap.GetMax());
    printf("堆大小：%d\n", maxHeap.GetSize());
    
    printf("\n删除堆顶元素：%d\n", maxHeap.DeleteMax());
    printf("删除后堆内：");
    maxHeap.Print();
    
    maxHeap.HeapSort();
    
    printf("\n【最小堆】\n");
    MinHeap minHeap;
    int arr2[] = {3, 5, 1, 4, 2, 8, 6};
    for (int i = 0; i < 7; i++) {
        minHeap.Insert(arr2[i]);
    }
    printf("最小堆：");
    minHeap.Print();
    printf("堆顶（最小值）：%d\n", minHeap.GetMin());
    printf("删除最小元素：%d\n", minHeap.DeleteMin());
    printf("删除后：");
    minHeap.Print();
    
    printf("\n【堆的应用】\n");
    printf("1. 优先队列\n");
    printf("2. 堆排序（O(nlogn)）\n");
    printf("3. 求TopK问题\n");
    printf("4. 找中位数\n");
    
    getchar();
    return 0;
}