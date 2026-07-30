// HashTable.hpp
#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iomanip>

/**
 * @brief 哈希表模板（开放地址法 - 线性探测）
 * @tparam Key 键类型
 * @tparam Value 值类型
 * 
 * 特性：
 * - 使用除留余数法作为哈希函数
 * - 使用线性探测处理冲突
 * - 支持动态扩容（装载因子 > 0.5 时以2倍增长）
 * - 支持删除（标记删除）
 * 
 * 时间复杂度：
 * - 插入/查找/删除：平均 O(1)，最坏 O(n)
 * - 空间复杂度：O(n)
 */
template<typename Key, typename Value>
class HashTable {
private:
    // 记录状态枚举
    enum class EntryState { EMPTY, OCCUPIED, DELETED };
    
    // 哈希表条目
    struct Entry {
        Key key;
        Value value;
        EntryState state;
        
        Entry() : key(Key()), value(Value()), state(EntryState::EMPTY) {}
        Entry(const Key& k, const Value& v) 
            : key(k), value(v), state(EntryState::OCCUPIED) {}
        Entry(Key&& k, Value&& v) 
            : key(std::move(k)), value(std::move(v)), state(EntryState::OCCUPIED) {}
    };
    
    std::vector<Entry> _table;      // 哈希表
    std::size_t _size;              // 元素个数
    std::size_t _capacity;          // 表容量
    
    // 默认初始容量
    static constexpr std::size_t DEFAULT_CAPACITY = 11;
    
