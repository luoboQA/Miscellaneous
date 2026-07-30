// SequentialList.hpp
#ifndef SEQUENTIAL_LIST_HPP
#define SEQUENTIAL_LIST_HPP

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <functional>

/**
 * @brief 顺序表模板（带自动扩容机制）
 * @tparam T 元素类型
 * 
 * 时间复杂度：
 * - 随机访问：O(1)
 * - 插入/删除（表尾）：均摊 O(1)
 * - 插入/删除（任意位置）：O(n)
 * - 查找：O(n)
 * - 空间复杂度：O(n)
 */
template<typename T>
class SequentialList {
private:
    T* _data;              // 数据数组
    std::size_t _size;     // 当前元素个数
    std::size_t _capacity; // 当前容量
    std::size_t _increment; // 每次扩容增加的数量
    
    /**
     * @brief 扩容
     * @throws std::bad_alloc 如果内存分配失败
     */
    void increaseCapacity() {
        std::size_t newCapacity = _capacity + _increment;
        T* newData = static_cast<T*>(realloc(_data, newCapacity * sizeof(T)));
        if (!newData) {
            throw std::bad_alloc();
        }
        _data = newData;
        _capacity = newCapacity;
    }
    
    /**
     * @brief 检查并扩容
     */
    void ensureCapacity() {
        if (_size >= _capacity) {
            increaseCapacity();
        }
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * @param capacity 初始容量
     * @param increment 扩容增量
     */
    explicit SequentialList(std::size_t capacity = 10, std::size_t increment = 5)
        : _size(0), _capacity(capacity), _increment(increment) {
        _data = static_cast<T*>(malloc(capacity * sizeof(T)));
        if (!_data) {
            throw std::bad_alloc();
        }
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    SequentialList(std::initializer_list<T> ilist)
        : SequentialList(ilist.size() * 2, std::max<std::size_t>(5, ilist.size())) {
        for (const T& val : ilist) {
            pushBack(val);
        }
    }
    
    /**
     * @brief 填充构造函数
     * @param count 元素个数
     * @param val 元素值
     */
    SequentialList(std::size_t count, const T& val)
        : SequentialList(count * 2, 5) {
        for (std::size_t i = 0; i < count; ++i) {
            pushBack(val);
        }
    }
    
    // ---- 析构函数 ----
    
    ~SequentialList() {
        // 调用析构函数
        for (std::size_t i = 0; i < _size; ++i) {
            _data[i].~T();
        }
        free(_data);
    }
    
    // ---- 拷贝控制 ----
    
    SequentialList(const SequentialList& other)
        : _size(other._size), _capacity(other._capacity), _increment(other._increment) {
        _data = static_cast<T*>(malloc(_capacity * sizeof(T)));
        if (!_data) {
            throw std::bad_alloc();
        }
        for (std::size_t i = 0; i < _size; ++i) {
            new (_data + i) T(other._data[i]);
        }
    }
    
    SequentialList(SequentialList&& other) noexcept
        : _data(other._data), _size(other._size), 
          _capacity(other._capacity), _increment(other._increment) {
        other._data = nullptr;
        other._size = other._capacity = other._increment = 0;
    }
    
    SequentialList& operator=(const SequentialList& other) {
        if (this != &other) {
            SequentialList tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    SequentialList& operator=(SequentialList&& other) noexcept {
        if (this != &other) {
            this->~SequentialList();
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
            _increment = other._increment;
            other._data = nullptr;
            other._size = other._capacity = other._increment = 0;
        }
        return *this;
    }
    
    void swap(SequentialList& other) {
        std::swap(_data, other._data);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
        std::swap(_increment, other._increment);
    }
    
    // ---- 容量 ----
    
    std::size_t size() const { return _size; }
    std::size_t capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }
    
    // ---- 访问 ----
    
    T& at(std::size_t pos) {
        if (pos >= _size) {
            throw std::out_of_range("Position out of range");
        }
        return _data[pos];
    }
    
    const T& at(std::size_t pos) const {
        if (pos >= _size) {
            throw std::out_of_range("Position out of range");
        }
        return _data[pos];
    }
    
    T& operator[](std::size_t pos) { return _data[pos]; }
    const T& operator[](std::size_t pos) const { return _data[pos]; }
    
    T& front() {
        if (empty()) throw std::out_of_range("List is empty");
        return _data[0];
    }
    const T& front() const {
        if (empty()) throw std::out_of_range("List is empty");
        return _data[0];
    }
    
    T& back() {
        if (empty()) throw std::out_of_range("List is empty");
        return _data[_size - 1];
    }
    const T& back() const {
        if (empty()) throw std::out_of_range("List is empty");
        return _data[_size - 1];
    }
    
    // ---- 插入 ----
    
    void pushBack(const T& val) {
        ensureCapacity();
        new (_data + _size) T(val);
        ++_size;
    }
    
    void pushBack(T&& val) {
        ensureCapacity();
        new (_data + _size) T(std::move(val));
        ++_size;
    }
    
    void insert(std::size_t pos, const T& val) {
        if (pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        ensureCapacity();
        
        for (std::size_t i = _size; i > pos; --i) {
            new (_data + i) T(std::move(_data[i - 1]));
            _data[i - 1].~T();
        }
        new (_data + pos) T(val);
        ++_size;
    }
    
    void insert(std::size_t pos, T&& val) {
        if (pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        ensureCapacity();
        
        for (std::size_t i = _size; i > pos; --i) {
            new (_data + i) T(std::move(_data[i - 1]));
            _data[i - 1].~T();
        }
        new (_data + pos) T(std::move(val));
        ++_size;
    }
    
    // ---- 删除 ----
    
    T popBack() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        T val = std::move(_data[_size - 1]);
        _data[_size - 1].~T();
        --_size;
        return val;
    }
    
    T erase(std::size_t pos) {
        if (pos >= _size) {
            throw std::out_of_range("Position out of range");
        }
        T val = std::move(_data[pos]);
        _data[pos].~T();
        for (std::size_t i = pos; i < _size - 1; ++i) {
            new (_data + i) T(std::move(_data[i + 1]));
            _data[i + 1].~T();
        }
        --_size;
        return val;
    }
    
    void clear() {
        for (std::size_t i = 0; i < _size; ++i) {
            _data[i].~T();
        }
        _size = 0;
    }
    
    // ---- 查找 ----
    
    std::size_t find(const T& val) const {
        for (std::size_t i = 0; i < _size; ++i) {
            if (_data[i] == val) return i;
        }
        return npos;
    }
    
    bool contains(const T& val) const {
        return find(val) != npos;
    }
    
    // ---- 遍历 ----
    
    void print(std::ostream& os = std::cout) const {
        for (std::size_t i = 0; i < _size; ++i) {
            os << _data[i] << "\t";
        }
        os << "\n";
    }
    
    void forEach(std::function<void(const T&)> callback) const {
        for (std::size_t i = 0; i < _size; ++i) {
            callback(_data[i]);
        }
    }
    
    // ---- 常量 ----
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);
};

template<typename T>
void swap(SequentialList<T>& a, SequentialList<T>& b) {
    a.swap(b);
}

#endif // SEQUENTIAL_LIST_HPP