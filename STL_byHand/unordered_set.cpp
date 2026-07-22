/**
 * @brief 无序集合（Unordered Set）容器
 * 
 * 基于哈希表实现，元素唯一，自动去重
 * 元素不排序，平均 O(1) 的插入/删除/查找
 * 
 * 核心设计：
 * - 哈希表：桶数组（buckets），每个桶是一个链表
 * - 哈希函数：std::hash<Key> 计算哈希值
 * - 负载因子：控制扩容时机（默认 0.75）
 * - 链地址法：解决哈希冲突
 * 
 * 与 MyUnorderedMultiset 的区别：
 * - MyUnorderedSet：元素唯一，插入重复元素会失败
 * - MyUnorderedMultiset：元素可重复，插入永远不会失败
 * 
 * 与 MySet 的区别：
 * - MySet：红黑树，O(log n)，元素有序
 * - MyUnorderedSet：哈希表，O(1) 平均，元素无序
 * 
 * 时间复杂度（平均）：
 * - 插入：O(1)
 * - 删除：O(1)
 * - 查找：O(1)
 * - 最坏情况：O(n)（所有元素在同一桶中）
 * 
 * @tparam Key 元素类型（必须支持 std::hash 和 == 比较）
 */
template<typename Key>
class MyUnorderedSet {
private:
    // ---- 节点结构 ----
    struct Node {
        Key data;        // 存储的数据
        Node* next;      // 指向下一个节点（链地址法）
        
        /**
         * @brief 节点构造函数
         * @param k 存储的值（默认构造）
         * @param n 下一个节点指针
         */
        Node(const Key& k = Key(), Node* n = nullptr) : data(k), next(n) {}

        /**
         * @brief 节点构造函数
         * @param k 存储的值（右值引用）
         * @param n 下一个节点指针
         */
        Node(Key&& k, Node* n = nullptr) : data(std::move(k)), next(n) {}
    };
    
    // ---- 成员变量 ----
    Node** buckets;           // 桶数组（每个元素是链表的头指针）
    std::size_t bucket_count; // 桶的数量
    std::size_t _size;        // 元素个数
    float max_load_factor;    // 最大负载因子（默认 0.75）

    // ---- 哈希函数 ----
    
    /**
     * @brief 计算键的哈希值并映射到桶索引
     * @param key 要哈希的值
     * @return std::size_t 桶索引 [0, bucket_count)
     * 时间复杂度：O(1)
     * 
     * 使用 std::hash<Key> 计算哈希值
     * 使用取模运算映射到桶索引
     */
    std::size_t hash(const Key& key) const {
        return std::hash<Key>{}(key) % bucket_count;
    }

    // ---- 重新哈希 ----
    
    /**
     * @brief 重新哈希（扩容或缩容）
     * @param new_bucket_count 新的桶数量
     * 时间复杂度：O(n)
     * 
     * 实现细节：
     * 1. 创建新的桶数组
     * 2. 遍历所有旧桶中的节点
     * 3. 重新计算每个节点的哈希值
     * 4. 将节点插入到新桶中
     * 5. 释放旧桶数组
     */
    void rehash(std::size_t new_bucket_count) {
        // 保存旧桶数组的指针和大小
        Node** old_buckets = buckets;
        std::size_t old_count = bucket_count;
        
        // 创建新桶数组
        bucket_count = new_bucket_count;
        buckets = new Node*[bucket_count]();  // () 初始化为 nullptr
        _size = 0;  // 重置大小，insert_node 会重新计数
        
        // 遍历所有旧桶
        for (std::size_t i = 0; i < old_count; ++i) {
            Node* cur = old_buckets[i];
            while (cur) {
                Node* next = cur->next;
                insert_node(cur);  // 重新插入到新桶
                cur = next;
            }
            // 注意：不能 delete old_buckets[i]，因为节点已被移动到新桶
        }
        
        // 释放旧桶数组（节点已经在新桶中，只释放桶数组本身）
        delete[] old_buckets;
    }

    /**
     * @brief 插入节点到哈希表（不检查重复）
     * @param node 要插入的节点
     * 时间复杂度：O(1) 平均
     * 
     * 头插法：新节点插入到桶的头部
     */
    void insert_node(Node* node) {
        std::size_t idx = hash(node->data);
        node->next = buckets[idx];
        buckets[idx] = node;
        ++_size;
    }

