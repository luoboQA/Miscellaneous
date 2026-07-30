/**
 * @brief 队列（Queue）容器适配器
 * 
 * 队列是一种先进先出（FIFO，First In First Out）的数据结构
 * 它是对底层容器的封装，只暴露队列特有的接口
 * 
 * 核心特性：
 * - 只能在尾部插入（push）
 * - 只能从头部删除（pop）
 * - 只能访问头部和尾部元素（front/back）
 * - 不支持遍历（没有迭代器）
 * 
 * 时间复杂度：
 * - push：O(1)（取决于底层容器）
 * - pop：O(1)（取决于底层容器）
 * - front/back：O(1)
 * - size/empty：O(1)（取决于底层容器）
 * 
 * 默认底层容器：MyDeque<T>
 * 因为 deque 支持 O(1) 的头尾操作，且内存效率高
 * 
 * @tparam T 元素类型
 * @tparam Container 底层容器类型（必须支持 push_back、pop_front、front、back、size、empty）
 */
template<typename T, typename Container = MyDeque<T>>
class MyQueue {
private:
    Container c;  // 底层容器对象，所有操作都转发给它

public:
    // ---- 容量 ----
    
    /**
     * @brief 判断队列是否为空
     * @return bool 队列是否为空
     * 时间复杂度：O(1)（转发给底层容器）
     */
    bool empty() const { return c.empty(); }
    
    /**
     * @brief 返回队列中的元素个数
     * @return std::size_t 元素个数
     * 时间复杂度：O(1)（转发给底层容器）
     */
    std::size_t size() const { return c.size(); }

    // ---- 元素访问 ----
    
    /**
     * @brief 访问队列头部元素（非常量版本）
     * @return T& 头部元素的引用
     * 时间复杂度：O(1)（转发给底层容器）
     * 前提：队列不为空
     * 
     * 头部元素是第一个被 push 进来的元素，也是下一个要被 pop 的元素
     */
    T& front() { return c.front(); }
    
    /**
     * @brief 访问队列头部元素（常量版本）
     * @return const T& 头部元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& front() const { return c.front(); }
    
    /**
     * @brief 访问队列尾部元素（非常量版本）
     * @return T& 尾部元素的引用
     * 时间复杂度：O(1)（转发给底层容器）
     * 前提：队列不为空
     * 
     * 尾部元素是最后一个被 push 进来的元素
     */
    T& back() { return c.back(); }
    
    /**
     * @brief 访问队列尾部元素（常量版本）
     * @return const T& 尾部元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& back() const { return c.back(); }

    // ---- 插入和删除（O(1)） ----
    
    /**
     * @brief 在队列尾部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)（转发给底层容器的 push_back）
     * 
     * 队列的 push 操作等价于在底层容器的尾部插入
     */
    void push(const T& val) { c.push_back(val); }
    
    /**
     * @brief 在队列尾部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     * 相比拷贝版本，减少一次拷贝
     */
    void push(T&& val) { c.push_back(std::move(val)); }
    
    /**
     * @brief 删除队列头部元素
     * 时间复杂度：O(1)（转发给底层容器的 pop_front）
     * 前提：队列不为空
     * 
     * 队列的 pop 操作等价于从底层容器的头部删除
     */
    void pop() { c.pop_front(); }

    // ---- 交换 ----
    
    /**
     * @brief 交换两个队列的内容
     * @param other 要交换的队列
     * 时间复杂度：O(1)（转发给底层容器的 swap）
     * 
     * 只交换底层容器，不复制任何元素
     */
    void swap(MyQueue& other) { c.swap(other.c); }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * @param a 第一个队列
 * @param b 第二个队列
 * 时间复杂度：O(1)
 */
template<typename T, typename Container>
void swap(MyQueue<T, Container>& a, MyQueue<T, Container>& b) {
    a.swap(b);
}

