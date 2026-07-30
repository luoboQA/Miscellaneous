// LinkQueue.hpp
#ifndef LINK_QUEUE_HPP
#define LINK_QUEUE_HPP

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <functional>

/**
 * @brief 链式队列模板
 * @tparam T 元素类型
 * 
 * 链式队列使用带头结点的单链表实现：
 * - front 指向头结点
 * - rear 指向尾结点
 * 
 * 时间复杂度：
 * - 入队：O(1)
 * - 出队：O(1)
 * - 取队头：O(1)
 * - 求长度：O(n)（若维护长度则为O(1)）
 * - 空间复杂度：O(n)
 */
template<typename T>
class LinkQueue {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& val) : data(val), next(nullptr) {}
        Node(T&& val) : data(std::move(val)), next(nullptr) {}
    };
    
    Node* _front;          // 头结点（队头）
    Node* _rear;           // 尾结点（队尾）
    std::size_t _size;     // 元素个数
    
    /**
     * @brief 销毁队列
     */
    void destroy() {
        Node* p = _front;
        while (p) {
            Node* q = p;
            p = p->next;
            delete q;
        }
        _front = _rear = nullptr;
        _size = 0;
    }
    
    /**
     * @brief 拷贝队列
     * @param other 要拷贝的对象
     */
    void copy(const LinkQueue& other) {
        if (other.empty()) {
            _front = new Node(T());
            _rear = _front;
            _size = 0;
            return;
        }
        
        _front = new Node(T());
        _rear = _front;
        _size = 0;
        
        Node* p = other._front->next;
        while (p) {
            push(p->data);
            p = p->next;
        }
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空队列
     */
    LinkQueue() : _size(0) {
        _front = new Node(T());
        _rear = _front;
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    LinkQueue(std::initializer_list<T> ilist) : LinkQueue() {
        for (const T& val : ilist) {
            push(val);
        }
    }
    
    /**
     * @brief 填充构造函数
     * @param count 元素个数
     * @param val 元素值
     */
    LinkQueue(std::size_t count, const T& val) : LinkQueue() {
        for (std::size_t i = 0; i < count; ++i) {
            push(val);
        }
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     */
    ~LinkQueue() {
        destroy();
    }
    
    // ---- 拷贝控制 ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    LinkQueue(const LinkQueue& other) {
        copy(other);
    }
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    LinkQueue(LinkQueue&& other) noexcept 
        : _front(other._front), _rear(other._rear), _size(other._size) {
        other._front = nullptr;
        other._rear = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return LinkQueue&
     * 时间复杂度：O(n)
     */
    LinkQueue& operator=(const LinkQueue& other) {
        if (this != &other) {
            LinkQueue tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return LinkQueue&
     * 时间复杂度：O(1)
     */
    LinkQueue& operator=(LinkQueue&& other) noexcept {
        if (this != &other) {
            this->~LinkQueue();
            _front = other._front;
            _rear = other._rear;
            _size = other._size;
            other._front = nullptr;
            other._rear = nullptr;
            other._size = 0;
        }
        return *this;
    }
    
    /**
     * @brief 交换两个队列
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(LinkQueue& other) {
        std::swap(_front, other._front);
        std::swap(_rear, other._rear);
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
     * @brief 获取队头元素（常量版本）
     * @return const T& 队头元素引用
     * @throws std::out_of_range 如果队列为空
     * 时间复杂度：O(1)
     */
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return _front->next->data;
    }
    
    /**
     * @brief 获取队头元素（非常量版本）
     * @return T& 队头元素引用
     * @throws std::out_of_range 如果队列为空
     * 时间复杂度：O(1)
     */
    T& front() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return _front->next->data;
    }
    
    /**
     * @brief 获取队尾元素（常量版本）
     * @return const T& 队尾元素引用
     * @throws std::out_of_range 如果队列为空
     * 时间复杂度：O(1)
     */
    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return _rear->data;
    }
    
    /**
     * @brief 获取队尾元素（非常量版本）
     * @return T& 队尾元素引用
     * @throws std::out_of_range 如果队列为空
     * 时间复杂度：O(1)
     */
    T& back() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return _rear->data;
    }
    
    // ---- 修改 ----
    
    /**
     * @brief 入队（拷贝版本）
     * @param val 要入队的值
     * 时间复杂度：O(1)
     */
    void push(const T& val) {
        Node* newNode = new Node(val);
        _rear->next = newNode;
        _rear = newNode;
        ++_size;
    }
    
    /**
     * @brief 入队（移动版本）
     * @param val 要入队的值（右值引用）
     * 时间复杂度：O(1)
     */
    void push(T&& val) {
        Node* newNode = new Node(std::move(val));
        _rear->next = newNode;
        _rear = newNode;
        ++_size;
    }
    
    /**
     * @brief 出队
     * @return T 出队的元素
     * @throws std::out_of_range 如果队列为空
     * 时间复杂度：O(1)
     */
    T pop() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        
        Node* p = _front->next;
        T data = std::move(p->data);
        _front->next = p->next;
        
        if (_rear == p) {
            _rear = _front;
        }
        
        delete p;
        --_size;
        return data;
    }
    
    /**
     * @brief 清空队列
     * 时间复杂度：O(n)
     */
    void clear() {
        Node* p = _front->next;
        while (p) {
            Node* q = p;
            p = p->next;
            delete q;
        }
        _front->next = nullptr;
        _rear = _front;
        _size = 0;
    }
    
    // ---- 查找 ----
    
    /**
     * @brief 查找元素
     * @param val 要查找的值
     * @return bool 是否存在
     * 时间复杂度：O(n)
     */
    bool contains(const T& val) const {
        Node* p = _front->next;
        while (p) {
            if (p->data == val) {
                return true;
            }
            p = p->next;
        }
        return false;
    }
    
    /**
     * @brief 查找元素位置（从队头开始，0-based）
     * @param val 要查找的值
     * @return std::size_t 位置，未找到返回 npos
     * 时间复杂度：O(n)
     */
    std::size_t find(const T& val) const {
        std::size_t pos = 0;
        Node* p = _front->next;
        while (p) {
            if (p->data == val) {
                return pos;
            }
            p = p->next;
            ++pos;
        }
        return npos;
    }
    
    // ---- 遍历 ----
    
    /**
     * @brief 打印队列（从队头到队尾）
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void print(std::ostream& os = std::cout) const {
        Node* p = _front->next;
        while (p) {
            os << p->data << " ";
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
        Node* p = _front->next;
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
        Node* p = _front->next;
        while (p) {
            callback(p->data);
            p = p->next;
        }
    }
    
    // ---- 静态常量 ----
    
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(LinkQueue<T>& a, LinkQueue<T>& b) {
    a.swap(b);
}

#endif // LINK_QUEUE_HPP