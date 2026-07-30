template<typename T>
class MyVector {
private:
    T* _start;           // 指向第一个元素
    T* _finish;          // 指向最后一个有效元素的下一个位置
    T* _end_of_storage;  // 指向分配的内存末尾

    /**
     * @brief 重新分配内存（扩容/缩容）
     * @param new_cap 新容量（元素个数）
     * 
     * 时间复杂度：O(n)
     * 1. 分配新内存（使用 operator new）
     * 2. 将旧元素移动构造到新内存（placement new + move）
     * 3. 析构旧元素
     * 4. 释放旧内存
     * 5. 更新指针
     */
    void reallocate(std::size_t new_cap) {
        // 1. 分配原始内存（不构造对象）
        T* new_start = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        
        // 2. 移动构造旧元素到新内存
        for (std::size_t i = 0; i < size(); ++i) {
            new (new_start + i) T(std::move(_start[i]));  // placement new + 移动构造
            _start[i].~T();  // 析构旧元素（但旧内存还没释放）
        }
        
        // 3. 释放旧内存
        ::operator delete(_start);
        
        // 4. 更新指针
        _start = new_start;
        _finish = _start + size();
        _end_of_storage = _start + new_cap;
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空 vector，所有指针为 nullptr
     */
    MyVector() : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {}
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 1. 逐个析构所有元素
     * 2. 释放内存
     */
    ~MyVector() {
        // 逐个调用元素的析构函数
        for (std::size_t i = 0; i < size(); ++i) {
            _start[i].~T();
        }
        // 释放原始内存
        ::operator delete(_start);
    }

    // ---- 访问（随机访问 O(1)） ----
    
    /**
     * @brief 下标访问（非常量版本）
     * @param idx 索引
     * @return T& 元素引用
     * 时间复杂度：O(1)
     * 不进行边界检查（与原生数组行为一致）
     */
    T& operator[](std::size_t idx) { return _start[idx]; }
    
    /**
     * @brief 下标访问（常量版本）
     * @param idx 索引
     * @return const T& 常量引用
     * 时间复杂度：O(1)
     */
    const T& operator[](std::size_t idx) const { return _start[idx]; }
    
    /**
     * @brief 带边界检查的访问
     * @param idx 索引
     * @return T& 元素引用
     * 时间复杂度：O(1)
     * @throws std::out_of_range 如果 idx >= size()
     */
    T& at(std::size_t idx) {
        if (idx >= size()) throw std::out_of_range("out of range");
        return _start[idx];
    }

    // ---- 迭代器 ----
    
    T* begin() { return _start; }
    T* end() { return _finish; }
    const T* begin() const { return _start; }
    const T* end() const { return _finish; }

    // ---- 容量 ----
    
    /**
     * @brief 返回当前元素个数
     * 时间复杂度：O(1)
     */
    std::size_t size() const { return _finish - _start; }
    
    /**
     * @brief 返回当前容量（已分配内存可容纳的元素个数）
     * 时间复杂度：O(1)
     */
    std::size_t capacity() const { return _end_of_storage - _start; }
    
    /**
     * @brief 判断是否为空
     * 时间复杂度：O(1)
     */
    bool empty() const { return _start == _finish; }

    // ---- 尾部操作 ----
    
    /**
     * @brief 在尾部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：均摊 O(1)
     * 如果容量不足，会触发 reallocate（O(n)）
     */
    void push_back(const T& val) {
        if (_finish == _end_of_storage) {
            // 扩容策略：空时分配 1，否则翻倍
            std::size_t new_cap = empty() ? 1 : capacity() * 2;
            reallocate(new_cap);
        }
        // 在 _finish 位置构造元素（拷贝构造）
        new (_finish) T(val);
        ++_finish;
    }

    /**
     * @brief 在尾部插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：均摊 O(1)
     * 相比拷贝版本，减少一次拷贝
     */
    void push_back(T&& val) {
        if (_finish == _end_of_storage) {
            std::size_t new_cap = empty() ? 1 : capacity() * 2;
            reallocate(new_cap);
        }
        // 在 _finish 位置构造元素（移动构造）
        new (_finish) T(std::move(val));
        ++_finish;
    }

    /**
     * @brief 删除尾部元素
     * 时间复杂度：O(1)
     * 不会缩容（容量不变）
     */
    void pop_back() {
        if (!empty()) {
            --_finish;
            _finish->~T();  // 析构尾部元素
        }
    }

    // ---- 插入操作 ----
    
    /**
     * @brief 在头部插入元素
     * @param val 要插入的值
     * 时间复杂度：O(n)
     * 需要将所有元素后移一位
     */
    void insert_head(const T& val) {
        insert(begin(), val);
    }

    /**
     * @brief 在任意位置插入元素
     * @param pos 插入位置（迭代器）
     * @param val 要插入的值
     * 时间复杂度：O(n)（移动元素 + 可能的扩容）
     * 
     * 实现细节：
     * 1. 如果容量不足，先扩容
     * 2. 从尾部到 pos，逐个将元素后移一位
     * 3. 在 pos 位置构造新元素
     */
    void insert(T* pos, const T& val) {
        

        // 如果容量不足，先扩容
        if (_finish == _end_of_storage) {
            std::size_t new_cap = empty() ? 1 : capacity() * 2;
            reallocate(new_cap);
        }
        
        // 从后往前移动元素（先移动最后一个，避免覆盖）
        for (T* p = _finish; p > pos; --p) {
            // 把 p-1 的元素移动构造到 p 位置
            new (p) T(std::move(*(p - 1)));
            // 析构 p-1 位置的对象（但还没赋值，后面会覆盖）
            (p - 1)->~T();
        }
        
        // 在 pos 位置构造新元素
        new (pos) T(val);
        ++_finish;
    }

    // ---- 删除操作 ----
    
    /**
     * @brief 删除头部元素
     * 时间复杂度：O(n)
     * 需要将所有元素前移一位
     */
    void erase_head() {
        if (!empty()) erase(begin());
    }

    /**
     * @brief 删除任意位置的元素
     * @param pos 要删除的位置（迭代器）
     * 时间复杂度：O(n)
     * 
     * 实现细节：
     * 1. 先析构 pos 位置的对象
     * 2. 从 pos+1 到末尾，逐个将元素前移一位
     * 3. 更新 _finish
     */
    void erase(T* pos) {
        if (pos < _finish) {
            // 1. 析构要删除的元素
            pos->~T();
            
            // 2. 将后续元素前移
            for (T* p = pos; p < _finish - 1; ++p) {
                // 把 p+1 的元素移动构造到 p 位置
                new (p) T(std::move(*(p + 1)));
                // 析构 p+1 位置的对象
                (p + 1)->~T();
            }
            
            // 3. 更新 _finish
            --_finish;
        }
    }

    // ---- 批量操作 ----
    
    /**
     * @brief 清空所有元素
     * 时间复杂度：O(n)
     * 逐个析构所有元素，但保留内存
     */
    void clear() {
        for (std::size_t i = 0; i < size(); ++i) {
            _start[i].~T();
        }
        _finish = _start;
    }

    /**
     * @brief 预留空间
     * @param n 预留的容量
     * 时间复杂度：O(n)（如果重新分配）
     * 如果 n > 当前容量，重新分配；否则不做任何事
     */
    void reserve(std::size_t n) {
        if (n > capacity()) {
            reallocate(n);
        }
    }

    /**
     * @brief 收缩容量到恰好容纳当前元素
     * 时间复杂度：O(n)
     * 释放多余内存
     */
    void shrink_to_fit() {
        if (size() < capacity()) {
            reallocate(size());
        }
    }

    // ---- 拷贝控制（五法则） ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     * 深拷贝所有元素
     */
    MyVector(const MyVector& other) {
        std::size_t n = other.size();
        // 分配内存
        _start = static_cast<T*>(::operator new(n * sizeof(T)));
        // 逐个拷贝构造元素
        for (std::size_t i = 0; i < n; ++i) {
            new (_start + i) T(other._start[i]);
        }
        _finish = _start + n;
        _end_of_storage = _finish;
    }

    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     * 直接窃取资源，other 变为空
     */
    MyVector(MyVector&& other) noexcept
        : _start(other._start), _finish(other._finish), _end_of_storage(other._end_of_storage) {
        // 让 other 进入空状态（析构时不会释放我们的资源）
        other._start = other._finish = other._end_of_storage = nullptr;
    }

    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     * 使用 copy-and-swap 惯用法，提供强异常安全保证
     */
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            MyVector tmp(other);  // 拷贝构造临时对象
            swap(tmp);            // 交换，临时对象销毁时释放旧资源
        }
        return *this;
    }

    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     * 直接窃取资源，other 变为空
     */
    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            // 先析构当前对象
            this->~MyVector();
            // 窃取资源
            _start = other._start;
            _finish = other._finish;
            _end_of_storage = other._end_of_storage;
            // 让 other 进入空状态
            other._start = other._finish = other._end_of_storage = nullptr;
        }
        return *this;
    }

    /**
     * @brief 交换两个 vector
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     * 只交换三个指针
     */
    void swap(MyVector& other) {
        std::swap(_start, other._start);
        std::swap(_finish, other._finish);
        std::swap(_end_of_storage, other._end_of_storage);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * 让 std::swap 和泛型代码能高效交换 MyVector
 */
template<typename T>
void swap(MyVector<T>& a, MyVector<T>& b) {
    a.swap(b);
}