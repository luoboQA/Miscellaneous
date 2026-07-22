/**
 * @brief 单向链表（Forward List）实现
 * 单链表结构，每个节点只指向下一个节点
 * 支持 O(1) 的头部插入/删除，O(1) 的指定位置后插入/删除
 * 
 * 核心设计：
 * - 哨兵节点（head）：简化边界条件处理
 * - 只支持正向迭代（单向）
 * - 插入/删除操作都在指定位置之后（after）
 * 
 * 与 std::forward_list 对应
 * 时间复杂度：
 * - 头部插入/删除：O(1)
 * - 指定位置后插入/删除：O(1)
 * - 查找：O(n)
 */
template<typename T>
class MyForwardList {
private:
    // ---- 节点结构 ----
    struct Node {
        T data;      // 存储的数据
        Node* next;  // 指向下一个节点
        
        /**
         * @brief 节点构造函数（拷贝版本）
         * @param val 要存储的值
         * @param n 下一个节点指针
         * 时间复杂度：O(1)
         */
        Node(const T& val = T(), Node* n = nullptr) : data(val), next(n) {}
        
        /**
         * @brief 节点构造函数（移动版本）
         * @param val 要存储的值（右值引用）
         * @param n 下一个节点指针
         * 时间复杂度：O(1)
         */
        Node(T&& val, Node* n = nullptr) : data(std::move(val)), next(n) {}
    };
    
    Node* head;  // 哨兵节点（不存储数据），指向链表头部

public:
    // ---- 迭代器 ----
    
    /**
     * @brief 单向链表迭代器
     * 只支持正向遍历（++），不支持反向遍历（--）
     * 是前向迭代器（Forward Iterator）
     */
    class Iterator {
    private:
        Node* ptr;  // 指向当前节点
        
    public:
        /**
         * @brief 迭代器构造函数
         * @param p 指向的节点指针
         * 时间复杂度：O(1)
         */
        Iterator(Node* p = nullptr) : ptr(p) {}
        
        /**
         * @brief 解引用运算符
         * @return T& 当前节点的数据引用
         * 时间复杂度：O(1)
         */
        T& operator*() { return ptr->data; }
        
        /**
         * @brief 前置递增（++it）
         * @return Iterator& 移动到下一个节点
         * 时间复杂度：O(1)
         */
        Iterator& operator++() { ptr = ptr->next; return *this; }
        
        /**
         * @brief 后置递增（it++）
         * @return Iterator 移动前的迭代器副本
         * 时间复杂度：O(1)
         */
        Iterator operator++(int) { Iterator tmp = *this; ptr = ptr->next; return tmp; }
        
        /**
         * @brief 相等比较
         * @param other 另一个迭代器
         * @return bool 是否指向同一节点
         * 时间复杂度：O(1)
         */
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        
        /**
         * @brief 不等比较
         * @param other 另一个迭代器
         * @return bool 是否指向不同节点
         * 时间复杂度：O(1)
         */
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        
        /**
         * @brief 获取底层节点指针
         * @return Node* 当前迭代器指向的节点
         * 时间复杂度：O(1)
         */
        Node* get_node() const { return ptr; }
    };

    // ---- 迭代器访问 ----
    
    /**
     * @brief 返回指向第一个元素的迭代器
     * @return Iterator 指向第一个数据节点
     * 时间复杂度：O(1)
     */
    Iterator begin() { return Iterator(head->next); }
    
    /**
     * @brief 返回指向链表末尾的迭代器（nullptr）
     * @return Iterator 指向 nullptr
     * 时间复杂度：O(1)
     */
    Iterator end() { return Iterator(nullptr); }

    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空链表，初始化哨兵节点
     * 时间复杂度：O(1)
     */
    MyForwardList() : head(new Node()) {}
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 释放所有节点内存
     * 时间复杂度：O(n)
     */
    ~MyForwardList() {
        clear();    // 释放所有数据节点
        delete head; // 释放哨兵节点
    }

    // ---- 容量 ----
    
    /**
     * @brief 判断链表是否为空
     * @return bool 链表是否为空
     * 时间复杂度：O(1)
     */
    bool empty() const { return head->next == nullptr; }
    
    /**
     * @brief 返回链表长度
     * @return std::size_t 节点个数（不含哨兵）
     * 时间复杂度：O(n)
     * 单链表不支持 O(1) 的 size，必须遍历
     */
    std::size_t size() const {
        std::size_t count = 0;
        Node* cur = head->next;
        while (cur) { ++count; cur = cur->next; }
        return count;
    }

    // ---- 访问 ----
    
    /**
     * @brief 访问第一个元素（非常量版本）
     * @return T& 第一个元素的引用
     * 时间复杂度：O(1)
     * 前提：链表不为空
     */
    T& front() { return head->next->data; }
    
    /**
     * @brief 访问第一个元素（常量版本）
     * @return const T& 第一个元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& front() const { return head->next->data; }

    // ---- 头部插入（O(1)） ----
    
    /**
     * @brief 在头部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     * 
     * 实现细节：
     * 1. 创建新节点，next 指向原来的第一个节点
     * 2. head->next 指向新节点
     */
    void push_front(const T& val) {
        head->next = new Node(val, head->next);
    }
    
