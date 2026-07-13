template<typename T>
class MyVector {
private:
    T* _start;
    T* _finish;
    T* _end_of_storage;

    void reallocate(std::size_t new_cap) {
        T* new_start = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        for (std::size_t i = 0; i < size(); ++i) {
            new (new_start + i) T(std::move(_start[i]));
            _start[i].~T();
        }
        ::operator delete(_start);
        _start = new_start;
        _finish = _start + size();
        _end_of_storage = _start + new_cap;
    }

public:
    MyVector() : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {}
    
    ~MyVector() {
        for (std::size_t i = 0; i < size(); ++i) _start[i].~T();
        ::operator delete(_start);
    }

    // 随机访问 O(1)
    T& operator[](std::size_t idx) { return _start[idx]; }
    const T& operator[](std::size_t idx) const { return _start[idx]; }
    
    T& at(std::size_t idx) {
        if (idx >= size()) throw std::out_of_range("out of range");
        return _start[idx];
    }

    // 迭代器
    T* begin() { return _start; }
    T* end() { return _finish; }
    const T* begin() const { return _start; }
    const T* end() const { return _finish; }

    // 容量
    std::size_t size() const { return _finish - _start; }
    std::size_t capacity() const { return _end_of_storage - _start; }
    bool empty() const { return _start == _finish; }

    // 尾部插入 O(1) 均摊
    void push_back(const T& val) {
        if (_finish == _end_of_storage) {
            std::size_t new_cap = empty() ? 1 : capacity() * 2;
            reallocate(new_cap);
        }
        new (_finish) T(val);
        ++_finish;
    }

    void push_back(T&& val) {
        if (_finish == _end_of_storage) {
            std::size_t new_cap = empty() ? 1 : capacity() * 2;
            reallocate(new_cap);
        }
        new (_finish) T(std::move(val));
        ++_finish;
    }

    // 尾部删除 O(1)
    void pop_back() {
        if (!empty()) {
            --_finish;
            _finish->~T();
        }
    }

    // 头部插入 O(n)
    void insert_head(const T& val) {
        insert(begin(), val);
    }

    // 任意位置插入 O(n)
    void insert(T* pos, const T& val) {
        if (_finish == _end_of_storage) {
            std::size_t new_cap = empty() ? 1 : capacity() * 2;
            reallocate(new_cap);
        }
        for (T* p = _finish; p > pos; --p) {
            new (p) T(std::move(*(p-1)));
            (p-1)->~T();
        }
        new (pos) T(val);
        ++_finish;
    }

    // 头部删除 O(n)
    void erase_head() {
        if (!empty()) erase(begin());
    }

    // 任意位置删除 O(n)
    void erase(T* pos) {
        if (pos < _finish) {
            pos->~T();
            for (T* p = pos; p < _finish - 1; ++p) {
                new (p) T(std::move(*(p+1)));
                (p+1)->~T();
            }
            --_finish;
        }
    }

    // 清空
    void clear() {
        for (std::size_t i = 0; i < size(); ++i) _start[i].~T();
        _finish = _start;
    }

    // 预留空间
    void reserve(std::size_t n) {
        if (n > capacity()) reallocate(n);
    }

    // 收缩到合适大小
    void shrink_to_fit() {
        if (size() < capacity()) reallocate(size());
    }

    // 拷贝构造
    MyVector(const MyVector& other) {
        std::size_t n = other.size();
        _start = static_cast<T*>(::operator new(n * sizeof(T)));
        for (std::size_t i = 0; i < n; ++i) new (_start + i) T(other._start[i]);
        _finish = _start + n;
        _end_of_storage = _finish;
    }

    // 移动构造
    MyVector(MyVector&& other) noexcept
        : _start(other._start), _finish(other._finish), _end_of_storage(other._end_of_storage) {
        other._start = other._finish = other._end_of_storage = nullptr;
    }

    // 拷贝赋值
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            MyVector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            this->~MyVector();
            _start = other._start;
            _finish = other._finish;
            _end_of_storage = other._end_of_storage;
            other._start = other._finish = other._end_of_storage = nullptr;
        }
        return *this;
    }

    void swap(MyVector& other) {
        std::swap(_start, other._start);
        std::swap(_finish, other._finish);
        std::swap(_end_of_storage, other._end_of_storage);
    }
};