// LinkedListWithHead.hpp
#ifndef LINKED_LIST_WITH_HEAD_HPP
#define LINKED_LIST_WITH_HEAD_HPP

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <functional>

/**
 * @brief 带头结点且记录头尾指针的单链表模板
 * @tparam T 元素类型
 * 
 * 特点：
 * - 包含头结点（哨兵），简化边界条件
 * - hp 指向第一个数据节点
 * - tp 指向最后一个数据节点
 * - 入队操作 O(1)，无需遍历
 * 
 * 时间复杂度：
 * - 头插/头删：O(1)
 * - 尾插：O(1)（利用 tp 指针）
 * - 尾删：O(n)（需要找到倒数第二个节点）
 * - 插入/删除（任意位置）：O(n)
 * - 查找：O(n)
 * - 空间复杂度：O(n)
 */
template<typename T>
class LinkedListWithHead {
private:
    struct Node {
        T data;
        Node* next;
        
        Node() : next(nullptr) {}
        Node(const T& val) : data(val), next(nullptr) {}
        Node(T&& val) : data(std::move(val)), next(nullptr) {}
    };
    
    Node* _head;           // 头结点（哨兵）
    Node* _first;          // 指向第一个数据节点（hp）
    Node* _last;           // 指向最后一个数据节点（tp）
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
        
        Node* p = _first;
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
        if (pos >= _size) {
            throw std::out_of_range("Position out of range");
        }
        
        if (pos == 0) {
            return _head;
        }
        
