// CircularLinkedList.hpp
#ifndef CIRCULAR_LINKED_LIST_HPP
#define CIRCULAR_LINKED_LIST_HPP

#include <iostream>
#include <stdexcept>
#include <initializer_list>

/**
 * @brief 循环链表模板
 * @tparam T 元素类型
 * 
 * 时间复杂度：
 * - 插入/删除（头/尾）：O(1)
 * - 插入/删除（任意位置）：O(n)
 * - 查找：O(n)
 * - 空间复杂度：O(n)
 */
template<typename T>
class CircularLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& val) : data(val), next(nullptr) {}
        Node(T&& val) : data(std::move(val)), next(nullptr) {}
    };
    
    Node* _head;          // 头节点（哨兵）
    std::size_t _size;    // 元素个数
    
    /**
     * @brief 获取指定位置的前驱节点
     * @param pos 位置（从1开始）
     * @return Node* 前驱节点指针
     * @throws std::out_of_range 如果位置无效
     */
    Node* getPreNode(std::size_t pos) const {
        if (pos < 1 || pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* p = _head;
        for (std::size_t i = 0; i < pos - 1; ++i) {
            p = p->next;
        }
        return p;
    }
    
    /**
     * @brief 递归拷贝链表
     * @param node 当前节点
     * @param head 头节点（用于判断循环结束）
     * @return Node* 拷贝后的节点
     */
    Node* copyList(Node* node, Node* head) {
        if (node == head) return head;
        
        Node* newNode = new Node(node->data);
        newNode->next = copyList(node->next, head);
        return newNode;
    }
    
    /**
     * @brief 销毁链表
     * @param head 头节点
     */
    void destroyList(Node* head) {
        if (!head) return;
        
        Node* p = head->next;
        while (p != head) {
            Node* q = p;
            p = p->next;
            delete q;
        }
        delete head;
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空循环链表
     */
    CircularLinkedList() : _size(0) {
        _head = new Node(T());  // 哨兵节点
        _head->next = _head;
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    CircularLinkedList(std::initializer_list<T> ilist) : CircularLinkedList() {
        for (const T& val : ilist) {
            pushBack(val);
        }
    }
    
    /**
     * @brief 填充构造函数
     * @param count 元素个数
     * @param val 元素值
     */
    CircularLinkedList(std::size_t count, const T& val) : CircularLinkedList() {
        for (std::size_t i = 0; i < count; ++i) {
            pushBack(val);
        }
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 释放所有节点内存
     */
    ~CircularLinkedList() {
        destroyList(_head);
    }
    
    // ---- 拷贝控制（五法则） ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    CircularLinkedList(const CircularLinkedList& other) : _size(other._size) {
        if (other._head) {
            _head = new Node(other._head->data);
            _head->next = copyList(other._head->next, other._head);
        }
    }
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    CircularLinkedList(CircularLinkedList&& other) noexcept 
        : _head(other._head), _size(other._size) {
        other._head = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return CircularLinkedList&
     * 时间复杂度：O(n)
     */
    CircularLinkedList& operator=(const CircularLinkedList& other) {
        if (this != &other) {
            CircularLinkedList tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return CircularLinkedList&
     * 时间复杂度：O(1)
     */
    CircularLinkedList& operator=(CircularLinkedList&& other) noexcept {
        if (this != &other) {
            this->~CircularLinkedList();
            _head = other._head;
            _size = other._size;
            other._head = nullptr;
            other._size = 0;
        }
        return *this;
    }
    
    /**
     * @brief 交换两个链表
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(CircularLinkedList& other) {
        std::swap(_head, other._head);
        std::swap(_size, other._size);
    }
    
    // ---- 容量 ----
    
    /**
     * @brief 返回元素个数
     * @return std::size_t
     * 时间复杂度：O(1)
     */
    std::size_t size() const { return _size; }
    
    /**
     * @brief 判断是否为空
     * @return bool
     * 时间复杂度：O(1)
     */
    bool empty() const { return _size == 0; }
    
    // ---- 访问 ----
    
    /**
     * @brief 获取指定位置的元素（常量版本）
     * @param pos 位置（从1开始）
     * @return const T& 元素引用
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    const T& at(std::size_t pos) const {
        if (pos < 1 || pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* p = _head->next;
        for (std::size_t i = 1; i < pos; ++i) {
            p = p->next;
        }
        return p->data;
    }
    
    /**
     * @brief 获取指定位置的元素（非常量版本）
     * @param pos 位置（从1开始）
     * @return T& 元素引用
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    T& at(std::size_t pos) {
        return const_cast<T&>(const_cast<const CircularLinkedList*>(this)->at(pos));
    }
    
    /**
     * @brief 获取第一个元素
     * @return const T& 元素引用
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(1)
     */
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return _head->next->data;
    }
    
    /**
     * @brief 获取第一个元素（非常量版本）
     * @return T& 元素引用
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(1)
     */
    T& front() {
        return const_cast<T&>(const_cast<const CircularLinkedList*>(this)->front());
    }
    
    /**
     * @brief 获取最后一个元素
     * @return const T& 元素引用
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(n)
     */
    const T& back() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        
        Node* p = _head;
        while (p->next != _head) {
            p = p->next;
        }
        return p->data;
    }
    
    /**
     * @brief 获取最后一个元素（非常量版本）
     * @return T& 元素引用
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(n)
     */
    T& back() {
        return const_cast<T&>(const_cast<const CircularLinkedList*>(this)->back());
    }
    
    // ---- 插入操作 ----
    
    /**
     * @brief 在头部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     */
    void pushFront(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = _head->next;
        _head->next = newNode;
        ++_size;
    }
    
    /**
     * @brief 在头部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void pushFront(T&& val) {
        Node* newNode = new Node(std::move(val));
        newNode->next = _head->next;
        _head->next = newNode;
        ++_size;
    }
    
    /**
     * @brief 在尾部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(n)
     */
    void pushBack(const T& val) {
        Node* p = _head;
        while (p->next != _head) {
            p = p->next;
        }
        
        Node* newNode = new Node(val);
        newNode->next = _head;
        p->next = newNode;
        ++_size;
    }
    
    /**
     * @brief 在尾部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(n)
     */
    void pushBack(T&& val) {
        Node* p = _head;
        while (p->next != _head) {
            p = p->next;
        }
        
        Node* newNode = new Node(std::move(val));
        newNode->next = _head;
        p->next = newNode;
        ++_size;
    }
    
    /**
     * @brief 在指定位置插入元素（拷贝版本）
     * @param pos 位置（从1开始）
     * @param val 要插入的值
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    void insert(std::size_t pos, const T& val) {
        if (pos < 1 || pos > _size + 1) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* p = _head;
        for (std::size_t i = 0; i < pos - 1; ++i) {
            p = p->next;
        }
        
        Node* newNode = new Node(val);
        newNode->next = p->next;
        p->next = newNode;
        ++_size;
    }
    
    /**
     * @brief 在指定位置插入元素（移动版本）
     * @param pos 位置（从1开始）
     * @param val 要插入的值（右值引用）
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    void insert(std::size_t pos, T&& val) {
        if (pos < 1 || pos > _size + 1) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* p = _head;
        for (std::size_t i = 0; i < pos - 1; ++i) {
            p = p->next;
        }
        
        Node* newNode = new Node(std::move(val));
        newNode->next = p->next;
        p->next = newNode;
        ++_size;
    }
    
    // ---- 删除操作 ----
    
    /**
     * @brief 删除头部元素
     * @return T 被删除的元素
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(1)
     */
    T popFront() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        
        Node* p = _head->next;
        T data = p->data;
        _head->next = p->next;
        delete p;
        --_size;
        return data;
    }
    
    /**
     * @brief 删除尾部元素
     * @return T 被删除的元素
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(n)
     */
    T popBack() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        
        Node* p = _head;
        while (p->next->next != _head) {
            p = p->next;
        }
        
        Node* q = p->next;
        T data = q->data;
        p->next = _head;
        delete q;
        --_size;
        return data;
    }
    
    /**
     * @brief 删除指定位置的元素
     * @param pos 位置（从1开始）
     * @return T 被删除的元素
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    T erase(std::size_t pos) {
        if (pos < 1 || pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* p = _head;
        for (std::size_t i = 0; i < pos - 1; ++i) {
            p = p->next;
        }
        
        Node* q = p->next;
        T data = q->data;
        p->next = q->next;
        delete q;
        --_size;
        return data;
    }
    
    // ---- 查找 ----
    
    /**
     * @brief 查找元素
     * @param val 要查找的值
     * @return std::size_t 元素位置（从1开始），未找到返回0
     * 时间复杂度：O(n)
     */
    std::size_t find(const T& val) const {
        std::size_t pos = 1;
        Node* p = _head->next;
        while (p != _head) {
            if (p->data == val) {
                return pos;
            }
            p = p->next;
            ++pos;
        }
        return 0;
    }
    
    /**
     * @brief 判断是否包含某元素
     * @param val 要查找的值
     * @return bool 是否包含
     * 时间复杂度：O(n)
     */
    bool contains(const T& val) const {
        return find(val) != 0;
    }
    
    // ---- 遍历 ----
    
    /**
     * @brief 遍历并打印所有元素
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void print(std::ostream& os = std::cout) const {
        Node* p = _head->next;
        while (p != _head) {
            os << p->data << "\t";
            p = p->next;
        }
        os << "\n";
    }
    
    /**
     * @brief 遍历并执行回调函数
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void forEach(std::function<void(const T&)> callback) const {
        Node* p = _head->next;
        while (p != _head) {
            callback(p->data);
            p = p->next;
        }
    }
    
    // ---- 操作 ----
    
    /**
     * @brief 清空所有元素
     * 时间复杂度：O(n)
     */
    void clear() {
        Node* p = _head->next;
        while (p != _head) {
            Node* q = p;
            p = p->next;
            delete q;
        }
        _head->next = _head;
        _size = 0;
    }
    
    /**
     * @brief 反转链表
     * 时间复杂度：O(n)
     */
    void reverse() {
        if (_size <= 1) return;
        
        Node* prev = _head;
        Node* curr = _head->next;
        Node* last = _head;
        
        // 找到最后一个节点
        while (last->next != _head) {
            last = last->next;
        }
        
        _head->next = last;
        
        while (curr != _head) {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        
        last->next = _head;
    }
    
    /**
     * @brief 合并另一个循环链表到当前链表
     * @param other 要合并的链表（会被清空）
     * 时间复杂度：O(n)
     */
    void merge(CircularLinkedList& other) {
        if (other.empty()) return;
        
        // 找到当前链表的尾节点
        Node* tail = _head;
        while (tail->next != _head) {
            tail = tail->next;
        }
        
        // 找到另一个链表的尾节点
        Node* otherTail = other._head;
        while (otherTail->next != other._head) {
            otherTail = otherTail->next;
        }
        
        // 合并
        tail->next = other._head->next;
        otherTail->next = _head;
        
        _size += other._size;
        
        // 清空 other
        other._head->next = other._head;
        other._size = 0;
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(CircularLinkedList<T>& a, CircularLinkedList<T>& b) {
    a.swap(b);
}

#endif // CIRCULAR_LINKED_LIST_HPP