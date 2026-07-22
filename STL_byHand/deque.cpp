/**
 * @brief 双端队列（Deque）实现
 * 使用中控器（map）+ 缓冲区的分块存储结构
 * 支持 O(1) 的头尾插入/删除和 O(1) 的随机访问
 * 
 * 核心设计：
 * - map：指针数组，每个元素指向一个 Buffer
 * - Buffer：固定大小的数组，存储 T 类型元素
 * - 通过 start_buf/start_pos 和 end_buf/end_pos 定位头尾
 */
template<typename T>
class MyDeque {
private:
    // ---- 常量 ----
    static constexpr std::size_t BUFFER_SIZE = 8;  // 每个缓冲区的大小
    
    // ---- 缓冲区结构 ----
    struct Buffer {
        T data[BUFFER_SIZE];  // 固定大小数组，存储实际数据
    };
    
    // ---- 成员变量 ----
    Buffer** map;              // 中控器（指针数组），每个元素指向一个 Buffer
    std::size_t map_size;      // 中控器的容量（指针个数）
    std::size_t start_buf;     // 起始缓冲区在 map 中的索引
    std::size_t start_pos;     // 起始缓冲区内的位置 [0, BUFFER_SIZE)
    std::size_t end_buf;       // 结束缓冲区在 map 中的索引
    std::size_t end_pos;       // 结束缓冲区内的位置 [0, BUFFER_SIZE)
    std::size_t _size;         // 当前元素个数

    // ---- 内部辅助函数 ----
    
    /**
     * @brief 分配中控器（map）
     * @param n 中控器大小（指针个数）
     * 时间复杂度：O(n)
     * 初始化所有指针为 nullptr
     */
    void allocate_map(std::size_t n) {
        map = new Buffer*[n];
        for (std::size_t i = 0; i < n; ++i) map[i] = nullptr;
        map_size = n;
    }

    /**
     * @brief 分配一个缓冲区
     * @param idx 在 map 中的索引
     * 时间复杂度：O(1)
     * 如果该位置已有缓冲区，则不重复分配
     */
    void allocate_buffer(std::size_t idx) {
        if (!map[idx]) map[idx] = new Buffer;
    }

    /**
     * @brief 释放一个缓冲区
     * @param idx 在 map 中的索引
     * 时间复杂度：O(1)
     * 释放后指针置空，防止悬空
     */
    void deallocate_buffer(std::size_t idx) {
        if (map[idx]) {
            delete map[idx];
            map[idx] = nullptr;
        }
    }

    /**
     * @brief 重新分配中控器（扩容）
     * 时间复杂度：O(map_size)
     * 将 map 扩容为 2 倍，旧数据移到中间位置
     * 这样头尾都有增长空间，减少未来扩容次数
     */
    void reallocate_map() {
        std::size_t new_size = map_size * 2;  // 翻倍扩容
        Buffer** new_map = new Buffer*[new_size];
        for (std::size_t i = 0; i < new_size; ++i) new_map[i] = nullptr;
        
        // 将旧数据移到新 map 的中间位置
        std::size_t offset = (new_size - map_size) / 2;
        for (std::size_t i = 0; i < map_size; ++i) {
            new_map[i + offset] = map[i];
        }
        
        // 更新起始和结束缓冲区索引
        start_buf += offset;
        end_buf += offset;
        
        // 释放旧 map，更新为新 map
        delete[] map;
        map = new_map;
        map_size = new_size;
    }

    /**
     * @brief 确保头部有空间
     * 时间复杂度：均摊 O(1)
     * 如果头部缓冲区已满（start_pos == 0），则向前扩展
     */
    void ensure_space_front() {
        if (start_pos == 0) {
            if (start_buf == 0) reallocate_map();  // map 头部没有空间了
            --start_buf;          // 前移一个缓冲区
            allocate_buffer(start_buf);  // 分配新缓冲区
            start_pos = BUFFER_SIZE;  // 从缓冲区尾部开始（从后往前填）
        }
    }

