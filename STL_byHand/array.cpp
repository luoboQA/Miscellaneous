template<typename T, std::size_t N>
class MyArray {
private:
    T data[N];

public:
    // ---- 构造函数 ----
    MyArray() = default;

    MyArray(std::initializer_list<T> init) {
        if (init.size() > N) throw std::out_of_range("too many initializers");
        std::copy(init.begin(), init.end(), data);
        if (init.size() < N) {
            std::fill(data + init.size(), data + N, T{});
        }
    }

    MyArray(const MyArray& other) {
        std::copy(other.data, other.data + N, data);
    }

    MyArray(MyArray&& other) noexcept {
        std::move(other.data, other.data + N, data);
    }

    MyArray& operator=(const MyArray& other) {
        if (this != &other) {
            std::copy(other.data, other.data + N, data);
        }
        return *this;
    }

    MyArray& operator=(MyArray&& other) noexcept {
        if (this != &other) {
            std::move(other.data, other.data + N, data);
        }
        return *this;
    }

    // ---- 访问 ----
    T& operator[](std::size_t idx) { return data[idx]; }
    const T& operator[](std::size_t idx) const { return data[idx]; }

    T& at(std::size_t idx) {
        if (idx >= N) throw std::out_of_range("out of range");
        return data[idx];
    }
    const T& at(std::size_t idx) const {
        if (idx >= N) throw std::out_of_range("out of range");
        return data[idx];
    }

    T& front() { return data[0]; }
    const T& front() const { return data[0]; }
    T& back() { return data[N-1]; }
    const T& back() const { return data[N-1]; }
    T* data() { return data; }
    const T* data() const { return data; }

    // ---- 迭代器 ----
    T* begin() { return data; }
    T* end() { return data + N; }
    const T* begin() const { return data; }
    const T* end() const { return data + N; }
    const T* cbegin() const { return data; }
    const T* cend() const { return data + N; }

    // ---- 容量 ----
    std::size_t size() const { return N; }
    bool empty() const { return N == 0; }

    // ---- 修改 ----
    void fill(const T& val) {
        for (std::size_t i = 0; i < N; ++i) data[i] = val;
    }

    void swap(MyArray& other) {
        for (std::size_t i = 0; i < N; ++i) {
            T tmp = data[i];
            data[i] = other.data[i];
            other.data[i] = tmp;
        }
    }

    // 使用示例：赋值整个原生数组
    void assign(const T (&arr)[N]) {
        std::copy(std::begin(arr), std::end(arr), data);
    }

    // 外部 swap 重载（方便 ADL）
    friend void swap(MyArray& a, MyArray& b) {
        a.swap(b);
    }
};