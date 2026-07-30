// SequentialStack.hpp
#ifndef SEQUENTIAL_STACK_HPP
#define SEQUENTIAL_STACK_HPP

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <functional>

/**
 * @brief 顺序栈模板（带自动扩容）
 * @tparam T 元素类型
 * 
 * 时间复杂度：
 * - 入栈/出栈：O(1)
 * - 取栈顶：O(1)
 * - 空间复杂度：O(n)
 */
template<typename T>
class SequentialStack {
private:
    T* _data;
    std::size_t _top;
    std::size_t _capacity;
    std::size_t _increment;
    
    void ensureCapacity() {
        if (_top >= _capacity) {
            std::size_t newCapacity = _capacity + _increment;
            T* newData = static_cast<T*>(realloc(_data, newCapacity * sizeof(T)));
            if (!newData) {
                throw std::bad_alloc();
            }
            _data = newData;
            _capacity = newCapacity;
        }
    }

public:
    // ---- 构造函数 ----
    
    explicit SequentialStack(std::size_t capacity = 10, std::size_t increment = 5)
        : _top(0), _capacity(capacity), _increment(increment) {
        _data = static_cast<T*>(malloc(capacity * sizeof(T)));
        if (!_data) {
            throw std::bad_alloc();
        }
    }
    
    SequentialStack(std::initializer_list<T> ilist)
        : SequentialStack(ilist.size() * 2, 5) {
        for (const T& val : ilist) {
            push(val);
        }
    }
    
    // ---- 析构函数 ----
    
    ~SequentialStack() {
        while (_top > 0) {
            _data[--_top].~T();
        }
        free(_data);
    }
    
    // ---- 拷贝控制 ----
    
    SequentialStack(const SequentialStack& other)
        : _top(other._top), _capacity(other._capacity), _increment(other._increment) {
        _data = static_cast<T*>(malloc(_capacity * sizeof(T)));
        if (!_data) {
            throw std::bad_alloc();
        }
        for (std::size_t i = 0; i < _top; ++i) {
            new (_data + i) T(other._data[i]);
        }
    }
    
    SequentialStack(SequentialStack&& other) noexcept
        : _data(other._data), _top(other._top), 
          _capacity(other._capacity), _increment(other._increment) {
        other._data = nullptr;
        other._top = other._capacity = other._increment = 0;
    }
    
    SequentialStack& operator=(const SequentialStack& other) {
        if (this != &other) {
            SequentialStack tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    SequentialStack& operator=(SequentialStack&& other) noexcept {
        if (this != &other) {
            this->~SequentialStack();
            _data = other._data;
            _top = other._top;
            _capacity = other._capacity;
            _increment = other._increment;
            other._data = nullptr;
            other._top = other._capacity = other._increment = 0;
        }
        return *this;
    }
    
    void swap(SequentialStack& other) {
        std::swap(_data, other._data);
        std::swap(_top, other._top);
        std::swap(_capacity, other._capacity);
        std::swap(_increment, other._increment);
    }
    
    // ---- 容量 ----
    
    std::size_t size() const { return _top; }
    std::size_t capacity() const { return _capacity; }
    bool empty() const { return _top == 0; }
    
    // ---- 访问 ----
    
    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return _data[_top - 1];
    }
    
    T& top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return _data[_top - 1];
    }
    
    // ---- 修改 ----
    
    void push(const T& val) {
        ensureCapacity();
        new (_data + _top) T(val);
        ++_top;
    }
    
    void push(T&& val) {
        ensureCapacity();
        new (_data + _top) T(std::move(val));
        ++_top;
    }
    
    T pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        --_top;
        T val = std::move(_data[_top]);
        _data[_top].~T();
        return val;
    }
    
    void clear() {
        while (_top > 0) {
            _data[--_top].~T();
        }
    }
    
    // ---- 遍历 ----
    
    void print(std::ostream& os = std::cout) const {
        for (std::size_t i = 0; i < _top; ++i) {
            os << _data[i] << "\t";
        }
        os << "\n";
    }
    
    void forEach(std::function<void(const T&)> callback) const {
        for (std::size_t i = 0; i < _top; ++i) {
            callback(_data[i]);
        }
    }
};

template<typename T>
void swap(SequentialStack<T>& a, SequentialStack<T>& b) {
    a.swap(b);
}

#endif // SEQUENTIAL_STACK_HPP