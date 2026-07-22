template<typename T>
class MyList {
private:
    /**
     * @brief 链表节点结构体
     * 双向链表的节点，包含数据和前后指针
     */
    struct Node {
        T data;      // 存储的数据
        Node* prev;  // 指向前一个节点
        Node* next;  // 指向后一个节点
        
        /**
         * @brief 节点构造函数（拷贝版本）
         * @param val 要存储的值
         * @param p 前驱节点指针
         * @param n 后继节点指针
         * 时间复杂度：O(1)
         */
        Node(const T& val = T(), Node* p = nullptr, Node* n = nullptr) 
            : data(val), prev(p), next(n) {}
        
        /**
         * @brief 节点构造函数（移动版本）
         * @param val 要存储的值（右值引用）
         * @param p 前驱节点指针
         * @param n 后继节点指针
         * 时间复杂度：O(1)
         * 移动构造 data，避免拷贝
         */
        Node(T&& val, Node* p = nullptr, Node* n = nullptr) 
            : data(std::move(val)), prev(p), next(n) {}
    };
    
    Node* head;  // 哨兵节点（不存储数据），指向链表头部

public:
    // ---- 迭代器 ----
    
    /**
     * @brief 双向链表迭代器
     * 提供类似指针的接口，用于遍历链表
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
         * @brief 前置递减（--it）
         * @return Iterator& 移动到前一个节点
         * 时间复杂度：O(1)
         */
        Iterator& operator--() { ptr = ptr->prev; return *this; }
        
