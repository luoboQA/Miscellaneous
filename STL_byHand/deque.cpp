template<typename T>
class MyDeque {
private:
    static constexpr std::size_t BUFFER_SIZE = 8;
    
    struct Buffer {
        T data[BUFFER_SIZE];
    };
    
    Buffer** map;        // 中控器（指针数组）
    std::size_t map_size;
    std::size_t start_buf;   // 起始缓冲区索引
    std::size_t start_pos;   // 起始缓冲区内的位置
    std::size_t end_buf;     // 结束缓冲区索引
    std::size_t end_pos;     // 结束缓冲区内的位置
    std::size_t _size;

    void allocate_map(std::size_t n) {
        map = new Buffer*[n];
        for (std::size_t i = 0; i < n; ++i) map[i] = nullptr;
        map_size = n;
    }

    void allocate_buffer(std::size_t idx) {
        if (!map[idx]) map[idx] = new Buffer;
    }

    void deallocate_buffer(std::size_t idx) {
        if (map[idx]) {
            delete map[idx];
            map[idx] = nullptr;
        }
    }

    void reallocate_map() {
        std::size_t new_size = map_size * 2;
        Buffer** new_map = new Buffer*[new_size];
        for (std::size_t i = 0; i < new_size; ++i) new_map[i] = nullptr;
        
        // 将旧数据移到中间
        std::size_t offset = (new_size - map_size) / 2;
        for (std::size_t i = 0; i < map_size; ++i) {
            new_map[i + offset] = map[i];
        }
        
        start_buf += offset;
        end_buf += offset;
        delete[] map;
        map = new_map;
        map_size = new_size;
    }

    void ensure_space_front() {
        if (start_pos == 0) {
            if (start_buf == 0) reallocate_map();
            --start_buf;
            allocate_buffer(start_buf);
            start_pos = BUFFER_SIZE;
        }
    }

    void ensure_space_back() {
        if (end_pos == BUFFER_SIZE) {
            if (end_buf == map_size - 1) reallocate_map();
            ++end_buf;
            allocate_buffer(end_buf);
            end_pos = 0;
        }
    }

public:
    MyDeque() : map(nullptr), map_size(0), start_buf(0), start_pos(0), 
                end_buf(0), end_pos(0), _size(0) {
        allocate_map(4);
        allocate_buffer(0);
        start_buf = end_buf = 0;
        start_pos = end_pos = BUFFER_SIZE / 2;
    }

    ~MyDeque() {
        clear();
        for (std::size_t i = 0; i < map_size; ++i) deallocate_buffer(i);
        delete[] map;
    }

    // 头尾操作 O(1)
    void push_front(const T& val) {
        ensure_space_front();
        --start_pos;
        new (&map[start_buf]->data[start_pos]) T(val);
        ++_size;
    }

    void push_back(const T& val) {
        ensure_space_back();
        new (&map[end_buf]->data[end_pos]) T(val);
        ++end_pos;
        ++_size;
    }

    void pop_front() {
        if (_size == 0) return;
        map[start_buf]->data[start_pos].~T();
        ++start_pos;
        if (start_pos == BUFFER_SIZE) {
            deallocate_buffer(start_buf);
            ++start_buf;
            start_pos = 0;
        }
        --_size;
        if (_size == 0) {
            start_buf = end_buf = 0;
            start_pos = end_pos = BUFFER_SIZE / 2;
        }
    }

    void pop_back() {
        if (_size == 0) return;
        --end_pos;
        map[end_buf]->data[end_pos].~T();
        if (end_pos == 0) {
            deallocate_buffer(end_buf);
            --end_buf;
            end_pos = BUFFER_SIZE;
        }
        --_size;
        if (_size == 0) {
            start_buf = end_buf = 0;
            start_pos = end_pos = BUFFER_SIZE / 2;
        }
    }

    // 随机访问 O(1)
    T& operator[](std::size_t idx) {
        std::size_t buf_idx = start_buf + (start_pos + idx) / BUFFER_SIZE;
        std::size_t pos = (start_pos + idx) % BUFFER_SIZE;
        return map[buf_idx]->data[pos];
    }

    const T& operator[](std::size_t idx) const {
        std::size_t buf_idx = start_buf + (start_pos + idx) / BUFFER_SIZE;
        std::size_t pos = (start_pos + idx) % BUFFER_SIZE;
        return map[buf_idx]->data[pos];
    }

    T& at(std::size_t idx) {
        if (idx >= _size) throw std::out_of_range("out of range");
        return operator[](idx);
    }

    // 容量
    std::size_t size() const { return _size; }
    bool empty() const { return _size == 0; }

    // 访问
    T& front() { return map[start_buf]->data[start_pos]; }
    const T& front() const { return map[start_buf]->data[start_pos]; }
    T& back() { 
        std::size_t pos = (end_pos == 0) ? BUFFER_SIZE - 1 : end_pos - 1;
        std::size_t buf = (end_pos == 0) ? end_buf - 1 : end_buf;
        return map[buf]->data[pos];
    }
    const T& back() const {
        std::size_t pos = (end_pos == 0) ? BUFFER_SIZE - 1 : end_pos - 1;
        std::size_t buf = (end_pos == 0) ? end_buf - 1 : end_buf;
        return map[buf]->data[pos];
    }

    // 清空
    void clear() {
        while (_size > 0) pop_back();
    }

    // 迭代器（简单实现）
    class Iterator {
    private:
        MyDeque* deque;
        std::size_t index;
    public:
        Iterator(MyDeque* d, std::size_t i) : deque(d), index(i) {}
        T& operator*() { return (*deque)[index]; }
        Iterator& operator++() { ++index; return *this; }
        bool operator!=(const Iterator& other) const { return index != other.index; }
    };

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, _size); }
};

// ========== 测试代码 ==========
#include <iostream>
int main() {
    MyDeque<int> dq;
    
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    dq.push_front(0);
    dq.push_front(-1);
    
    std::cout << "Size: " << dq.size() << std::endl;  // 5
    std::cout << "Front: " << dq.front() << std::endl;  // -1
    std::cout << "Back: " << dq.back() << std::endl;    // 3
    std::cout << "dq[2] = " << dq[2] << std::endl;      // 1
    
    dq.pop_front();
    dq.pop_back();
    std::cout << "After pop: " << dq.front() << ", " << dq.back() << std::endl;  // 0, 2
    
    std::cout << "Elements: ";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    return 0;
}