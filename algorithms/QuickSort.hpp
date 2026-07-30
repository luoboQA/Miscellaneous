/*
 *（基准，分区）。通过选择基准元素，将数组分为小于基准和大于等于基准两部分，递归排序。
 *
 * 快速排序思路：
 * 1. 从数列中挑选一个元素作为基准（pivot）
 * 2. 将小于基准的元素放在基准前面，大于等于基准的放在基准后面（分区操作）
 * 3. 递归地对基准前后的子序列进行排序
 *
 * 排序算法	平均时间复杂度	最差时间复杂度	空间复杂度	数据对象稳定性
 * 快速排序	  O(n*log₂n)	 O(n²)	        O(log₂n)	不稳定
 * 数组：[5, 2, 4, 6, 1, 3]
 * 
 * 第1轮（基准=3，最后一个元素）：
 *   左指针left=0，右指针right=4（end-1）
 *   left找≥3：arr[0]=5≥3 → left=0
 *   right找<3：arr[4]=1<3 → right=4
 *   交换arr[0]和arr[4] → [1, 2, 4, 6, 5, 3]
 *   left=1，right=3
 *   left找≥3：arr[1]=2<3 → left=2
 *   right找<3：arr[3]=6≥3 → right=2
 *   left=2，right=2 → 退出循环
 *   arr[2]=4≥arr[5]=3 → 交换 → [1, 2, 3, 6, 5, 4]
 *   基准3已就位 ✓
 *   左子区间：[1, 2]  右子区间：[6, 5, 4]
 * 
 * 第2轮（左区间基准=2）：
 *   [1, 2] → left找≥2：arr[1]=2≥2 → left=1
 *   right找<2：arr[0]=1<2 → right=0
 *   交换arr[1]和arr[0] → [2, 1]
 *   left=1，right=0 → 退出循环
 *   arr[1]=1≥arr[1]=2？不成立 → left++ → left=2
 *   基准2已就位 ✓
 *   左子区间：[1]（已有序）
 * 
 * 第3轮（右区间基准=4）：
 *   [6, 5, 4] → left找≥4：arr[0]=6≥4 → left=0
 *   right找<4：arr[1]=5≥4 → right=0
 *   left=0，right=0 → 退出循环
 *   arr[0]=6≥arr[2]=4 → 交换 → [4, 5, 6]
 *   基准4已就位 ✓
 *   右子区间：[5, 6]
 * 
 * 第4轮（右子区间基准=6）：
 *   [5, 6] → left找≥6：arr[1]=6≥6 → left=1
 *   right找<6：arr[0]=5<6 → right=0
 *   交换arr[1]和arr[0] → [6, 5]
 *   left=1，right=0 → 退出循环
 *   arr[1]=5≥arr[1]=6？不成立 → left++ → left=2
 *   基准6已就位 ✓
 *   左子区间：[5]（已有序）
 * 
 * 结果：[1, 2, 3, 4, 5, 6]
 */

/**
 * @brief 快速排序（递归版本）
 * 
 * @tparam T 元素类型（需支持 < 和 >= 比较操作）
 * @param arr 待排序数组（指针）
 * @param len 数组长度
 * 
 * @note 时间复杂度 O(n*log₂n)，空间复杂度 O(log₂n)
 * @note 不稳定排序算法，相同元素的相对顺序可能改变
 * @note 选择最后一个元素作为基准，适用于整数、浮点数及支持比较操作的自定义类型
 * @note 递归深度最坏为 O(n)，大数据量建议使用迭代版本
 */
template <typename T>
void quick_sort_recursive(T arr[], int start, int end) {
    if (start >= end) {
        return;
    }
    
    T pivot = arr[end];           // 选择最后一个元素作为基准
    int left = start;
    int right = end - 1;
    
    // 分区操作：将小于 pivot 的放左边，大于等于 pivot 的放右边
    while (left < right) {
        // 从左向右找第一个不小于 pivot 的元素
        while (arr[left] < pivot && left < right) {
            left++;
        }
        // 从右向左找第一个小于 pivot 的元素
        while (arr[right] >= pivot && left < right) {
            right--;
        }
        // 交换这对"错位"的元素
        std::swap(arr[left], arr[right]);
    }
    
    // 将基准放到正确位置（left 或 left+1）
    if (arr[left] >= arr[end]) {
        std::swap(arr[left], arr[end]);
    } else {
        left++;   // 基准比 arr[left] 大，应放在 left 的右边
    }
    
    // 递归排序左右子区间（不包含已就位的基准）
    quick_sort_recursive(arr, start, left - 1);
    quick_sort_recursive(arr, left + 1, end);
}

template <typename T>
void quick_sort(T arr[], int len) {
    quick_sort_recursive(arr, 0, len - 1);
}

/**
 * @brief 快速排序（迭代版本）
 * 
 * @tparam T 元素类型（需支持 < 和 >= 比较操作）
 * @param arr 待排序数组（指针）
 * @param len 数组长度
 * 
 * @note 使用栈模拟递归，避免递归调用栈溢出风险
 * @note 时间复杂度 O(n*log₂n)，空间复杂度 O(log₂n)
 * @note 适用于数据量极大的场景
 */
template <typename T>
void quick_sort_iterative(T arr[], const int len) {
    if (len <= 0) {
        return;
    }
    
    // 使用栈模拟递归，存储待处理的子区间 [start, end]
    struct Range {
        int start, end;
        Range(int s = 0, int e = 0) : start(s), end(e) {}
    };
    
    // 动态分配栈空间（最坏情况下需 O(n) 空间）
    Range* stack = new Range[len];
    int top = 0;
    stack[top++] = Range(0, len - 1);
    
    while (top > 0) {
        Range range = stack[--top];
        if (range.start >= range.end) {
            continue;
        }
        
        T pivot = arr[range.end];
        int left = range.start;
        int right = range.end - 1;
        
        // 分区操作（与递归版本相同）
        while (left < right) {
            while (arr[left] < pivot && left < right) {
                left++;
            }
            while (arr[right] >= pivot && left < right) {
                right--;
            }
            std::swap(arr[left], arr[right]);
        }
        
        // 将基准放到正确位置
        if (arr[left] >= arr[range.end]) {
            std::swap(arr[left], arr[range.end]);
        } else {
            left++;
        }
        
        // 将左右子区间入栈（先左后右，处理顺序无关紧要）
        stack[top++] = Range(range.start, left - 1);
        stack[top++] = Range(left + 1, range.end);
    }
    
    delete[] stack;
}