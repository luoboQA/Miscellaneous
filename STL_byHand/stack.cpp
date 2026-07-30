/**
 * @brief 栈（Stack）容器适配器
 * 
 * 栈是一种后进先出（LIFO，Last In First Out）的数据结构
 * 它是对底层容器的封装，只暴露栈特有的接口
 * 
 * 核心特性：
 * - 只能在顶部插入（push）
 * - 只能从顶部删除（pop）
 * - 只能访问顶部元素（top）
 * - 不支持遍历（没有迭代器）
 * - 不支持随机访问
 * 
 * 时间复杂度：
 * - push：O(1)（取决于底层容器）
 * - pop：O(1)（取决于底层容器）
 * - top：O(1)
 * - size/empty：O(1)（取决于底层容器）
 * 
 * 默认底层容器：MyDeque<T>
 * 因为 deque 支持 O(1) 的尾部操作，且内存效率高
 * 也可以使用 MyVector<T> 或 MyList<T>
 * 
 * @tparam T 元素类型
 * @tparam Container 底层容器类型（必须支持 push_back、pop_back、back、size、empty）
 */
template<typename T, typename Container = MyDeque<T>>
class MyStack {
private:
    Container c;  // 底层容器对象，所有操作都转发给它

public:
    // ---- 容量 ----
    
    /**
     * @brief 判断栈是否为空
     * @return bool 栈是否为空
     * 时间复杂度：O(1)（转发给底层容器）
     */
    bool empty() const { return c.empty(); }
    
    /**
     * @brief 返回栈中的元素个数
     * @return std::size_t 元素个数
     * 时间复杂度：O(1)（转发给底层容器）
     */
    std::size_t size() const { return c.size(); }

    // ---- 元素访问 ----
    
    /**
     * @brief 访问栈顶元素（非常量版本）
     * @return T& 栈顶元素的引用
     * 时间复杂度：O(1)（转发给底层容器的 back）
     * 前提：栈不为空
     * 
     * 栈顶元素是最后一个被 push 进来的元素，也是下一个要被 pop 的元素
     */
    T& top() { return c.back(); }
    
    /**
     * @brief 访问栈顶元素（常量版本）
     * @return const T& 栈顶元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& top() const { return c.back(); }

    // ---- 插入和删除（O(1)） ----
    
    /**
     * @brief 在栈顶插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)（转发给底层容器的 push_back）
     * 
     * 栈的 push 操作等价于在底层容器的尾部插入
     */
    void push(const T& val) { c.push_back(val); }
    
    /**
     * @brief 在栈顶插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     * 相比拷贝版本，减少一次拷贝
     */
    void push(T&& val) { c.push_back(std::move(val)); }
    
    /**
     * @brief 删除栈顶元素
     * 时间复杂度：O(1)（转发给底层容器的 pop_back）
     * 前提：栈不为空
     * 
     * 栈的 pop 操作等价于从底层容器的尾部删除
     */
    void pop() { c.pop_back(); }

    // ---- 交换 ----
    
    /**
     * @brief 交换两个栈的内容
     * @param other 要交换的栈
     * 时间复杂度：O(1)（转发给底层容器的 swap）
     * 
     * 只交换底层容器，不复制任何元素
     */
    void swap(MyStack& other) { c.swap(other.c); }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * @param a 第一个栈
 * @param b 第二个栈
 * 时间复杂度：O(1)
 */
template<typename T, typename Container>
void swap(MyStack<T, Container>& a, MyStack<T, Container>& b) {
    a.swap(b);
}