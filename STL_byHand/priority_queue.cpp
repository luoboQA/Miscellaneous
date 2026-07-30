/**
 * @brief 优先队列（Priority Queue）容器适配器
 * 
 * 优先队列是一种特殊的队列，它的元素按照优先级顺序出队
 * 默认实现为最大堆（Max Heap），即最大值优先出队，每个父节点 ≥ 其子节点
 * 
 * 核心特性：
 * - 只能访问顶部元素（最大值/最小值）
 * - 插入元素后自动调整堆结构
 * - 删除顶部元素后自动调整堆结构
 * - 不支持遍历（没有迭代器）
 * 
 * 时间复杂度：
 * - push：O(log n)
 * - pop：O(log n)
 * - top：O(1)
 * - size/empty：O(1)
 * 
 * 默认底层容器：MyVector<T>
 * 因为 vector 支持 O(1) 的随机访问，适合堆操作
 * 
 * @tparam T 元素类型
 * @tparam Container 底层容器类型（必须支持 push_back、pop_back、front、size、empty、operator[]）
 */
template<typename T, typename Container = MyVector<T>>
class MyPriorityQueue {
private:
    Container c;  // 底层容器，存储堆元素

    // ---- 堆调整算法 ----
    
    /**
     * @brief 上浮操作（向上调整堆）
     * @param idx 从哪个索引开始上浮
     * 时间复杂度：O(log n)
     * 
     * 使用场景：在尾部插入新元素后调用
     * 工作原理：比较当前节点与其父节点，如果当前节点更大则交换，继续向上
     * 
     * 堆性质：父节点索引 = (idx - 1) / 2
     *        子节点索引 = 2*idx+1（左），2*idx+2（右）
     */
    void heapify_up(std::size_t idx) {
        while (idx > 0) {
            std::size_t parent = (idx - 1) / 2;      // 计算父节点索引
            if (c[parent] < c[idx]) {                // 子节点大于父节点（最大堆）
                std::swap(c[parent], c[idx]);        // 交换
                idx = parent;                        // 继续向上
            } else {
                break;                               // 满足堆性质，停止
            }
        }
    }

    /**
     * @brief 下沉操作（向下调整堆）
     * @param idx 从哪个索引开始下沉
     * 时间复杂度：O(log n)
     * 
     * 使用场景：删除顶部元素后调用
     * 工作原理：比较当前节点与其左右子节点，如果子节点更大则与最大的子节点交换，继续向下
     */
    void heapify_down(std::size_t idx) {
        std::size_t size = c.size();
        while (true) {
            std::size_t largest = idx;               // 假设当前节点最大
            std::size_t left = 2 * idx + 1;          // 左子节点索引
            std::size_t right = 2 * idx + 2;         // 右子节点索引
            
            // 与左子节点比较
            if (left < size && c[largest] < c[left]) {
                largest = left;
            }
            // 与右子节点比较
            if (right < size && c[largest] < c[right]) {
                largest = right;
            }
            
            if (largest != idx) {                    // 需要交换
                std::swap(c[idx], c[largest]);       // 交换
                idx = largest;                       // 继续向下
            } else {
                break;                               // 满足堆性质，停止
            }
        }
    }

public:
    // ---- 容量 ----
    
    /**
     * @brief 判断优先队列是否为空
     * @return bool 是否为空
     * 时间复杂度：O(1)（转发给底层容器）
     */
    bool empty() const { return c.empty(); }
    
    /**
     * @brief 返回优先队列中的元素个数
     * @return std::size_t 元素个数
     * 时间复杂度：O(1)（转发给底层容器）
     */
    std::size_t size() const { return c.size(); }

    // ---- 元素访问 ----
    
    /**
     * @brief 访问顶部元素（最大值）
     * @return const T& 顶部元素的常量引用
     * 时间复杂度：O(1)
     * 前提：优先队列不为空
     * 
     * 在最大堆中，顶部元素是最大的元素
     */
    const T& top() const { return c.front(); }

    // ---- 插入（O(log n)） ----
    
    /**
     * @brief 插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(log n)
     * 
     * 实现细节：
     * 1. 在尾部插入新元素
     * 2. 上浮调整，保持堆性质
     */
    void push(const T& val) {
        c.push_back(val);                         // 尾部插入
        heapify_up(c.size() - 1);                 // 上浮调整
    }
    
    /**
     * @brief 插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(log n)
     * 相比拷贝版本，减少一次拷贝
     */
    void push(T&& val) {
        c.push_back(std::move(val));              // 尾部插入（移动）
        heapify_up(c.size() - 1);                 // 上浮调整
    }

    // ---- 删除（O(log n)） ----
    
    /**
     * @brief 删除顶部元素（最大值）
     * 时间复杂度：O(log n)
     * 前提：优先队列不为空
     * 
     * 实现细节：
     * 1. 将顶部元素与尾部元素交换
     * 2. 删除尾部元素（原顶部元素）
     * 3. 从顶部开始下沉调整，保持堆性质
     */
    void pop() {
        if (c.empty()) return;
        std::swap(c.front(), c.back());           // 交换顶部和尾部
        c.pop_back();                              // 删除尾部（原顶部）
        if (!c.empty()) heapify_down(0);           // 下沉调整
    }

    // ---- 交换 ----
    
    /**
     * @brief 交换两个优先队列的内容
     * @param other 要交换的优先队列
     * 时间复杂度：O(1)（转发给底层容器的 swap）
     */
    void swap(MyPriorityQueue& other) { c.swap(other.c); }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * @param a 第一个优先队列
 * @param b 第二个优先队列
 * 时间复杂度：O(1)
 */
template<typename T, typename Container>
void swap(MyPriorityQueue<T, Container>& a, MyPriorityQueue<T, Container>& b) {
    a.swap(b);
}