    /**
     * @brief 确保尾部有空间
     * 时间复杂度：均摊 O(1)
     * 如果尾部缓冲区已满（end_pos == BUFFER_SIZE），则向后扩展
     */
    void ensure_space_back() {
        if (end_pos == BUFFER_SIZE) {
            if (end_buf == map_size - 1) reallocate_map();  // map 尾部没有空间了
            ++end_buf;            // 后移一个缓冲区
            allocate_buffer(end_buf);  // 分配新缓冲区
            end_pos = 0;          // 从缓冲区头部开始（从头往后填）
        }
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 时间复杂度：O(1)
     * 初始化 map 大小为 4，分配一个缓冲区
     * 起始和结束位置都在缓冲区中间（为头尾扩展留空间）
     */
    MyDeque() : map(nullptr), map_size(0), start_buf(0), start_pos(0), 
                end_buf(0), end_pos(0), _size(0) {
        allocate_map(4);          // 初始分配 4 个指针
        allocate_buffer(0);       // 分配第 0 个缓冲区
        start_buf = end_buf = 0;
        start_pos = end_pos = BUFFER_SIZE / 2;  // 从中间开始，两端都能扩展
    }

    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 时间复杂度：O(n)
     * 先清空所有元素，再释放所有缓冲区和 map
     */
    ~MyDeque() {
        clear();                  // 析构所有元素
        for (std::size_t i = 0; i < map_size; ++i) deallocate_buffer(i);  // 释放所有缓冲区
        delete[] map;             // 释放 map
    }

    // ---- 头尾操作（O(1) 均摊） ----
    
    /**
     * @brief 在头部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：均摊 O(1)
     */
    void push_front(const T& val) {
        ensure_space_front();
        --start_pos;  // 前移一位
        new (&map[start_buf]->data[start_pos]) T(val);  // 在当前位置构造对象
        ++_size;
    }

    /**
     * @brief 在尾部插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：均摊 O(1)
     */
    void push_back(const T& val) {
        ensure_space_back();
        new (&map[end_buf]->data[end_pos]) T(val);  // 在当前尾部构造对象
        ++end_pos;
        ++_size;
    }

    /**
     * @brief 删除头部元素
     * 时间复杂度：O(1)
     */
    void pop_front() {
        if (_size == 0) return;
        map[start_buf]->data[start_pos].~T();  // 析构头部元素
        ++start_pos;
        if (start_pos == BUFFER_SIZE) {        // 当前缓冲区已空
            deallocate_buffer(start_buf);      // 释放空缓冲区
            ++start_buf;                       // 移动到下一个缓冲区
            start_pos = 0;
        }
        --_size;
        if (_size == 0) {                      // 如果全部清空，重置到初始状态
            start_buf = end_buf = 0;
            start_pos = end_pos = BUFFER_SIZE / 2;
        }
    }

    /**
     * @brief 删除尾部元素
     * 时间复杂度：O(1)
     */
    void pop_back() {
        if (_size == 0) return;
        --end_pos;
        map[end_buf]->data[end_pos].~T();      // 析构尾部元素
        if (end_pos == 0) {                    // 当前缓冲区已空
            deallocate_buffer(end_buf);        // 释放空缓冲区
            --end_buf;                         // 移动到前一个缓冲区
            end_pos = BUFFER_SIZE;
        }
        --_size;
        if (_size == 0) {                      // 如果全部清空，重置到初始状态
            start_buf = end_buf = 0;
            start_pos = end_pos = BUFFER_SIZE / 2;
        }
    }

    // ---- 随机访问（O(1)） ----
    
    /**
     * @brief 下标访问（非常量版本）
     * @param idx 索引
     * @return T& 元素引用
     * 时间复杂度：O(1)
     * 不进行边界检查
     */
    T& operator[](std::size_t idx) {
        std::size_t buf_idx = start_buf + (start_pos + idx) / BUFFER_SIZE;
        std::size_t pos = (start_pos + idx) % BUFFER_SIZE;
        return map[buf_idx]->data[pos];
    }

    /**
     * @brief 下标访问（常量版本）
     * @param idx 索引
     * @return const T& 常量引用
     * 时间复杂度：O(1)
     */
    const T& operator[](std::size_t idx) const {
        std::size_t buf_idx = start_buf + (start_pos + idx) / BUFFER_SIZE;
        std::size_t pos = (start_pos + idx) % BUFFER_SIZE;
        return map[buf_idx]->data[pos];
    }

    /**
     * @brief 带边界检查的访问
     * @param idx 索引
     * @return T& 元素引用
     * 时间复杂度：O(1)
     * @throws std::out_of_range 如果 idx >= _size
     */
    T& at(std::size_t idx) {
        if (idx >= _size) throw std::out_of_range("out of range");
        return operator[](idx);
    }

    // ---- 容量 ----
    
    /**
     * @brief 返回元素个数
     * @return std::size_t 当前元素个数
     * 时间复杂度：O(1)
     */
    std::size_t size() const { return _size; }
    
    /**
     * @brief 判断是否为空
     * @return bool 是否为空
     * 时间复杂度：O(1)
     */
    bool empty() const { return _size == 0; }

    // ---- 访问首尾元素 ----
    
    /**
     * @brief 访问第一个元素（非常量版本）
     * @return T& 第一个元素的引用
     * 时间复杂度：O(1)
     */
    T& front() { return map[start_buf]->data[start_pos]; }
    
    /**
     * @brief 访问第一个元素（常量版本）
     * @return const T& 第一个元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& front() const { return map[start_buf]->data[start_pos]; }
    
    /**
     * @brief 访问最后一个元素（非常量版本）
     * @return T& 最后一个元素的引用
     * 时间复杂度：O(1)
     * 注意：end_pos 指向尾后位置，所以最后一个元素在 end_pos - 1
     * 如果 end_pos == 0，则在前一个缓冲区的末尾
     */
    T& back() { 
        std::size_t pos = (end_pos == 0) ? BUFFER_SIZE - 1 : end_pos - 1;
        std::size_t buf = (end_pos == 0) ? end_buf - 1 : end_buf;
        return map[buf]->data[pos];
    }
    
    /**
     * @brief 访问最后一个元素（常量版本）
     * @return const T& 最后一个元素的常量引用
     * 时间复杂度：O(1)
     */
    const T& back() const {
        std::size_t pos = (end_pos == 0) ? BUFFER_SIZE - 1 : end_pos - 1;
        std::size_t buf = (end_pos == 0) ? end_buf - 1 : end_buf;
        return map[buf]->data[pos];
    }

    // ---- 清空 ----
    
    /**
     * @brief 清空所有元素
     * 时间复杂度：O(n)
     * 通过反复 pop_back 来析构所有元素
     * 释放空缓冲区，但保留一个缓冲区用于后续操作
     */
    void clear() {
        while (_size > 0) pop_back();
    }

    // ---- 迭代器（简单实现） ----
    
    /**
     * @brief 迭代器类
     * 提供遍历 deque 的接口
     * 时间复杂度：所有操作 O(1)
     */
    class Iterator {
    private:
        MyDeque* deque;      // 指向所属 deque
        std::size_t index;   // 逻辑索引
        
    public:
        Iterator(MyDeque* d, std::size_t i) : deque(d), index(i) {}
        
        /**
         * @brief 解引用运算符
         * @return T& 当前元素的引用
         */
        T& operator*() { return (*deque)[index]; }
        
        /**
         * @brief 前置递增
         * @return Iterator& 移动到下一个元素
         */
        Iterator& operator++() { ++index; return *this; }
        
        /**
         * @brief 不等比较
         * @param other 另一个迭代器
         * @return bool 是否指向不同位置
         */
        bool operator!=(const Iterator& other) const { return index != other.index; }
    };

    /**
     * @brief 返回指向第一个元素的迭代器
     * @return Iterator 指向第一个元素
     * 时间复杂度：O(1)
     */
    Iterator begin() { return Iterator(this, 0); }
    
    /**
     * @brief 返回指向尾后位置的迭代器
     * @return Iterator 指向尾后位置
     * 时间复杂度：O(1)
     */
    Iterator end() { return Iterator(this, _size); }
};