        Node* p = _head;
        for (std::size_t i = 0; i < pos; ++i) {
            p = p->next;
        }
        return p;
    }
    
    /**
     * @brief 拷贝链表
     * @param other 要拷贝的对象
     */
    void copy(const LinkedListWithHead& other) {
        _head = new Node();
        _first = nullptr;
        _last = nullptr;
        _size = 0;
        
        Node* p = other._first;
        while (p) {
            pushBack(p->data);
            p = p->next;
        }
    }
    
    /**
     * @brief 销毁链表
     */
    void destroy() {
        Node* p = _head;
        while (p) {
            Node* q = p;
            p = p->next;
            delete q;
        }
        _head = nullptr;
        _first = nullptr;
        _last = nullptr;
        _size = 0;
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建带头结点的空链表
     */
    LinkedListWithHead() : _size(0) {
        _head = new Node();
        _first = nullptr;
        _last = nullptr;
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    LinkedListWithHead(std::initializer_list<T> ilist) : LinkedListWithHead() {
        for (const T& val : ilist) {
            pushBack(val);
        }
    }
    
    /**
     * @brief 填充构造函数
     * @param count 元素个数
     * @param val 元素值
     */
    LinkedListWithHead(std::size_t count, const T& val) : LinkedListWithHead() {
        for (std::size_t i = 0; i < count; ++i) {
            pushBack(val);
        }
    }
    
    /**
     * @brief 从数组构造
     * @param arr 数组
     * @param n 数组大小
     */
    LinkedListWithHead(const T* arr, std::size_t n) : LinkedListWithHead() {
        for (std::size_t i = 0; i < n; ++i) {
            pushBack(arr[i]);
        }
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     */
    ~LinkedListWithHead() {
        destroy();
    }
    
    // ---- 拷贝控制 ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    LinkedListWithHead(const LinkedListWithHead& other) {
        copy(other);
    }
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    LinkedListWithHead(LinkedListWithHead&& other) noexcept 
        : _head(other._head), _first(other._first), 
          _last(other._last), _size(other._size) {
        other._head = nullptr;
        other._first = nullptr;
        other._last = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return LinkedListWithHead&
     * 时间复杂度：O(n)
     */
    LinkedListWithHead& operator=(const LinkedListWithHead& other) {
        if (this != &other) {
            LinkedListWithHead tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return LinkedListWithHead&
     * 时间复杂度：O(1)
     */
    LinkedListWithHead& operator=(LinkedListWithHead&& other) noexcept {
        if (this != &other) {
            this->~LinkedListWithHead();
            _head = other._head;
            _first = other._first;
            _last = other._last;
            _size = other._size;
            other._head = nullptr;
            other._first = nullptr;
            other._last = nullptr;
            other._size = 0;
        }
        return *this;
    }
    
    /**
     * @brief 交换两个链表
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(LinkedListWithHead& other) {
        std::swap(_head, other._head);
        std::swap(_first, other._first);
        std::swap(_last, other._last);
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
        return _first->data;
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
        return _first->data;
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
        return _last->data;
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
        return _last->data;
    }
    
    // ---- 插入 ----
    
    /**
     * @brief 在头部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     */
    void pushFront(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = _first;
        _head->next = newNode;
        if (empty()) {
            _last = newNode;
        }
        _first = newNode;
        ++_size;
    }
    
    /**
     * @brief 在头部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void pushFront(T&& val) {
        Node* newNode = new Node(std::move(val));
        newNode->next = _first;
        _head->next = newNode;
        if (empty()) {
            _last = newNode;
        }
        _first = newNode;
        ++_size;
    }
    
    /**
     * @brief 在尾部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(1)
     */
    void pushBack(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = nullptr;
        
        if (empty()) {
            _head->next = newNode;
            _first = newNode;
        } else {
            _last->next = newNode;
        }
        _last = newNode;
        ++_size;
    }
    
    /**
     * @brief 在尾部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(1)
     */
    void pushBack(T&& val) {
        Node* newNode = new Node(std::move(val));
        newNode->next = nullptr;
        
        if (empty()) {
            _head->next = newNode;
            _first = newNode;
        } else {
            _last->next = newNode;
        }
        _last = newNode;
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
        
        if (pos == _size) {
            pushBack(val);
            return;
        }
        
        Node* prev = getPreNode(pos);
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
        
        if (pos == _size) {
            pushBack(std::move(val));
            return;
        }
        
        Node* prev = getPreNode(pos);
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
        
        Node* p = _first;
        T data = std::move(p->data);
        
        _head->next = p->next;
        _first = p->next;
        
        if (_first == nullptr) {
            _last = nullptr;
        }
        
        delete p;
        --_size;
        return data;
    }
    
    /**
     * @brief 删除尾部元素
     * @return T 被删除的元素
     * @throws std::out_of_range 如果链表为空
     * 时间复杂度：O(n)（需要找到倒数第二个节点）
     */
    T popBack() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        
        if (_size == 1) {
            return popFront();
        }
        
        // 找到倒数第二个节点
        Node* p = _first;
        while (p->next != _last) {
            p = p->next;
        }
        
        T data = std::move(_last->data);
        delete _last;
        _last = p;
        _last->next = nullptr;
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
        
        if (pos == _size - 1) {
            return popBack();
        }
        
        Node* prev = getPreNode(pos);
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
        
        if (_first->data == val) {
            popFront();
            return true;
        }
        
        Node* p = _first;
        while (p->next && p->next->data != val) {
            p = p->next;
        }
        
        if (!p->next) return false;
        
        Node* q = p->next;
        p->next = q->next;
        if (q == _last) {
            _last = p;
        }
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
        while (!empty() && _first->data == val) {
            popFront();
            ++count;
        }
        
        if (empty()) return count;
        
        // 删除其他匹配的元素
        Node* p = _first;
        while (p->next) {
            if (p->next->data == val) {
                Node* q = p->next;
                p->next = q->next;
                if (q == _last) {
                    _last = p;
                }
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
        Node* p = _first;
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
        if (empty()) {
            os << "链表为空\n";
            return;
        }
        
        os << "链表元素（共" << _size << "个）：";
        Node* p = _first;
        while (p) {
            os << p->data << "\t";
            p = p->next;
        }
        os << "\n";
    }
    
    /**
     * @brief 打印链表状态（调试用）
     * @param os 输出流
     */
    void printStatus(std::ostream& os = std::cout) const {
        os << "【状态】first指向：";
        if (empty()) {
            os << "NULL";
        } else {
            os << _first->data;
        }
        os << "，last指向：";
        if (empty()) {
            os << "NULL";
        } else {
            os << _last->data;
        }
        os << "，大小：" << _size << "\n";
    }
    
    /**
     * @brief 遍历并执行回调函数
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void forEach(std::function<void(const T&)> callback) const {
        Node* p = _first;
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
        Node* p = _first;
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
        Node* p = _first;
        while (p) {
            Node* q = p;
            p = p->next;
            delete q;
        }
        _head->next = nullptr;
        _first = nullptr;
        _last = nullptr;
        _size = 0;
    }
    
    /**
     * @brief 反转链表
     * 时间复杂度：O(n)
     */
    void reverse() {
        if (_size <= 1) return;
        
        Node* prev = nullptr;
        Node* curr = _first;
        Node* next = nullptr;
        
        while (curr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        
        _last = _first;
        _first = prev;
        _head->next = _first;
    }
    
    // ---- 静态常量 ----
    
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(LinkedListWithHead<T>& a, LinkedListWithHead<T>& b) {
    a.swap(b);
}

#endif // LINKED_LIST_WITH_HEAD_HPP