// DoublyLinkedList.hpp
#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <functional>

/**
 * @brief 双向链表模板
 * @tparam T 元素类型
 * 
 * 时间复杂度：
 * - 插入/删除（头/尾）：O(1)
 * - 插入/删除（任意位置）：O(n)
 * - 查找：O(n)
 * - 空间复杂度：O(n)
 */
template<typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        
        Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
        Node(T&& val) : data(std::move(val)), prev(nullptr), next(nullptr) {}
    };
    
    Node* _head;          // 头节点（哨兵）
    Node* _tail;          // 尾节点（哨兵）
    std::size_t _size;    // 元素个数
    
    /**
     * @brief 获取指定位置的节点
     * @param pos 位置（从1开始）
     * @return Node* 节点指针
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    Node* getNode(std::size_t pos) const {
        if (pos < 1 || pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* p = _head->next;
        for (std::size_t i = 1; i < pos; ++i) {
            p = p->next;
        }
        return p;
    }
    
    /**
     * @brief 递归拷贝链表
     * @param node 当前节点
     * @return Node* 拷贝后的节点
     */
    Node* copyList(Node* node) {
        if (!node) return nullptr;
        
        Node* newNode = new Node(node->data);
        newNode->next = copyList(node->next);
        if (newNode->next) {
            newNode->next->prev = newNode;
        }
        return newNode;
    }
    
    /**
     * @brief 销毁链表
     * @param head 头节点
     */
    void destroyList(Node* head) {
        Node* p = head;
        while (p) {
            Node* q = p;
            p = p->next;
            delete q;
        }
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空双向链表
     */
    DoublyLinkedList() : _size(0) {
        _head = new Node(T());
        _tail = new Node(T());
        _head->next = _tail;
        _tail->prev = _head;
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    DoublyLinkedList(std::initializer_list<T> ilist) : DoublyLinkedList() {
        for (const T& val : ilist) {
            pushBack(val);
        }
    }
    
    /**
     * @brief 填充构造函数
     * @param count 元素个数
     * @param val 元素值
     */
    DoublyLinkedList(std::size_t count, const T& val) : DoublyLinkedList() {
        for (std::size_t i = 0; i < count; ++i) {
            pushBack(val);
        }
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 释放所有节点内存
     */
    ~DoublyLinkedList() {
        destroyList(_head);
        // _tail 会被一起销毁
    }
    
    // ---- 拷贝控制（五法则） ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    DoublyLinkedList(const DoublyLinkedList& other) : _size(other._size) {
        _head = new Node(T());
        _tail = new Node(T());
        _head->next = _tail;
        _tail->prev = _head;
        
        Node* p = _head;
        Node* otherP = other._head->next;
        while (otherP != other._tail) {
            Node* newNode = new Node(otherP->data);
            newNode->prev = p;
            p->next = newNode;
            p = newNode;
            otherP = otherP->next;
        }
        p->next = _tail;
        _tail->prev = p;
    }
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    DoublyLinkedList(DoublyLinkedList&& other) noexcept 
        : _head(other._head), _tail(other._tail), _size(other._size) {
        other._head = nullptr;
        other._tail = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return DoublyLinkedList&
     * 时间复杂度：O(n)
     */
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            DoublyLinkedList tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return DoublyLinkedList&
     * 时间复杂度：O(1)
     */
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            this->~DoublyLinkedList();
            _head = other._head;
            _tail = other._tail;
            _size = other._size;
            other._head = nullptr;
            other._tail = nullptr;
            other._size = 0;
        }
        return *this;
    }
    
    /**
     * @brief 交换两个链表
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(DoublyLinkedList& other) {
        std::swap(_head, other._head);
        std::swap(_tail, other._tail);
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
        return getNode(pos)->data;
    }
    
    /**
     * @brief 获取指定位置的元素（非常量版本）
     * @param pos 位置（从1开始）
     * @return T& 元素引用
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    T& at(std::size_t pos) {
        return getNode(pos)->data;
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
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return _head->next->data;
    }
    
    /**
     * @brief 获取最后一个元素
     * @return const T& 元素引用
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(1)
     */
    const T& back() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return _tail->prev->data;
    }
    
    /**
     * @brief 获取最后一个元素（非常量版本）
     * @return T& 元素引用
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(1)
     */
    T& back() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return _tail->prev->data;
    }
    
    // ---- 插入操作 ----
    
    /**
     * @brief 在头部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     */
    void pushFront(const T& val) {
        Node* newNode = new Node(val);
        newNode->prev = _head;
        newNode->next = _head->next;
        _head->next->prev = newNode;
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
        newNode->prev = _head;
        newNode->next = _head->next;
        _head->next->prev = newNode;
        _head->next = newNode;
        ++_size;
    }
    
    /**
     * @brief 在尾部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     */
    void pushBack(const T& val) {
        Node* newNode = new Node(val);
        newNode->prev = _tail->prev;
        newNode->next = _tail;
        _tail->prev->next = newNode;
        _tail->prev = newNode;
        ++_size;
    }
    
    /**
     * @brief 在尾部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void pushBack(T&& val) {
        Node* newNode = new Node(std::move(val));
        newNode->prev = _tail->prev;
        newNode->next = _tail;
        _tail->prev->next = newNode;
        _tail->prev = newNode;
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
        
        if (pos == _size + 1) {
            pushBack(val);
            return;
        }
        
        Node* p = getNode(pos);
        Node* newNode = new Node(val);
        newNode->prev = p->prev;
        newNode->next = p;
        p->prev->next = newNode;
        p->prev = newNode;
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
        
        if (pos == _size + 1) {
            pushBack(std::move(val));
            return;
        }
        
        Node* p = getNode(pos);
        Node* newNode = new Node(std::move(val));
        newNode->prev = p->prev;
        newNode->next = p;
        p->prev->next = newNode;
        p->prev = newNode;
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
        p->next->prev = _head;
        delete p;
        --_size;
        return data;
    }
    
    /**
     * @brief 删除尾部元素
     * @return T 被删除的元素
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(1)
     */
    T popBack() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        
        Node* p = _tail->prev;
        T data = p->data;
        _tail->prev = p->prev;
        p->prev->next = _tail;
        delete p;
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
        
        Node* p = getNode(pos);
        T data = p->data;
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        --_size;
        return data;
    }
    
    /**
     * @brief 删除第一个匹配的元素
     * @param val 要删除的值
     * @return bool 是否成功删除
     * 时间复杂度：O(n)
     */
    bool erase(const T& val) {
        Node* p = _head->next;
        while (p != _tail) {
            if (p->data == val) {
                p->prev->next = p->next;
                p->next->prev = p->prev;
                delete p;
                --_size;
                return true;
            }
            p = p->next;
        }
        return false;
    }
    
    /**
     * @brief 删除所有匹配的元素
     * @param val 要删除的值
     * @return std::size_t 删除的元素个数
     * 时间复杂度：O(n)
     */
    std::size_t eraseAll(const T& val) {
        std::size_t count = 0;
        Node* p = _head->next;
        while (p != _tail) {
            if (p->data == val) {
                Node* toDelete = p;
                p = p->next;
                toDelete->prev->next = toDelete->next;
                toDelete->next->prev = toDelete->prev;
                delete toDelete;
                --_size;
                ++count;
            } else {
                p = p->next;
            }
        }
        return count;
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
        while (p != _tail) {
            if (p->data == val) {
                return pos;
            }
            p = p->next;
            ++pos;
        }
        return 0;
    }
    
    /**
     * @brief 查找元素的前驱
     * @param val 要查找的值
     * @return const T& 前驱元素的引用
     * @throws std::out_of_range 如果元素不存在或没有前驱
     * 时间复杂度：O(n)
     */
    const T& predecessor(const T& val) const {
        Node* p = _head->next;
        while (p != _tail) {
            if (p->data == val) {
                if (p->prev == _head) {
                    throw std::out_of_range("Element has no predecessor");
                }
                return p->prev->data;
            }
            p = p->next;
        }
        throw std::out_of_range("Element not found");
    }
    
    /**
     * @brief 查找元素的后继
     * @param val 要查找的值
     * @return const T& 后继元素的引用
     * @throws std::out_of_range 如果元素不存在或没有后继
     * 时间复杂度：O(n)
     */
    const T& successor(const T& val) const {
        Node* p = _head->next;
        while (p != _tail) {
            if (p->data == val) {
                if (p->next == _tail) {
                    throw std::out_of_range("Element has no successor");
                }
                return p->next->data;
            }
            p = p->next;
        }
        throw std::out_of_range("Element not found");
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
     * @brief 正向遍历并打印所有元素
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void print(std::ostream& os = std::cout) const {
        Node* p = _head->next;
        while (p != _tail) {
            os << p->data << "\t";
            p = p->next;
        }
        os << "\n";
    }
    
    /**
     * @brief 反向遍历并打印所有元素
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void printReverse(std::ostream& os = std::cout) const {
        Node* p = _tail->prev;
        while (p != _head) {
            os << p->data << "\t";
            p = p->prev;
        }
        os << "\n";
    }
    
    /**
     * @brief 正向遍历并执行回调函数
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void forEach(std::function<void(const T&)> callback) const {
        Node* p = _head->next;
        while (p != _tail) {
            callback(p->data);
            p = p->next;
        }
    }
    
    /**
     * @brief 反向遍历并执行回调函数
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void forEachReverse(std::function<void(const T&)> callback) const {
        Node* p = _tail->prev;
        while (p != _head) {
            callback(p->data);
            p = p->prev;
        }
    }
    
    // ---- 操作 ----
    
    /**
     * @brief 清空所有元素
     * 时间复杂度：O(n)
     */
    void clear() {
        Node* p = _head->next;
        while (p != _tail) {
            Node* q = p;
            p = p->next;
            delete q;
        }
        _head->next = _tail;
        _tail->prev = _head;
        _size = 0;
    }
    
    /**
     * @brief 反转链表
     * 时间复杂度：O(n)
     */
    void reverse() {
        if (_size <= 1) return;
        
        Node* current = _head;
        Node* temp = nullptr;
        
        while (current) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }
        
        // 交换头尾哨兵
        std::swap(_head, _tail);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(DoublyLinkedList<T>& a, DoublyLinkedList<T>& b) {
    a.swap(b);
}

#endif // DOUBLY_LINKED_LIST_HPP