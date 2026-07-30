// LinkedList.hpp
#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <functional>

/**
 * @brief 单链表模板
 * @tparam T 元素类型
 * 
 * 单链表是一种线性表，每个节点包含数据域和指向下一个节点的指针。
 * 
 * 时间复杂度：
 * - 头插/头删：O(1)
 * - 尾插/尾删：O(n)（不维护尾指针）
 * - 插入/删除（任意位置）：O(n)
 * - 查找：O(n)
 * - 空间复杂度：O(n)
 */
template<typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& val) : data(val), next(nullptr) {}
        Node(T&& val) : data(std::move(val)), next(nullptr) {}
    };
    
    Node* _head;           // 头节点指针（不带头结点）
    std::size_t _size;     // 元素个数
    
    /**
     * @brief 获取指定位置的节点
     * @param pos 位置（从0开始）
     * @return Node* 节点指针
     * @throws std::out_of_range 如果位置无效
     */
    Node* getNode(std::size_t pos) const {
        if (pos >= _size) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* p = _head;
        for (std::size_t i = 0; i < pos; ++i) {
            p = p->next;
        }
        return p;
    }
    
    /**
     * @brief 获取指定位置的前驱节点
     * @param pos 位置（从0开始）
     * @return Node* 前驱节点指针
     */
    Node* getPreNode(std::size_t pos) const {
        if (pos == 0 || pos > _size) {
            throw std::out_of_range("Invalid position");
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
     * @return Node* 拷贝后的节点
     */
    Node* copyList(Node* node) {
        if (!node) return nullptr;
        
        Node* newNode = new Node(node->data);
        newNode->next = copyList(node->next);
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
     * 创建空链表
     */
    LinkedList() : _head(nullptr), _size(0) {}
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    LinkedList(std::initializer_list<T> ilist) : _head(nullptr), _size(0) {
        for (const T& val : ilist) {
            pushBack(val);
        }
    }
    
    /**
     * @brief 填充构造函数
     * @param count 元素个数
     * @param val 元素值
     */
    LinkedList(std::size_t count, const T& val) : _head(nullptr), _size(0) {
        for (std::size_t i = 0; i < count; ++i) {
            pushBack(val);
        }
    }
    
    /**
     * @brief 从数组构造
     * @param arr 数组
     * @param n 数组大小
     */
    LinkedList(const T* arr, std::size_t n) : _head(nullptr), _size(0) {
        for (std::size_t i = 0; i < n; ++i) {
            pushBack(arr[i]);
        }
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     */
    ~LinkedList() {
        destroyList(_head);
    }
    
    // ---- 拷贝控制 ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    LinkedList(const LinkedList& other) 
        : _head(copyList(other._head)), _size(other._size) {}
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    LinkedList(LinkedList&& other) noexcept 
        : _head(other._head), _size(other._size) {
        other._head = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return LinkedList&
     * 时间复杂度：O(n)
     */
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            LinkedList tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return LinkedList&
     * 时间复杂度：O(1)
     */
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            this->~LinkedList();
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
    void swap(LinkedList& other) {
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
     * @param pos 位置（从0开始）
     * @return const T& 元素引用
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    const T& at(std::size_t pos) const {
        return getNode(pos)->data;
    }
    
    /**
     * @brief 获取指定位置的元素（非常量版本）
     * @param pos 位置（从0开始）
     * @return T& 元素引用
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    T& at(std::size_t pos) {
        return getNode(pos)->data;
    }
    
    /**
     * @brief 下标访问（不进行边界检查）
     * @param pos 位置（从0开始）
     * @return T& 元素引用
     * 时间复杂度：O(n)
     */
    T& operator[](std::size_t pos) {
        return getNode(pos)->data;
    }
    
    /**
     * @brief 下标访问（不进行边界检查，常量版本）
     * @param pos 位置（从0开始）
     * @return const T& 元素引用
     * 时间复杂度：O(n)
     */
    const T& operator[](std::size_t pos) const {
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
        return _head->data;
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
        return _head->data;
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
        while (p->next) {
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
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        Node* p = _head;
        while (p->next) {
            p = p->next;
        }
        return p->data;
    }
    
    // ---- 插入 ----
    
    /**
     * @brief 在头部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     */
    void pushFront(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = _head;
        _head = newNode;
        ++_size;
    }
    
    /**
     * @brief 在头部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void pushFront(T&& val) {
        Node* newNode = new Node(std::move(val));
        newNode->next = _head;
        _head = newNode;
        ++_size;
    }
    
    /**
     * @brief 在尾部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(n)
     */
    void pushBack(const T& val) {
        Node* newNode = new Node(val);
        if (empty()) {
            _head = newNode;
        } else {
            Node* p = _head;
            while (p->next) {
                p = p->next;
            }
            p->next = newNode;
        }
        ++_size;
    }
    
    /**
     * @brief 在尾部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(n)
     */
    void pushBack(T&& val) {
        Node* newNode = new Node(std::move(val));
        if (empty()) {
            _head = newNode;
        } else {
            Node* p = _head;
            while (p->next) {
                p = p->next;
            }
            p->next = newNode;
        }
        ++_size;
    }
    
    /**
     * @brief 在指定位置插入元素（拷贝版本）
     * @param pos 位置（从0开始）
     * @param val 要插入的值
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    void insert(std::size_t pos, const T& val) {
        if (pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        
        if (pos == 0) {
            pushFront(val);
            return;
        }
        
        Node* prev = getNode(pos - 1);
        Node* newNode = new Node(val);
        newNode->next = prev->next;
        prev->next = newNode;
        ++_size;
    }
    
    /**
     * @brief 在指定位置插入元素（移动版本）
     * @param pos 位置（从0开始）
     * @param val 要插入的值（右值引用）
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    void insert(std::size_t pos, T&& val) {
        if (pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        
        if (pos == 0) {
            pushFront(std::move(val));
            return;
        }
        
        Node* prev = getNode(pos - 1);
        Node* newNode = new Node(std::move(val));
        newNode->next = prev->next;
        prev->next = newNode;
        ++_size;
    }
    
    // ---- 删除 ----
    
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
        
        Node* p = _head;
        T data = std::move(p->data);
        _head = _head->next;
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
        
        if (_size == 1) {
            return popFront();
        }
        
        Node* p = _head;
        while (p->next->next) {
            p = p->next;
        }
        
        T data = std::move(p->next->data);
        delete p->next;
        p->next = nullptr;
        --_size;
        return data;
    }
    
    /**
     * @brief 删除指定位置的元素
     * @param pos 位置（从0开始）
     * @return T 被删除的元素
     * @throws std::out_of_range 如果位置无效
     * 时间复杂度：O(n)
     */
    T erase(std::size_t pos) {
        if (pos >= _size) {
            throw std::out_of_range("Position out of range");
        }
        
        if (pos == 0) {
            return popFront();
        }
        
        Node* prev = getNode(pos - 1);
        Node* p = prev->next;
        T data = std::move(p->data);
        prev->next = p->next;
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
        if (empty()) return false;
        
        if (_head->data == val) {
            popFront();
            return true;
        }
        
        Node* p = _head;
        while (p->next && p->next->data != val) {
            p = p->next;
        }
        
        if (!p->next) return false;
        
        Node* q = p->next;
        p->next = q->next;
        delete q;
        --_size;
        return true;
    }
    
    /**
     * @brief 删除所有匹配的元素
     * @param val 要删除的值
     * @return std::size_t 删除的元素个数
     * 时间复杂度：O(n)
     */
    std::size_t eraseAll(const T& val) {
        std::size_t count = 0;
        
        // 删除头部匹配的元素
        while (!empty() && _head->data == val) {
            popFront();
            ++count;
        }
        
        if (empty()) return count;
        
        // 删除其他匹配的元素
        Node* p = _head;
        while (p->next) {
            if (p->next->data == val) {
                Node* q = p->next;
                p->next = q->next;
                delete q;
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
     * @return std::size_t 元素位置（从0开始），未找到返回 npos
     * 时间复杂度：O(n)
     */
    std::size_t find(const T& val) const {
        std::size_t pos = 0;
        Node* p = _head;
        while (p) {
            if (p->data == val) {
                return pos;
            }
            p = p->next;
            ++pos;
        }
        return npos;
    }
    
    /**
     * @brief 判断是否包含某元素
     * @param val 要查找的值
     * @return bool 是否包含
     * 时间复杂度：O(n)
     */
    bool contains(const T& val) const {
        return find(val) != npos;
    }
    
    // ---- 遍历 ----
    
    /**
     * @brief 打印所有元素
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void print(std::ostream& os = std::cout) const {
        Node* p = _head;
        while (p) {
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
        Node* p = _head;
        while (p) {
            callback(p->data);
            p = p->next;
        }
    }
    
    /**
     * @brief 遍历并执行回调函数（可修改）
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void forEach(std::function<void(T&)> callback) {
        Node* p = _head;
        while (p) {
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
        destroyList(_head);
        _head = nullptr;
        _size = 0;
    }
    
    /**
     * @brief 反转链表
     * 时间复杂度：O(n)
     */
    void reverse() {
        if (_size <= 1) return;
        
        Node* prev = nullptr;
        Node* curr = _head;
        Node* next = nullptr;
        
        while (curr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        _head = prev;
    }
    
    // ---- 静态常量 ----
    
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(LinkedList<T>& a, LinkedList<T>& b) {
    a.swap(b);
}

#endif // LINKED_LIST_HPP