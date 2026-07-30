// CircularQueue.hpp
#ifndef CIRCULAR_QUEUE_HPP
#define CIRCULAR_QUEUE_HPP

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <functional>

/**
 * @brief 循环队列模板
 * @tparam T 元素类型
 * 
 * 使用循环数组实现，牺牲一个单元区分空和满
 * 
 * 时间复杂度：
 * - 入队/出队：O(1)
 * - 取队头/队尾：O(1)
 * - 空间复杂度：O(n)
 */
template<typename T>
class CircularQueue {
private:
    T* _data;
    std::size_t _front;
    std::size_t _rear;
    std::size_t _capacity;
    
public:
    // ---- 构造函数 ----
    
    explicit CircularQueue(std::size_t capacity = 10)
        : _front(0), _rear(0), _capacity(capacity + 1) {
        _data = static_cast<T*>(malloc(_capacity * sizeof(T)));
        if (!_data) {
            throw std::bad_alloc();
        }
    }
    
    CircularQueue(std::initializer_list<T> ilist)
        : CircularQueue(ilist.size() * 2) {
        for (const T& val : ilist) {
            push(val);
        }
    }
    
    // ---- 析构函数 ----
    
    ~CircularQueue() {
        while (!empty()) {
            pop();
        }
        free(_data);
    }
    
    // ---- 拷贝控制 ----
    
    CircularQueue(const CircularQueue& other)
        : _front(other._front), _rear(other._rear), _capacity(other._capacity) {
        _data = static_cast<T*>(malloc(_capacity * sizeof(T)));
        if (!_data) {
            throw std::bad_alloc();
        }
        std::size_t i = other._front;
        while (i != other._rear) {
            new (_data + i) T(other._data[i]);
            i = (i + 1) % _capacity;
        }
    }
    
    CircularQueue(CircularQueue&& other) noexcept
        : _data(other._data), _front(other._front), 
          _rear(other._rear), _capacity(other._capacity) {
        other._data = nullptr;
        other._front = other._rear = other._capacity = 0;
    }
    
    CircularQueue& operator=(const CircularQueue& other) {
        if (this != &other) {
            CircularQueue tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    CircularQueue& operator=(CircularQueue&& other) noexcept {
        if (this != &other) {
            this->~CircularQueue();
            _data = other._data;
            _front = other._front;
            _rear = other._rear;
            _capacity = other._capacity;
            other._data = nullptr;
            other._front = other._rear = other._capacity = 0;
        }
        return *this;
    }
    
    void swap(CircularQueue& other) {
        std::swap(_data, other._data);
        std::swap(_front, other._front);
        std::swap(_rear, other._rear);
        std::swap(_capacity, other._capacity);
    }
    
    // ---- 容量 ----
    
    std::size_t size() const {
        return (_rear - _front + _capacity) % _capacity;
    }
    
    std::size_t capacity() const { return _capacity - 1; }
    
    bool empty() const { return _front == _rear; }
    
    bool full() const { return (_rear + 1) % _capacity == _front; }
    
    // ---- 访问 ----
    
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return _data[_front];
    }
    
    T& front() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return _data[_front];
    }
    
    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return _data[(_rear - 1 + _capacity) % _capacity];
    }
    
    T& back() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return _data[(_rear - 1 + _capacity) % _capacity];
    }
    
    // ---- 修改 ----
    
    void push(const T& val) {
        if (full()) {
            throw std::overflow_error("Queue is full");
        }
        new (_data + _rear) T(val);
        _rear = (_rear + 1) % _capacity;
    }
    
    void push(T&& val) {
        if (full()) {
            throw std::overflow_error("Queue is full");
        }
        new (_data + _rear) T(std::move(val));
        _rear = (_rear + 1) % _capacity;
    }
    
    T pop() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        T val = std::move(_data[_front]);
        _data[_front].~T();
        _front = (_front + 1) % _capacity;
        return val;
    }
    
    void clear() {
        while (!empty()) {
            pop();
        }
    }
    
    // ---- 遍历 ----
    
    void print(std::ostream& os = std::cout) const {
        std::size_t i = _front;
        while (i != _rear) {
            os << _data[i] << "\t";
            i = (i + 1) % _capacity;
        }
        os << "\n";
    }
    
    void forEach(std::function<void(const T&)> callback) const {
        std::size_t i = _front;
        while (i != _rear) {
            callback(_data[i]);
            i = (i + 1) % _capacity;
        }
    }
};

template<typename T>
void swap(CircularQueue<T>& a, CircularQueue<T>& b) {
    a.swap(b);
}

#endif // CIRCULAR_QUEUE_HPP