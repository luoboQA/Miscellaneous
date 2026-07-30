/*
 *（计数数组，累计数组）。通过统计每个值出现的次数，然后根据累计计数将元素放到正确位置。
 *
 * 计数排序思路：
 * 1. 找出待排序数组中的最大值和最小值
 * 2. 统计数组中每个值出现的次数，存入计数数组
 * 3. 对计数数组进行累加，使得每个位置存储的是该值在排序后数组中的最终位置
 * 4. 反向遍历原数组，将每个元素放到正确位置，保持稳定性
 *
 * 排序算法	平均时间复杂度	最差时间复杂度	空间复杂度	数据对象稳定性
 * 计数排序	  O(n+m)	         O(n+m)	        O(n+m)	    稳定
 * 数组：[4, 2, 2, 8, 3, 3, 1]
 * 
 * 第1步：找出 min=1, max=8, range=8
 * 
 * 第2步：统计次数
 *   vecCount: [1, 2, 2, 1, 0, 0, 0, 1]
 *   分别对应值: 1, 2, 3, 4, 5, 6, 7, 8
 * 
 * 第3步：累计次数
 *   vecCount: [1, 3, 5, 6, 6, 6, 6, 7]
 *   含义：值1排在第1位，值2排在第2-3位，值3排在第4-5位...
 * 
 * 第4步：反向填充（保持稳定性）
 *   原数组从后往前：
 *   取1 → 放在第1位 → [1, 0, 0, 0, 0, 0, 0]
 *   取3 → 放在第5位 → [1, 0, 0, 0, 3, 0, 0]
 *   取3 → 放在第4位 → [1, 0, 0, 3, 3, 0, 0]
 *   取8 → 放在第7位 → [1, 0, 0, 3, 3, 0, 8]
 *   取2 → 放在第3位 → [1, 0, 2, 3, 3, 0, 8]
 *   取2 → 放在第2位 → [1, 2, 2, 3, 3, 0, 8]
 *   取4 → 放在第6位 → [1, 2, 2, 3, 3, 4, 8]
 * 
 * 结果：[1, 2, 2, 3, 3, 4, 8]
 */

/**
 * @brief 计数排序
 * 
 * @param vecRaw 待排序的整数向量（引用传递，只读）
 * @param vecObj 排序后的结果向量（引用传递，需要预先分配相同大小的空间）
 * 
 * @note 时间复杂度 O(n + m)，空间复杂度 O(n + m)
 * @note 稳定排序算法，相同元素的相对顺序不变
 * @note 要求所有元素为整数，且数值范围不宜过大
 * @note 仅适用于整数排序，不适合浮点数或字符串
 */
void CountSort(std::vector<int>& vecRaw, std::vector<int>& vecObj) {
    // 1. 确保待排序容器非空
    if (vecRaw.size() == 0)
        return;

    // 2. 找出数组中的最大值和最小值
    int minVal = *std::min_element(vecRaw.begin(), vecRaw.end());
    int maxVal = *std::max_element(vecRaw.begin(), vecRaw.end());
    int range = maxVal - minVal + 1;  // 数值范围

    // 3. 创建计数数组，统计每个值出现的次数
    std::vector<int> vecCount(range, 0);
    
    // 统计每个键值出现的次数（偏移 minVal 存储）
    for (int i = 0; i < vecRaw.size(); i++) {
        vecCount[vecRaw[i] - minVal]++;
    }
    
    // 4. 累加计数：后面的键值位置为前面所有键值出现次数之和
    for (int i = 1; i < range; i++) {
        vecCount[i] += vecCount[i - 1];
    }

    // 5. 反向填充目标数组（保持稳定性）
    //    反向遍历确保相同元素的相对顺序不变
    for (int i = vecRaw.size(); i > 0; i--) {
        int value = vecRaw[i - 1];
        int index = vecCount[value - minVal] - 1;
        vecObj[index] = value;
        vecCount[value - minVal]--;
    }
}