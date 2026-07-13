template<typename Key>
class MyUnorderedSet {
private:
    struct Node {
        Key data;
        Node* next;
        Node(const Key& k = Key(), Node* n = nullptr) : data(k), next(n) {}
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
        std::size_t idx = hash(node->data);
        node->next = buckets[idx];
        buckets[idx] = node;
        ++_size;
    }

    Node* find_node(const Key& key) const {
        std::size_t idx = hash(key);
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->data == key) return cur;
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
    MyUnorderedSet() : bucket_count(8), _size(0), max_load_factor(0.75f) {
        buckets = new Node*[bucket_count]();
    }
    
    ~MyUnorderedSet() {
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
        
        Key& operator*() { return ptr->data; }
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

    // 插入 O(1) 平均
    bool insert(const Key& key) {
        if (find_node(key)) return false;
        check_rehash();
        Node* new_node = new Node(key);
        std::size_t idx = hash(key);
        new_node->next = buckets[idx];
        buckets[idx] = new_node;
        ++_size;
        return true;
    }

    // 查找 O(1) 平均
    bool find(const Key& key) const {
        return find_node(key) != nullptr;
    }

    // 删除 O(1) 平均
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

    // 移动构造
    MyUnorderedSet(MyUnorderedSet&& other) noexcept
        : buckets(other.buckets), bucket_count(other.bucket_count), 
          _size(other._size), max_load_factor(other.max_load_factor) {
        other.buckets = nullptr;
        other.bucket_count = 0;
        other._size = 0;
    }

    // 拷贝赋值
    MyUnorderedSet& operator=(const MyUnorderedSet& other) {
        if (this != &other) {
            MyUnorderedSet tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
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

    void swap(MyUnorderedSet& other) {
        std::swap(buckets, other.buckets);
        std::swap(bucket_count, other.bucket_count);
        std::swap(_size, other._size);
        std::swap(max_load_factor, other.max_load_factor);
    }
};

// 测试代码
#include <iostream>
int main() {
    MyUnorderedSet<int> us;
    
    us.insert(5);
    us.insert(3);
    us.insert(8);
    us.insert(1);
    us.insert(9);
    us.insert(3);  // 重复插入，忽略
    
    std::cout << "Size: " << us.size() << std::endl;  // 5
    std::cout << "Bucket count: " << us.bucket_count() << std::endl;  // 8
    
    std::cout << "Find 5: " << us.find(5) << std::endl;  // 1
    std::cout << "Find 6: " << us.find(6) << std::endl;  // 0
    
    std::cout << "Elements: ";
    for (auto it = us.begin(); it != us.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 顺序不定（哈希表无序）
    
    us.erase(5);
    std::cout << "After erase 5, size: " << us.size() << std::endl;  // 4
    
    us.insert(10);
    us.insert(12);
    us.insert(14);  // 可能触发 rehash
    
    std::cout << "After insert more, bucket count: " << us.bucket_count() << std::endl;
    std::cout << "Elements: ";
    for (auto it = us.begin(); it != us.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    us.clear();
    std::cout << "After clear, empty: " << us.empty() << std::endl;  // 1
    
    MyUnorderedSet<int> us2;
    us2.insert(100);
    us2.insert(200);
    us2.insert(300);
    
    us = us2;
    std::cout << "After assignment: ";
    for (auto it = us.begin(); it != us.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    return 0;
}