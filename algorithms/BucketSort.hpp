/*
 *（桶，插入排序，归并）。通过将数据分散到多个有序桶中，再归并所有桶得到完整有序序列。
 *
 * 桶排序思路：
 * 1. 创建 BUCKET_NUM 个空桶
 * 2. 将数组元素按区间分配到对应的桶中（每个桶覆盖一定数值范围）
 * 3. 向桶中插入元素时保持链表有序（插入排序）
 * 4. 依次归并所有桶中的元素，得到完整有序序列
 * 5. 将排序结果写回原数组
 *
 * 排序算法	平均时间复杂度	最差时间复杂度	空间复杂度	数据对象稳定性
 * 桶排序	  O(n+m)（均匀）	O(n²)（集中）	O(n+m)	    稳定
 * 数组：[29, 25, 3, 49, 9, 37, 21, 43]，BUCKET_NUM = 10，区间长度10
 * 
 * 桶分配（arr[i] / 10）：
 *   29 → 桶2
 *   25 → 桶2
 *   3  → 桶0
 *   49 → 桶4
 *   9  → 桶0
 *   37 → 桶3
 *   21 → 桶2
 *   43 → 桶4
 * 
 * 各桶元素（插入排序保持有序）：
 *   桶0：[3, 9]
 *   桶1：[]
 *   桶2：[21, 25, 29]
 *   桶3：[37]
 *   桶4：[43, 49]
 *   桶5：[]
 *   桶6：[]
 *   桶7：[]
 *   桶8：[]
 *   桶9：[]
 * 
 * 归并过程：
 *   桶0 [3,9] + 桶1 [] = [3,9]
 *   [3,9] + 桶2 [21,25,29] = [3,9,21,25,29]
 *   [3,9,21,25,29] + 桶3 [37] = [3,9,21,25,29,37]
 *   [3,9,21,25,29,37] + 桶4 [43,49] = [3,9,21,25,29,37,43,49]
 * 
 * 结果：[3, 9, 21, 25, 29, 37, 43, 49]
 * 
 * 桶排序适用场景：
 *   - 数据均匀分布在已知范围内
 *   - 数据量较大，且数值范围较小
 *   - 需要线性时间复杂度的排序
 *   - 不适合数据分布极不均匀的场景（会退化为O(n²)）
 */

#include <vector>

const int BUCKET_NUM = 10;

/**
 * @brief 桶排序中链表节点结构
 * 
 * 每个桶内部使用链表存储元素，在插入时保持有序
 */
struct BucketNode {
    explicit BucketNode(int i = 0) : mData(i), mNext(nullptr) {}
    BucketNode* mNext;
    int mData;
};

/**
 * @brief 向有序链表中插入新节点，保持链表有序（升序）
 * 
 * @param head  链表头节点指针
 * @param val   要插入的值
 * @return BucketNode* 插入后的链表头节点指针
 * 
 * @note 时间复杂度 O(k)，k 为链表长度
 * @note 使用哑节点简化边界条件处理
 */
BucketNode* insert(BucketNode* head, int val) {
    BucketNode dummyNode;
    BucketNode* newNode = new BucketNode(val);
    BucketNode *pre, *curr;
    
    dummyNode.mNext = head;
    pre = &dummyNode;
    curr = head;
    
    while (curr != nullptr && curr->mData <= val) {
        pre = curr;
        curr = curr->mNext;
    }
    
    newNode->mNext = curr;
    pre->mNext = newNode;
    
    return dummyNode.mNext;
}

/**
 * @brief 合并两个有序链表，返回合并后的链表头节点
 * 
 * @param head1 第一个有序链表的头节点
 * @param head2 第二个有序链表的头节点
 * @return BucketNode* 合并后链表的头节点指针
 * 
 * @note 时间复杂度 O(n1 + n2)，n1、n2 为两个链表的长度
 * @note 归并过程中不创建新节点，仅调整指针
 */
BucketNode* merge(BucketNode* head1, BucketNode* head2) {
    BucketNode dummyNode;
    BucketNode* tail = &dummyNode;
    
    while (head1 != nullptr && head2 != nullptr) {
        if (head1->mData <= head2->mData) {
            tail->mNext = head1;
            head1 = head1->mNext;
        } else {
            tail->mNext = head2;
            head2 = head2->mNext;
        }
        tail = tail->mNext;
    }
    
    if (head1 != nullptr) {
        tail->mNext = head1;
    }
    if (head2 != nullptr) {
        tail->mNext = head2;
    }
    
    return dummyNode.mNext;
}

/**
 * @brief 桶排序主函数
 * 
 * @param arr   待排序的整数数组
 * @param n     数组长度
 * 
 * @note 要求数据分布在 [0, BUCKET_NUM * 10) 范围内
 * @note 本实现使用 BUCKET_NUM = 10 个桶，每个桶覆盖区间长度为 10
 * @note 每个桶内部使用插入排序保持有序
 * @note 最后通过归并所有桶得到完整有序序列
 */
void BucketSort(int n, int arr[]) {
    std::vector<BucketNode*> buckets(BUCKET_NUM, nullptr);
    
    for (int i = 0; i < n; ++i) {
        int index = arr[i] / BUCKET_NUM;
        BucketNode* head = buckets[index];
        buckets[index] = insert(head, arr[i]);
    }
    
    BucketNode* head = buckets[0];
    for (int i = 1; i < BUCKET_NUM; ++i) {
        head = merge(head, buckets[i]);
    }
    
    for (int i = 0; i < n; ++i) {
        arr[i] = head->mData;
        head = head->mNext;
    }
}