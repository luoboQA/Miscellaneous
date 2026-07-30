template<typename T, std::size_t N>
class MyArray {
private:
    T data[N];

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 时间复杂度：O(1)
     * 注意：对于内置类型（如 int），元素不会被初始化，值是不确定的
     */
    MyArray() = default;

    /**
     * @brief 列表初始化构造函数
     * @param init 初始化列表
     * 时间复杂度：O(N)
     * 1. 拷贝 init 中的元素到 data
     * 2. 如果 init.size() < N，用 T{} 填充剩余元素
     * @throws std::out_of_range 如果 init.size() > N
     */
    MyArray(std::initializer_list<T> init) {
        if (init.size() > N) throw std::out_of_range("too many initializers");
        std::copy(init.begin(), init.end(), data);
        if (init.size() < N) {
            std::fill(data + init.size(), data + N, T{});
        }
    }

    /**
     * @brief 拷贝构造函数
     * @param other 要拷贝的对象
     * 时间复杂度：O(N)
     * 深拷贝所有元素
     */
    MyArray(const MyArray& other) {
        std::copy(other.data, other.data + N, data);
    }

    /**
     * @brief 移动构造函数
     * @param other 要移动的对象
     * 时间复杂度：O(N)
     * 逐元素移动（对元素类型 T 调用移动构造）
     * 注意：数组本身没有移动语义，必须逐个元素移动
     */
    MyArray(MyArray&& other) noexcept {
        for (size_t i = 0; i < N; ++i) {
            data[i] = std::move(other.data[i]);
        }
    }

    /**
     * @brief 拷贝赋值运算符
     * @param other 要拷贝的对象
     * @return MyArray& 当前对象的引用
     * 时间复杂度：O(N)
     * 先检查自赋值，再逐元素拷贝
     */
    MyArray& operator=(const MyArray& other) {
        if (this != &other) {
            std::copy(other.data, other.data + N, data);
        }
        return *this;
    }

    /**
     * @brief 移动赋值运算符
     * @param other 要移动的对象
     * @return MyArray& 当前对象的引用
     * 时间复杂度：O(N)
     * 逐元素移动赋值
     */
    MyArray& operator=(MyArray&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < N; ++i) {
                data[i] = std::move(other.data[i]);
            }
        }
        return *this;
    }

    // ---- 访问（随机访问 O(1)） ----
    
    /**
     * @brief 下标访问（非常量版本）
     * @param idx 索引
     * @return T& 元素引用
     * 时间复杂度：O(1)
     * 不进行边界检查（与原生数组行为一致）
     */
    T& operator[](std::size_t idx) { return data[idx]; }
    
    /**
     * @brief 下标访问（常量版本）
     * @param idx 索引
     * @return const T& 常量引用
     * 时间复杂度：O(1)
     */
    const T& operator[](std::size_t idx) const { return data[idx]; }

    /**
     * @brief 带边界检查的访问（非常量版本）
     * @param idx 索引
     * @return T& 元素引用
     * 时间复杂度：O(1)
     * @throws std::out_of_range 如果 idx >= N
     */
    T& at(std::size_t idx) {
        if (idx >= N) throw std::out_of_range("out of range");
        return data[idx];
    }

    /**
     * @brief 带边界检查的访问（常量版本）
     * @param idx 索引
     * @return const T& 常量引用
     * 时间复杂度：O(1)
     * @throws std::out_of_range 如果 idx >= N
     */
    const T& at(std::size_t idx) const {
        if (idx >= N) throw std::out_of_range("out of range");
        return data[idx];
    }

    /**
     * @brief 访问第一个元素（非常量版本）
     * @return T& 第一个元素的引用
     * 时间复杂度：O(1)
     * 前提：N > 0
     */
    T& front() { return data[0]; }

    /**
     * @brief 访问第一个元素（常量版本）
     * @return const T& 第一个元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& front() const { return data[0]; }

    /**
     * @brief 访问最后一个元素（非常量版本）
     * @return T& 最后一个元素的引用
     * 时间复杂度：O(1)
     * 前提：N > 0
     */
    T& back() { return data[N - 1]; }

    /**
     * @brief 访问最后一个元素（常量版本）
     * @return const T& 最后一个元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& back() const { return data[N - 1]; }

    /**
     * @brief 获取底层数组指针（非常量版本）
     * @return T* 指向 data 的指针
     * 时间复杂度：O(1)
     */
    T* data() { return data; }

    /**
     * @brief 获取底层数组指针（常量版本）
     * @return const T* 指向 data 的常量指针
     * 时间复杂度：O(1)
     */
    const T* data() const { return data; }

    // ---- 迭代器 ----
    
    /**
     * @brief 返回指向第一个元素的迭代器（非常量版本）
     * @return T* 迭代器
     * 时间复杂度：O(1)
     */
    T* begin() { return data; }

    /**
     * @brief 返回指向最后一个元素之后的迭代器（非常量版本）
     * @return T* 迭代器
     * 时间复杂度：O(1)
     */
    T* end() { return data + N; }

    /**
     * @brief 返回指向第一个元素的迭代器（常量版本）
     * @return const T* 迭代器
     * 时间复杂度：O(1)
     */
    const T* begin() const { return data; }

    /**
     * @brief 返回指向最后一个元素之后的迭代器（常量版本）
     * @return const T* 迭代器
     * 时间复杂度：O(1)
     */
    const T* end() const { return data + N; }

    /**
     * @brief 返回指向第一个元素的 const 迭代器
     * @return const T* const 迭代器
     * 时间复杂度：O(1)
     */
    const T* cbegin() const { return data; }

    /**
     * @brief 返回指向最后一个元素之后的 const 迭代器
     * @return const T* const 迭代器
     * 时间复杂度：O(1)
     */
    const T* cend() const { return data + N; }

    // ---- 容量 ----
    
    /**
     * @brief 返回数组大小（元素个数）
     * @return std::size_t N（模板参数）
     * 时间复杂度：O(1)
     * 编译期常量
     */
    std::size_t size() const { return N; }

    /**
     * @brief 判断数组是否为空
     * @return bool N == 0
     * 时间复杂度：O(1)
     * 编译期常量
     */
    bool empty() const { return N == 0; }

    // ---- 修改 ----
    
    /**
     * @brief 用指定值填充所有元素
     * @param val 要填充的值
     * 时间复杂度：O(N)
     */
    void fill(const T& val) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] = val;
        }
    }

    /**
     * @brief 交换两个数组的内容
     * @param other 要交换的数组
     * 时间复杂度：O(N)
     * 逐元素交换
     */
    void swap(MyArray& other) {
        for (std::size_t i = 0; i < N; ++i) {
            T tmp = data[i];
            data[i] = other.data[i];
            other.data[i] = tmp;
        }
    }

    /**
     * @brief 从原生数组赋值
     * @param arr 原生数组引用
     * 时间复杂度：O(N)
     * 拷贝 arr 的所有元素到 data
     */
    void assign(const T (&arr)[N]) {
        std::copy(std::begin(arr), std::end(arr), data);
    }

    // ---- 友元函数 ----
    
    /**
     * @brief 外部 swap 重载（支持 ADL）
     * @param a 第一个数组
     * @param b 第二个数组
     * 时间复杂度：O(N)
     * 调用成员 swap 实现
     * 让 std::swap 和泛型代码能高效交换 MyArray
     */
    friend void swap(MyArray& a, MyArray& b) {
        a.swap(b);
    }
};