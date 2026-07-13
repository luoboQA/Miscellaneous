template<typename Key, typename Value>
class MyUnorderedMultimap {
private:
    struct Node {
        std::pair<const Key, Value> data;
        Node* next;
        Node(const Key& k, const Value& v = Value(), Node* n = nullptr) 
            : data(k, v), next(n) {}
    };
    
    Node** buckets;
    std::size_t bucket_count;
    std::size_t _size;
    float max_load_factor;

    std::size_t hash(const Key& key) const {
        return std::hash<Key>{}(key) % bucket_count;
    }

    void rehash(std::size_t new_bucket_count) {
        Node** old_buckets = buckets;
        std::size_t old_count = bucket_count;
        
        bucket_count = new_bucket_count;
        buckets = new Node*[bucket_count]();
        _size = 0;
        
        for (std::size_t i = 0; i < old_count; ++i) {
            Node* cur = old_buckets[i];
            while (cur) {
                Node* next = cur->next;
                insert_node(cur);
                cur = next;
            }
            delete old_buckets[i];
        }
        delete[] old_buckets;
    }

    void insert_node(Node* node) {
        std::size_t idx = hash(node->data.first);
        node->next = buckets[idx];
        buckets[idx] = node;
        ++_size;
    }

    Node* find_node(const Key& key) const {
        std::size_t idx = hash(key);
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->data.first == key) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void check_rehash() {
        if ((float)_size / bucket_count > max_load_factor) {
            rehash(bucket_count * 2);
        }
    }

public:
    MyUnorderedMultimap() : bucket_count(8), _size(0), max_load_factor(0.75f) {
        buckets = new Node*[bucket_count]();
    }
    
    ~MyUnorderedMultimap() {
        clear();
        delete[] buckets;
    }

    // 迭代器
    class Iterator {
    private:
        Node* ptr;
        Node** buckets;
        std::size_t bucket_count;
        std::size_t current_bucket;
        
        void advance() {
            if (!ptr) return;
            ptr = ptr->next;
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
        Iterator(Node* p = nullptr, Node** b = nullptr, std::size_t bc = 0, std::size_t cb = 0)
            : ptr(p), buckets(b), bucket_count(bc), current_bucket(cb) {}
        
        std::pair<const Key, Value>& operator*() { return ptr->data; }
        std::pair<const Key, Value>* operator->() { return &ptr->data; }
        Iterator& operator++() { advance(); return *this; }
        Iterator operator++(int) { Iterator tmp = *this; advance(); return tmp; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() const {
        for (std::size_t i = 0; i < bucket_count; ++i) {
            if (buckets[i]) {
                return Iterator(buckets[i], buckets, bucket_count, i);
            }
        }
        return end();
    }
    
    Iterator end() const {
        return Iterator(nullptr, buckets, bucket_count, bucket_count);
    }

    // 容量
    bool empty() const { return _size == 0; }
    std::size_t size() const { return _size; }
    std::size_t bucket_count() const { return bucket_count; }

    // 插入 O(1) 平均 - 允许重复键
    void insert(const Key& key, const Value& value) {
        check_rehash();
        Node* new_node = new Node(key, value);
        std::size_t idx = hash(key);
        new_node->next = buckets[idx];
        buckets[idx] = new_node;
        ++_size;
    }

    // 查找 O(1) 平均
    bool find(const Key& key) const {
        return find_node(key) != nullptr;
    }

    // 计数 O(1) 平均
    std::size_t count(const Key& key) const {
        std::size_t cnt = 0;
        std::size_t idx = hash(key);
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->data.first == key) ++cnt;
            cur = cur->next;
        }
        return cnt;
    }

