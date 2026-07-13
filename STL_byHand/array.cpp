template<typename T, std::size_t N>
class MyArray {
private:
    T data[N];

public:
    // 随机访问
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
    
    // 迭代器
    T* begin() { return data; }
    T* end() { return data + N; }
    const T* begin() const { return data; }
    const T* end() const { return data + N; }
    
    // 容量
    std::size_t size() const { return N; }
    bool empty() const { return N == 0; }
    
    // 访问
    T& front() { return data[0]; }
    const T& front() const { return data[0]; }
    T& back() { return data[N-1]; }
    const T& back() const { return data[N-1]; }
    T* data() { return data; }
    const T* data() const { return data; }
    
    // 修改
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
};