    /**
     * @brief 判断一个数是否为质数
     * @param n 要判断的数
     * @return bool 是否为质数
     */
    bool isPrime(std::size_t n) const {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        
        for (std::size_t i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    }
    
    /**
     * @brief 获取下一个质数（不小于 n）
     * @param n 起始值
     * @return std::size_t 下一个质数
     */
    std::size_t nextPrime(std::size_t n) const {
        if (n <= 2) return 2;
        if (n % 2 == 0) ++n;
        
        while (!isPrime(n)) {
            n += 2;
        }
        return n;
    }
    
    /**
     * @brief 哈希函数（除留余数法）
     * @param key 键
     * @return std::size_t 哈希值
     */
    std::size_t hash(const Key& key) const {
        std::hash<Key> hasher;
        return hasher(key) % _capacity;
    }
    
    /**
     * @brief 线性探测解决冲突
     * @param pos 当前位置（会被修改）
     */
    void probe(std::size_t& pos) const {
        pos = (pos + 1) % _capacity;
    }
    
    /**
     * @brief 查找键的位置
     * @param key 要查找的键
     * @param pos 输出参数：找到的位置
     * @return bool 是否找到
     */
    bool findPosition(const Key& key, std::size_t& pos) const {
        std::size_t start = hash(key);
        pos = start;
        std::size_t probeCount = 0;
        
        // 线性探测直到找到空位或回到起点
        while (_table[pos].state != EntryState::EMPTY && probeCount < _capacity) {
            if (_table[pos].state == EntryState::OCCUPIED && _table[pos].key == key) {
                return true;
            }
            probe(pos);
            ++probeCount;
        }
        return false;
    }
    
    /**
     * @brief 查找空闲位置
     * @param key 要插入的键
     * @param pos 输出参数：空闲位置
     * @return bool 是否找到空闲位置
     */
    bool findFreePosition(const Key& key, std::size_t& pos) const {
        std::size_t start = hash(key);
        pos = start;
        std::size_t probeCount = 0;
        std::size_t firstDeleted = _capacity;  // 记录第一个删除位置
        
        while (_table[pos].state != EntryState::EMPTY && probeCount < _capacity) {
            if (_table[pos].state == EntryState::DELETED && firstDeleted == _capacity) {
                firstDeleted = pos;
            }
            if (_table[pos].state == EntryState::OCCUPIED && _table[pos].key == key) {
                return false;  // 键已存在
            }
            probe(pos);
            ++probeCount;
        }
        
        // 优先使用删除位置
        if (firstDeleted != _capacity) {
            pos = firstDeleted;
        }
        return true;
    }
    
    /**
     * @brief 计算装载因子
     * @return double 装载因子
     */
    double loadFactor() const {
        return static_cast<double>(_size) / _capacity;
    }
    
    /**
     * @brief 重建哈希表（扩容）
     * @throws std::runtime_error 如果无法扩容
     */
    void rehash() {
        // 保存旧表
        std::vector<Entry> oldTable = std::move(_table);
        std::size_t oldSize = _size;
        
        // 计算新容量：2倍增长，且为质数
        std::size_t newCapacity = nextPrime(_capacity * 2);
        if (newCapacity <= _capacity) {
            throw std::runtime_error("Hash table capacity overflow");
        }
        
        // 更新容量
        _capacity = newCapacity;
        
        // 初始化新表
        _table.clear();
        _table.resize(_capacity);
        _size = 0;
        
        // 重新插入所有元素
        for (const auto& entry : oldTable) {
            if (entry.state == EntryState::OCCUPIED) {
                insert(entry.key, entry.value);
            }
        }
        
        // 确保所有元素都插入了
        if (_size != oldSize) {
            throw std::runtime_error("Rehash failed: element count mismatch");
        }
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建指定初始容量的哈希表
     * @param initialCapacity 初始容量（默认使用 11）
     */
    HashTable(std::size_t initialCapacity = DEFAULT_CAPACITY) 
        : _size(0) {
        if (initialCapacity < 1) {
            initialCapacity = DEFAULT_CAPACITY;
        }
        _capacity = nextPrime(initialCapacity);
        _table.resize(_capacity);
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    HashTable(std::initializer_list<std::pair<Key, Value>> ilist) 
        : HashTable(ilist.size() * 2) {
        for (const auto& pair : ilist) {
            insert(pair.first, pair.second);
        }
    }
    
    // ---- 拷贝控制 ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     */
    HashTable(const HashTable& other) = default;
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     */
    HashTable(HashTable&& other) noexcept = default;
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return HashTable&
     */
    HashTable& operator=(const HashTable& other) = default;
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return HashTable&
     */
    HashTable& operator=(HashTable&& other) noexcept = default;
    
    // ---- 容量 ----
    
    /**
     * @brief 返回元素个数
     * @return std::size_t
     */
    std::size_t size() const { return _size; }
    
    /**
     * @brief 返回表容量
     * @return std::size_t
     */
    std::size_t capacity() const { return _capacity; }
    
    /**
     * @brief 判断是否为空
     * @return bool
     */
    bool empty() const { return _size == 0; }
    
    /**
     * @brief 获取当前装载因子
     * @return double
     */
    double loadFactor() const { return loadFactor(); }
    
    // ---- 查找 ----
    
    /**
     * @brief 查找键
     * @param key 要查找的键
     * @return bool 是否存在
     * 时间复杂度：平均 O(1)，最坏 O(n)
     */
    bool contains(const Key& key) const {
        std::size_t pos;
        return findPosition(key, pos);
    }
    
    /**
     * @brief 查找键并返回值
     * @param key 要查找的键
     * @return const Value& 值的引用
     * @throws std::out_of_range 如果键不存在
     * 时间复杂度：平均 O(1)，最坏 O(n)
     */
    const Value& at(const Key& key) const {
        std::size_t pos;
        if (!findPosition(key, pos)) {
            throw std::out_of_range("Key not found");
        }
        return _table[pos].value;
    }
    
    /**
     * @brief 查找键并返回值（非常量版本）
     * @param key 要查找的键
     * @return Value& 值的引用
     * @throws std::out_of_range 如果键不存在
     * 时间复杂度：平均 O(1)，最坏 O(n)
     */
    Value& at(const Key& key) {
        std::size_t pos;
        if (!findPosition(key, pos)) {
            throw std::out_of_range("Key not found");
        }
        return _table[pos].value;
    }
    
    /**
     * @brief 下标访问（如果键不存在则插入默认值）
     * @param key 键
     * @return Value& 值的引用
     * 时间复杂度：平均 O(1)，最坏 O(n)
     */
    Value& operator[](const Key& key) {
        std::size_t pos;
        if (!findPosition(key, pos)) {
            // 插入默认值
            insert(key, Value());
            // 重新查找
            findPosition(key, pos);
        }
        return _table[pos].value;
    }
    
    // ---- 插入 ----
    
    /**
     * @brief 插入键值对（拷贝版本）
     * @param key 键
     * @param value 值
     * @return bool 是否成功插入（false表示键已存在）
     * 时间复杂度：平均 O(1)，最坏 O(n)
     */
    bool insert(const Key& key, const Value& value) {
        // 检查是否需要扩容（装载因子 > 0.5）
        if (loadFactor() > 0.5) {
            rehash();
        }
        
        std::size_t pos;
        if (!findFreePosition(key, pos)) {
            return false;  // 键已存在
        }
        
        _table[pos] = Entry(key, value);
        ++_size;
        return true;
    }
    
    /**
     * @brief 插入键值对（移动版本）
     * @param key 键（右值引用）
     * @param value 值（右值引用）
     * @return bool 是否成功插入
     * 时间复杂度：平均 O(1)，最坏 O(n)
     */
    bool insert(Key&& key, Value&& value) {
        if (loadFactor() > 0.5) {
            rehash();
        }
        
        std::size_t pos;
        if (!findFreePosition(key, pos)) {
            return false;
        }
        
        _table[pos] = Entry(std::move(key), std::move(value));
        ++_size;
        return true;
    }
    
    /**
     * @brief 插入或更新键值对
     * @param key 键
     * @param value 值
     * 时间复杂度：平均 O(1)，最坏 O(n)
     */
    void insertOrAssign(const Key& key, const Value& value) {
        std::size_t pos;
        if (findPosition(key, pos)) {
            _table[pos].value = value;
        } else {
            insert(key, value);
        }
    }
    
    // ---- 删除 ----
    
    /**
     * @brief 删除键
     * @param key 要删除的键
     * @return bool 是否成功删除
     * 时间复杂度：平均 O(1)，最坏 O(n)
     */
    bool erase(const Key& key) {
        std::size_t pos;
        if (!findPosition(key, pos)) {
            return false;
        }
        
        _table[pos].state = EntryState::DELETED;
        --_size;
        return true;
    }
    
    /**
     * @brief 清空哈希表
     * 时间复杂度：O(n)
     */
    void clear() {
        for (auto& entry : _table) {
            entry.state = EntryState::EMPTY;
        }
        _size = 0;
    }
    
    // ---- 遍历 ----
    
    /**
     * @brief 遍历所有元素
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void forEach(std::function<void(const Key&, const Value&)> callback) const {
        for (const auto& entry : _table) {
            if (entry.state == EntryState::OCCUPIED) {
                callback(entry.key, entry.value);
            }
        }
    }
    
    /**
     * @brief 交换两个哈希表
     * @param other 要交换的对象
     */
    void swap(HashTable& other) {
        std::swap(_table, other._table);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename Key, typename Value>
void swap(HashTable<Key, Value>& a, HashTable<Key, Value>& b) {
    a.swap(b);
}

#endif // HASH_TABLE_HPP