    /**
     * @brief 查找值对应的节点
     * @param key 要查找的值
     * @return Node* 找到的节点，未找到返回 nullptr
     * 时间复杂度：O(1) 平均
     */
    Node* find_node(const Key& key) const {
        std::size_t idx = hash(key);
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->data == key) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    /**
     * @brief 检查是否需要扩容
     * 时间复杂度：O(1) 平均
     * 
     * 负载因子 = _size / bucket_count
     * 如果负载因子 > max_load_factor，触发扩容（桶数量翻倍）
     */
    void check_rehash() {
        if ((float)_size / bucket_count > max_load_factor) {
            rehash(bucket_count * 2);
        }
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空的哈希表，初始桶数量为 8
     * 时间复杂度：O(1)
     */
    MyUnorderedSet() : bucket_count(8), _size(0), max_load_factor(0.75f) {
        buckets = new Node*[bucket_count]();  // () 初始化为 nullptr
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 释放所有节点和桶数组
     * 时间复杂度：O(n)
     */
    ~MyUnorderedSet() {
        clear();         // 释放所有节点
        delete[] buckets; // 释放桶数组
    }

    // ---- 迭代器 ----
    
    /**
     * @brief 哈希表迭代器
     * 遍历所有桶中的节点（顺序与插入顺序无关）
     * 支持正向遍历（++）
     */
    class Iterator {
    private:
        Node* ptr;                // 当前节点指针
        Node** buckets;           // 桶数组指针
        std::size_t bucket_count; // 桶数量
        std::size_t current_bucket; // 当前所在桶索引
        
        /**
         * @brief 推进迭代器到下一个非空节点
         * 时间复杂度：均摊 O(1)
         * 
         * 实现细节：
         * 1. 如果当前节点有 next，直接移动到 next
         * 2. 否则，跳到下一个非空桶
         * 3. 如果所有桶都遍历完，ptr = nullptr
         */
        void advance() {
            if (!ptr) return;
            
            // 尝试移动到当前桶的下一个节点
            ptr = ptr->next;
            
            // 如果当前桶已经遍历完，跳到下一个非空桶
            if (!ptr) {
                ++current_bucket;
                while (current_bucket < bucket_count && !buckets[current_bucket]) {
                    ++current_bucket;
                }
                if (current_bucket < bucket_count) {
                    ptr = buckets[current_bucket];
                }
            }
        }
        
    public:
        Iterator(Node* p = nullptr, Node** b = nullptr, 
                 std::size_t bc = 0, std::size_t cb = 0)
            : ptr(p), buckets(b), bucket_count(bc), current_bucket(cb) {}
        
        /**
         * @brief 解引用运算符
         * @return Key& 当前元素的引用
         */
        Key& operator*() { return ptr->data; }
        
        /**
         * @brief 前置递增（++it）
         * @return Iterator& 移动到下一个元素
         * 时间复杂度：均摊 O(1)
         */
        Iterator& operator++() { advance(); return *this; }
        
        /**
         * @brief 后置递增（it++）
         * @return Iterator 移动前的迭代器副本
         * 时间复杂度：均摊 O(1)
         */
        Iterator operator++(int) { Iterator tmp = *this; advance(); return tmp; }
        
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    /**
     * @brief 返回指向第一个元素的迭代器
     * @return Iterator 指向第一个非空桶的第一个元素
     * 时间复杂度：O(bucket_count) 最坏，O(1) 平均
     */
    Iterator begin() const {
        for (std::size_t i = 0; i < bucket_count; ++i) {
            if (buckets[i]) {
                return Iterator(buckets[i], buckets, bucket_count, i);
            }
        }
        return end();
    }
    
    /**
     * @brief 返回指向尾后位置的迭代器
     * @return Iterator nullptr 迭代器
     * 时间复杂度：O(1)
     */
    Iterator end() const {
        return Iterator(nullptr, buckets, bucket_count, bucket_count);
    }

    // ---- 容量 ----
    
    bool empty() const { return _size == 0; }
    std::size_t size() const { return _size; }
    std::size_t bucket_count() const { return bucket_count; }

    // ---- 插入 ----
    
    /**
     * @brief 插入元素
     * @param key 要插入的值
     * @return bool 是否插入成功（元素已存在时返回 false）
     * 时间复杂度：O(1) 平均
     * 
     * 实现细节：
     * 1. 先查找元素是否已存在
     * 2. 如果不存在，检查是否需要扩容
     * 3. 头插法插入新节点
     */
    bool insert(const Key& key) {
        if (find_node(key)) return false;  // 元素已存在，拒绝插入
        
        check_rehash();  // 检查是否需要扩容
        
        // 头插法插入新节点
        Node* new_node = new Node(key);
        std::size_t idx = hash(key);
        new_node->next = buckets[idx];
        buckets[idx] = new_node;
        ++_size;
        return true;
    }

    // ---- 查找 ----
    
    /**
     * @brief 查找值是否存在
     * @param key 要查找的值
     * @return bool 值是否存在
     * 时间复杂度：O(1) 平均
     */
    bool find(const Key& key) const {
        return find_node(key) != nullptr;
    }

    // ---- 删除 ----
    
    /**
     * @brief 删除指定值的元素
     * @param key 要删除的值
     * @return bool 是否删除成功（值不存在时返回 false）
     * 时间复杂度：O(1) 平均
     * 
     * 实现细节：
     * 1. 计算哈希值找到桶索引
     * 2. 遍历链表，找到值匹配的节点
     * 3. 更新前驱节点的 next 指针
     * 4. 删除节点
     */
    bool erase(const Key& key) {
        std::size_t idx = hash(key);
        Node* cur = buckets[idx];
        Node* prev = nullptr;
        
        while (cur) {
            if (cur->data == key) {
                if (prev) prev->next = cur->next;
                else buckets[idx] = cur->next;
                delete cur;
                --_size;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // ---- 清空 ----
    
    /**
     * @brief 清空所有元素
     * 时间复杂度：O(n)
     * 释放所有节点，但保留桶数组
     */
    void clear() {
        for (std::size_t i = 0; i < bucket_count; ++i) {
            Node* cur = buckets[i];
            while (cur) {
                Node* next = cur->next;
                delete cur;
                cur = next;
            }
            buckets[i] = nullptr;
        }
        _size = 0;
    }

    // ---- 预留空间 ----
    
    /**
     * @brief 预留空间
     * @param count 要容纳的元素个数
     * 时间复杂度：O(n)
     * 
     * 计算需要的桶数量：count / max_load_factor + 1
     * 如果大于当前桶数量，触发 rehash
     */
    void reserve(std::size_t count) {
        std::size_t new_bucket_count = count / max_load_factor + 1;
        if (new_bucket_count > bucket_count) {
            rehash(new_bucket_count);
        }
    }

    // ---- 拷贝控制（五法则） ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    MyUnorderedSet(const MyUnorderedSet& other) 
        : bucket_count(other.bucket_count), _size(0), max_load_factor(other.max_load_factor) {
        buckets = new Node*[bucket_count]();
        for (std::size_t i = 0; i < bucket_count; ++i) {
            Node* cur = other.buckets[i];
            while (cur) {
                insert(cur->data);
                cur = cur->next;
            }
        }
    }

    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    MyUnorderedSet(MyUnorderedSet&& other) noexcept
        : buckets(other.buckets), bucket_count(other.bucket_count), 
          _size(other._size), max_load_factor(other.max_load_factor) {
        other.buckets = nullptr;
        other.bucket_count = 0;
        other._size = 0;
    }

    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return MyUnorderedSet& 当前对象的引用
     * 时间复杂度：O(n)
     * 使用 copy-and-swap 惯用法
     */
    MyUnorderedSet& operator=(const MyUnorderedSet& other) {
        if (this != &other) {
            MyUnorderedSet tmp(other);
            swap(tmp);
        }
        return *this;
    }

    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return MyUnorderedSet& 当前对象的引用
     * 时间复杂度：O(1)
     */
    MyUnorderedSet& operator=(MyUnorderedSet&& other) noexcept {
        if (this != &other) {
            clear();
            delete[] buckets;
            buckets = other.buckets;
            bucket_count = other.bucket_count;
            _size = other._size;
            max_load_factor = other.max_load_factor;
            other.buckets = nullptr;
            other.bucket_count = 0;
            other._size = 0;
        }
        return *this;
    }

    /**
     * @brief 交换两个 unordered_set
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(MyUnorderedSet& other) {
        std::swap(buckets, other.buckets);
        std::swap(bucket_count, other.bucket_count);
        std::swap(_size, other._size);
        std::swap(max_load_factor, other.max_load_factor);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * 时间复杂度：O(1)
 */
template<typename Key>
void swap(MyUnorderedSet<Key>& a, MyUnorderedSet<Key>& b) {
    a.swap(b);
}