        /**
         * @brief 后置递减（it--）
         * @return Iterator 移动前的迭代器副本
         * 时间复杂度：O(1)
         */
        Iterator operator--(int) { Iterator tmp = *this; ptr = ptr->prev; return tmp; }
        
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
         * 用于内部实现，不对外暴露
         */
        Node* get_node() const { return ptr; }
    };

    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空链表，初始化哨兵节点
     * 时间复杂度：O(1)
     * 哨兵节点的 prev 和 next 都指向自己
     */
    MyList() : head(new Node()) {
        head->prev = head;
        head->next = head;
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 释放所有节点内存
     * 时间复杂度：O(n)
     */
    ~MyList() {
        clear();    // 释放所有数据节点
        delete head; // 释放哨兵节点
    }

    // ---- 迭代器访问 ----
    
    /**
     * @brief 返回指向第一个元素的迭代器
     * @return Iterator 指向第一个数据节点
     * 时间复杂度：O(1)
     */
    Iterator begin() { return Iterator(head->next); }
    
    /**
     * @brief 返回指向链表末尾的迭代器（哨兵节点）
     * @return Iterator 指向哨兵节点
     * 时间复杂度：O(1)
     */
    Iterator end() { return Iterator(head); }

    // ---- 容量 ----
    
    /**
     * @brief 判断链表是否为空
     * @return bool 链表是否为空
     * 时间复杂度：O(1)
     */
    bool empty() const { return head->next == head; }
    
    /**
     * @brief 返回链表长度
     * @return std::size_t 节点个数（不含哨兵）
     * 时间复杂度：O(n)
     * 注意：链表不支持 O(1) 的 size，必须遍历
     */
    std::size_t size() const {
        std::size_t count = 0;
        Node* cur = head->next;
        while (cur != head) { ++count; cur = cur->next; }
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
    
    /**
     * @brief 访问最后一个元素（非常量版本）
     * @return T& 最后一个元素的引用
     * 时间复杂度：O(1)
     * 前提：链表不为空
     */
    T& back() { return head->prev->data; }
    
    /**
     * @brief 访问最后一个元素（常量版本）
     * @return const T& 最后一个元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& back() const { return head->prev->data; }

    // ---- 插入（O(1)） ----
    
    /**
     * @brief 在头部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     */
    void push_front(const T& val) { insert(begin(), val); }
    
    /**
     * @brief 在头部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void push_front(T&& val) { insert(begin(), std::move(val)); }
    
    /**
     * @brief 在尾部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     */
    void push_back(const T& val) { insert(end(), val); }
    
    /**
     * @brief 在尾部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void push_back(T&& val) { insert(end(), std::move(val)); }

    /**
     * @brief 在指定位置前插入元素（拷贝版本）
     * @param pos 插入位置（迭代器指向的节点之前）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     * 
     * 实现细节：
     * 1. 创建新节点，prev 指向 pos 的前一个，next 指向 pos
     * 2. 更新前后节点的指针
     */
    void insert(Iterator pos, const T& val) {
        Node* p = pos.get_node();          // 获取 pos 指向的节点
        Node* new_node = new Node(val, p->prev, p);  // 创建新节点
        p->prev->next = new_node;          // 前驱节点指向新节点
        p->prev = new_node;                // pos 节点的 prev 指向新节点
    }

    /**
     * @brief 在指定位置前插入元素（移动版本）
     * @param pos 插入位置（迭代器指向的节点之前）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void insert(Iterator pos, T&& val) {
        Node* p = pos.get_node();
        Node* new_node = new Node(std::move(val), p->prev, p);
        p->prev->next = new_node;
        p->prev = new_node;
    }

    // ---- 删除（O(1)） ----
    
    /**
     * @brief 删除头部元素
     * 时间复杂度：O(1)
     * 前提：链表不为空
     */
    void pop_front() { erase(begin()); }
    
    /**
     * @brief 删除尾部元素
     * 时间复杂度：O(1)
     * 前提：链表不为空
     */
    void pop_back() { erase(--end()); }

    /**
     * @brief 删除指定位置的元素
     * @param pos 要删除的位置
     * 时间复杂度：O(1)
     * 
     * 实现细节：
     * 1. 获取要删除的节点
     * 2. 更新前后节点的指针，绕过该节点
     * 3. 释放该节点的内存
     */
    void erase(Iterator pos) {
        Node* p = pos.get_node();
        p->prev->next = p->next;   // 前驱节点指向后继节点
        p->next->prev = p->prev;   // 后继节点指向前驱节点
        delete p;                  // 释放节点内存
    }

    // ---- 批量操作 ----
    
    /**
     * @brief 清空链表
     * 时间复杂度：O(n)
     * 释放所有数据节点，保留哨兵节点
     */
    void clear() {
        Node* cur = head->next;
        while (cur != head) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        head->prev = head;
        head->next = head;
    }

    // ---- 查找 ----
    
    /**
     * @brief 查找指定值的节点
     * @param val 要查找的值
     * @return Iterator 指向找到的节点，若不存在则返回 end()
     * 时间复杂度：O(n)
     * 线性查找，链表不支持快速查找
     */
    Iterator find(const T& val) {
        Node* cur = head->next;
        while (cur != head) {
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
     */
    MyList(const MyList& other) : head(new Node()) {
        head->prev = head;
        head->next = head;
        Node* cur = other.head->next;
        while (cur != other.head) {
            push_back(cur->data);
            cur = cur->next;
        }
    }

    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     * 直接窃取哨兵节点，other 变为空
     */
    MyList(MyList&& other) noexcept : head(other.head) {
        other.head = nullptr;  // 防止 other 析构时释放我们的节点
    }

    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return MyList& 当前对象的引用
     * 时间复杂度：O(n)
     * 使用 copy-and-swap 惯用法，提供强异常安全保证
     */
    MyList& operator=(const MyList& other) {
        if (this != &other) {
            MyList tmp(other);  // 拷贝构造临时对象
            swap(tmp);          // 交换，临时对象销毁时释放旧资源
        }
        return *this;
    }

    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return MyList& 当前对象的引用
     * 时间复杂度：O(n)
     * 先释放自己的资源，再窃取 other 的资源
     */
    MyList& operator=(MyList&& other) noexcept {
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
     * 只交换哨兵节点的指针
     */
    void swap(MyList& other) {
        std::swap(head, other.head);
    }

    // ---- 高级操作 ----
    
    /**
     * @brief 合并两个有序链表
     * @param other 另一个有序链表
     * 时间复杂度：O(n + m)
     * 将 other 中的节点按顺序合并到当前链表，other 变为空
     * 
     * 实现细节：
     * 1. 同时遍历两个链表
     * 2. 如果 other 的节点值小于当前节点，则把 other 节点插入到当前节点前
     * 3. 如果 other 有剩余，全部追加到尾部
     * 4. 最后将 other 的哨兵节点重置为空状态
     */
    void merge(MyList& other) {
        if (this == &other) return;  // 不能合并自己
        
        Iterator it1 = begin();
        Iterator it2 = other.begin();
        
        // 同时遍历两个链表
        while (it1 != end() && it2 != other.end()) {
            if (*it2 < *it1) {
                // 从 other 中取出当前节点
                Node* p = it2.get_node();
                ++it2;
                // 从 other 链表中解除 p 的链接
                p->prev->next = p->next;
                p->next->prev = p->prev;
                
                // 将 p 插入到 it1 前面
                Node* pos = it1.get_node();
                p->prev = pos->prev;
                p->next = pos;
                pos->prev->next = p;
                pos->prev = p;
            } else {
                ++it1;
            }
        }
        
        // 如果 other 还有剩余，全部追加到尾部
        while (it2 != other.end()) {
            Node* p = it2.get_node();
            ++it2;
            // 从 other 链表中解除 p 的链接
            p->prev->next = p->next;
            p->next->prev = p->prev;
            
            // 将 p 追加到尾部（head 之前）
            p->prev = head->prev;
            p->next = head;
            head->prev->next = p;
            head->prev = p;
        }
        
        // 重置 other 的哨兵节点
        other.head->next = other.head;
        other.head->prev = other.head;
    }

    /**
     * @brief 反转链表
     * 时间复杂度：O(n)
     * 遍历所有节点，交换每个节点的 prev 和 next 指针
     * 最后交换哨兵节点的 prev 和 next
     */
    void reverse() {
        Node* cur = head->next;
        while (cur != head) {
            Node* next = cur->next;
            std::swap(cur->prev, cur->next);  // 交换每个节点的前后指针
            cur = next;
        }
        std::swap(head->prev, head->next);  // 交换哨兵的前后指针
    }

    /**
     * @brief 排序（归并排序）
     * 时间复杂度：O(n log n)
     * 
     * 实现细节：
     * 1. 如果链表长度 <= 1，直接返回
     * 2. 将链表对半分成左右两个子链表
     * 3. 递归排序左右子链表
     * 4. 合并两个有序子链表
     * 
     * 注意：这是一个简单实现，会进行大量数据拷贝
     * 高效的链表排序应使用迭代器操作节点，但这里为了清晰性使用了 push_back
     */
    void sort() {
        if (size() <= 1) return;
        
        MyList left, right;
        Node* cur = head->next;
        std::size_t mid = size() / 2;
        
        // 将链表分成两半
        for (std::size_t i = 0; i < mid; ++i) {
            left.push_back(cur->data);
            cur = cur->next;
        }
        while (cur != head) {
            right.push_back(cur->data);
            cur = cur->next;
        }
        
        // 递归排序
        left.sort();
        right.sort();
        
        // 清空当前链表，合并左右两个有序链表
        clear();
        left.merge(right);
        
        // 将 left 中的数据拷贝回当前链表
        Iterator it = left.begin();
        while (it != left.end()) {
            push_back(*it);
            ++it;
        }
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * @param a 第一个链表
 * @param b 第二个链表
 * 时间复杂度：O(1)
 * 让 std::swap 和泛型代码能高效交换 MyList
 */
template<typename T>
void swap(MyList<T>& a, MyList<T>& b) {
    a.swap(b);
}