    // 删除一个元素 O(1) 平均
    bool erase_one(const Key& key) {
        std::size_t idx = hash(key);
        Node* cur = buckets[idx];
        Node* prev = nullptr;
        
        while (cur) {
            if (cur->data.first == key) {
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

    // 删除所有等于 key 的元素 O(k) 平均
    std::size_t erase_all(const Key& key) {
        std::size_t cnt = 0;
        std::size_t idx = hash(key);
        Node* cur = buckets[idx];
        Node* prev = nullptr;
        
        while (cur) {
            if (cur->data.first == key) {
                Node* to_delete = cur;
                cur = cur->next;
                if (prev) prev->next = cur;
                else buckets[idx] = cur;
                delete to_delete;
                --_size;
                ++cnt;
            } else {
                prev = cur;
                cur = cur->next;
            }
        }
        return cnt;
    }

    // 清空
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

    // 重新哈希
    void reserve(std::size_t count) {
        std::size_t new_bucket_count = count / max_load_factor + 1;
        if (new_bucket_count > bucket_count) {
            rehash(new_bucket_count);
        }
    }

    // 拷贝构造
    MyUnorderedMultimap(const MyUnorderedMultimap& other) 
        : bucket_count(other.bucket_count), _size(0), max_load_factor(other.max_load_factor) {
        buckets = new Node*[bucket_count]();
        for (std::size_t i = 0; i < bucket_count; ++i) {
            Node* cur = other.buckets[i];
            while (cur) {
                insert(cur->data.first, cur->data.second);
                cur = cur->next;
            }
        }
    }

    // 移动构造
    MyUnorderedMultimap(MyUnorderedMultimap&& other) noexcept
        : buckets(other.buckets), bucket_count(other.bucket_count), 
          _size(other._size), max_load_factor(other.max_load_factor) {
        other.buckets = nullptr;
        other.bucket_count = 0;
        other._size = 0;
    }

    // 拷贝赋值
    MyUnorderedMultimap& operator=(const MyUnorderedMultimap& other) {
        if (this != &other) {
            MyUnorderedMultimap tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
    MyUnorderedMultimap& operator=(MyUnorderedMultimap&& other) noexcept {
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

    void swap(MyUnorderedMultimap& other) {
        std::swap(buckets, other.buckets);
        std::swap(bucket_count, other.bucket_count);
        std::swap(_size, other._size);
        std::swap(max_load_factor, other.max_load_factor);
    }
};

// 测试代码
#include <iostream>
int main() {
    MyUnorderedMultimap<std::string, int> umm;
    
    umm.insert("apple", 5);
    umm.insert("banana", 3);
    umm.insert("apple", 8);   // 允许重复键
    umm.insert("orange", 7);
    umm.insert("banana", 2);  // 允许重复键
    umm.insert("apple", 1);   // 允许重复键
    umm.insert("grape", 4);
    
    std::cout << "Size: " << umm.size() << std::endl;  // 7
    std::cout << "Bucket count: " << umm.bucket_count() << std::endl;  // 8
    
    std::cout << "Find 'banana': " << umm.find("banana") << std::endl;  // 1
    std::cout << "Find 'peach': " << umm.find("peach") << std::endl;    // 0
    
    std::cout << "Count of apple: " << umm.count("apple") << std::endl;   // 3
    std::cout << "Count of banana: " << umm.count("banana") << std::endl; // 2
    std::cout << "Count of peach: " << umm.count("peach") << std::endl;   // 0
    
    std::cout << "Elements: ";
    for (auto it = umm.begin(); it != umm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // 顺序不定（哈希表无序）
    
    // 遍历所有 apple
    std::cout << "All apples: ";
    for (auto it = umm.begin(); it != umm.end(); ++it) {
        if (it->first == "apple") {
            std::cout << it->first << ":" << it->second << " ";
        }
    }
    std::cout << std::endl;
    
    umm.erase_one("apple");  // 删除一个 apple
    std::cout << "After erase one apple, size: " << umm.size() << std::endl;  // 6
    std::cout << "Count of apple: " << umm.count("apple") << std::endl;  // 2
    
    umm.erase_all("banana");  // 删除所有 banana
    std::cout << "After erase all banana, size: " << umm.size() << std::endl;  // 4
    std::cout << "Count of banana: " << umm.count("banana") << std::endl;  // 0
    
    std::cout << "Remaining elements: ";
    for (auto it = umm.begin(); it != umm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // apple:8 apple:1 orange:7 grape:4 或其它顺序
    
    umm.insert("x", 1);
    umm.insert("y", 2);
    umm.insert("z", 3);
    umm.insert("x", 4);  // 重复键
    
    std::cout << "After insert more, bucket count: " << umm.bucket_count() << std::endl;
    std::cout << "Elements: ";
    for (auto it = umm.begin(); it != umm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;
    
    umm.clear();
    std::cout << "After clear, empty: " << umm.empty() << std::endl;  // 1
    
    MyUnorderedMultimap<std::string, int> umm2;
    umm2.insert("a", 1);
    umm2.insert("b", 2);
    umm2.insert("a", 3);  // 重复
    
    umm = umm2;
    std::cout << "After assignment, size: " << umm.size() << std::endl;  // 3
    std::cout << "Elements: ";
    for (auto it = umm.begin(); it != umm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;
    
    return 0;
}