    /**
     * @brief 在头部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void push_front(T&& val) {
        head->next = new Node(std::move(val), head->next);
    }

    // ---- 指定位置后插入（O(1)） ----
    
    /**
     * @brief 在指定位置之后插入元素
     * @param pos 位置迭代器（指向一个已存在的节点）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     * 
     * 实现细节：
     * 1. 获取 pos 指向的节点 p
     * 2. 创建新节点，next 指向 p 原来的下一个节点
     * 3. p->next 指向新节点
     */
    void insert_after(Iterator pos, const T& val) {
        Node* p = pos.get_node();
        p->next = new Node(val, p->next);
    }

    // ---- 删除（O(1)） ----
    
    /**
     * @brief 删除头部元素
     * 时间复杂度：O(1)
     * 前提：链表不为空
     * 
     * 实现细节：
     * 1. 保存 head->next（要删除的节点）
     * 2. head->next 指向第二个节点
     * 3. 删除原第一个节点
     */
    void pop_front() {
        if (head->next) {
            Node* tmp = head->next;
            head->next = tmp->next;
            delete tmp;
        }
    }

    /**
     * @brief 删除指定位置之后的元素
     * @param pos 位置迭代器（指向一个已存在的节点）
     * 时间复杂度：O(1)
     * 
     * 实现细节：
     * 1. 获取 pos 指向的节点 p
     * 2. 保存 p->next（要删除的节点）
     * 3. p->next 指向要删除节点的下一个节点
     * 4. 删除目标节点
     */
    void erase_after(Iterator pos) {
        Node* p = pos.get_node();
        if (p->next) {
            Node* tmp = p->next;
            p->next = tmp->next;
            delete tmp;
        }
    }

    // ---- 清空 ----
    
    /**
     * @brief 清空链表
     * 时间复杂度：O(n)
     * 释放所有数据节点，保留哨兵节点
     */
    void clear() {
        Node* cur = head->next;
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        head->next = nullptr;
    }

    // ---- 查找 ----
    
    /**
     * @brief 查找指定值的节点
     * @param val 要查找的值
     * @return Iterator 指向找到的节点，若不存在则返回 end()
     * 时间复杂度：O(n)
     * 线性查找，单链表不支持快速查找
     */
    Iterator find(const T& val) {
        Node* cur = head->next;
        while (cur) {
            if (cur->data == val) return Iterator(cur);
            cur = cur->next;
        }
        return end();
    }

    // ---- 拷贝控制（五法则） ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     * 深拷贝所有节点
     * 
     * 实现细节：
     * 1. 创建哨兵节点
     * 2. 遍历 other，逐个拷贝节点到尾部
     */
    MyForwardList(const MyForwardList& other) : head(new Node()) {
        Node* cur = other.head->next;
        Node* tail = head;
        while (cur) {
            tail->next = new Node(cur->data);
            tail = tail->next;
            cur = cur->next;
        }
    }

    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     * 直接窃取哨兵节点，other 变为空
     */
    MyForwardList(MyForwardList&& other) noexcept : head(other.head) {
        other.head = nullptr;  // 防止 other 析构时释放我们的节点
    }

    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return MyForwardList& 当前对象的引用
     * 时间复杂度：O(n)
     */
    MyForwardList& operator=(const MyForwardList& other) {
        if (this != &other) {
            MyForwardList tmp(other);  // 拷贝构造临时对象
            swap(tmp);                 // 交换，临时对象销毁时释放旧资源
        }
        return *this;
    }

    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return MyForwardList& 当前对象的引用
     * 时间复杂度：O(n)
     * 先释放自己的资源，再窃取 other 的资源
     */
    MyForwardList& operator=(MyForwardList&& other) noexcept {
        if (this != &other) {
            clear();         // 释放自己的所有数据节点
            delete head;     // 释放自己的哨兵节点
            head = other.head;  // 窃取哨兵节点
            other.head = nullptr;  // 让 other 进入空状态
        }
        return *this;
    }

    /**
     * @brief 交换两个链表
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     * 只交换哨兵节点指针
     */
    void swap(MyForwardList& other) {
        std::swap(head, other.head);
    }

    // ---- 反转 ----
    
    /**
     * @brief 反转链表
     * 时间复杂度：O(n)
     * 
     * 实现细节：
     * 1. 遍历链表，逐个改变 next 指针方向
     * 2. 最后 head->next 指向新的第一个节点
     */
    void reverse() {
        Node* prev = nullptr;
        Node* cur = head->next;
        while (cur) {
            Node* next = cur->next;  // 保存下一个节点
            cur->next = prev;        // 反转指针
            prev = cur;              // 前移
            cur = next;              // 后移
        }
        head->next = prev;  // 哨兵指向新的第一个节点
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * @param a 第一个链表
 * @param b 第二个链表
 * 时间复杂度：O(1)
 */
template<typename T>
void swap(MyForwardList<T>& a, MyForwardList<T>& b) {
    a.swap(b);
}