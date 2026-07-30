// Heap.hpp
#ifndef HEAP_HPP
#define HEAP_HPP

#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

/**
 * @brief 堆模板（默认最大堆）
 * @tparam T 元素类型
 * @tparam Compare 比较函数（默认为 std::less，即最大堆）
 * 
 * 堆性质：
 * - 最大堆：每个父节点 ≥ 其子节点
 * - 最小堆：每个父节点 ≤ 其子节点
 * 
 * 规律：
 * - 节点 i 的左孩子：2*i
 * - 节点 i 的右孩子：2*i+1
 * - 节点 i 的父节点：i/2
 * - 最后一个非叶子节点：size/2
 * 
 * 时间复杂度：
 * - 插入：O(logn)
 * - 删除堆顶：O(logn)
 * - 获取堆顶：O(1)
 * - 堆排序：O(nlogn)
 * - 构建堆：O(n)
 * - 空间复杂度：O(n)
 */
template<typename T, typename Compare = std::less<T>>
class Heap {
private:
    std::vector<T> _heap;      // 从下标1开始存储
    std::size_t _size;         // 元素个数
    Compare _comp;             // 比较函数
    
    /**
     * @brief 向上调整（插入时使用）
     * @param i 要调整的节点位置
     */
    void shiftUp(std::size_t i) {
        while (i > 1 && _comp(_heap[i / 2], _heap[i])) {
            std::swap(_heap[i], _heap[i / 2]);
            i /= 2;
        }
    }
    
    /**
     * @brief 向下调整（删除时使用）
     * @param i 要调整的节点位置
     */
    void shiftDown(std::size_t i) {
        while (i * 2 <= _size) {
            std::size_t target = i * 2;
            if (target + 1 <= _size && _comp(_heap[target], _heap[target + 1])) {
                target = target + 1;
            }
            if (!_comp(_heap[i], _heap[target])) break;
            std::swap(_heap[i], _heap[target]);
            i = target;
        }
    }
    
    /**
     * @brief 校验堆性质（调试用）
     * @return bool 是否满足堆性质
     */
    bool validate() const {
        for (std::size_t i = 2; i <= _size; ++i) {
            if (_comp(_heap[i / 2], _heap[i])) {
                return false;
            }
        }
        return true;
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * @param capacity 初始容量
     */
    explicit Heap(std::size_t capacity = 100) 
        : _size(0) {
        _heap.reserve(capacity + 1);
        _heap.resize(1);  // 下标0占位
    }
    
    /**
     * @brief 从迭代器范围构造
     * @param first 起始迭代器
     * @param last 结束迭代器
     */
    template<typename InputIt>
    Heap(InputIt first, InputIt last, std::size_t capacity = 100) 
        : Heap(capacity) {
        for (auto it = first; it != last; ++it) {
            push(*it);
        }
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    Heap(std::initializer_list<T> ilist) 
        : Heap(ilist.size() * 2) {
        for (const T& val : ilist) {
            push(val);
        }
    }
    
    /**
     * @brief 从数组构建堆（批量构建，O(n)）
     * @param arr 数组
     * @param n 数组大小
     */
    Heap(const T* arr, std::size_t n) 
        : Heap(n * 2) {
        _size = n;
        _heap.resize(n + 1);
        for (std::size_t i = 0; i < n; ++i) {
            _heap[i + 1] = arr[i];
        }
        // 从最后一个非叶子节点开始向下调整
        for (std::size_t i = _size / 2; i >= 1; --i) {
            shiftDown(i);
        }
    }
    
    // ---- 拷贝控制 ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     */
    Heap(const Heap& other) = default;
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     */
    Heap(Heap&& other) noexcept = default;
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return Heap&
     */
    Heap& operator=(const Heap& other) = default;
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return Heap&
     */
    Heap& operator=(Heap&& other) noexcept = default;
    
    /**
     * @brief 交换两个堆
     * @param other 要交换的对象
     */
    void swap(Heap& other) {
        std::swap(_heap, other._heap);
        std::swap(_size, other._size);
    }
    
    // ---- 容量 ----
    
    /**
     * @brief 返回元素个数
     * @return std::size_t
     */
    std::size_t size() const { return _size; }
    
    /**
     * @brief 判断是否为空
     * @return bool
     */
    bool empty() const { return _size == 0; }
    
    /**
     * @brief 返回容量
     * @return std::size_t
     */
    std::size_t capacity() const { return _heap.capacity(); }
    
    // ---- 访问 ----
    
    /**
     * @brief 获取堆顶元素（常量版本）
     * @return const T& 堆顶元素引用
     * @throws std::out_of_range 如果堆为空
     * 时间复杂度：O(1)
     */
    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return _heap[1];
    }
    
    /**
     * @brief 获取堆顶元素（非常量版本）
     * @return T& 堆顶元素引用
     * @throws std::out_of_range 如果堆为空
     * 时间复杂度：O(1)
     */
    T& top() {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return _heap[1];
    }
    
    // ---- 插入 ----
    
    /**
     * @brief 插入元素（拷贝版本）
     * @param val 要插入的值
     * 时间复杂度：O(logn)
     */
    void push(const T& val) {
        if (_size + 1 >= _heap.size()) {
            _heap.reserve(_heap.size() * 2);
            _heap.resize(_heap.capacity());
        }
        _heap[++_size] = val;
        shiftUp(_size);
    }
    
    /**
     * @brief 插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * 时间复杂度：O(logn)
     */
    void push(T&& val) {
        if (_size + 1 >= _heap.size()) {
            _heap.reserve(_heap.size() * 2);
            _heap.resize(_heap.capacity());
        }
        _heap[++_size] = std::move(val);
        shiftUp(_size);
    }
    
    // ---- 删除 ----
    
    /**
     * @brief 删除堆顶元素
     * @return T 被删除的元素
     * @throws std::out_of_range 如果堆为空
     * 时间复杂度：O(logn)
     */
    T pop() {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        T maxVal = std::move(_heap[1]);
        _heap[1] = std::move(_heap[_size--]);
        shiftDown(1);
        return maxVal;
    }
    
    /**
     * @brief 清空堆
     * 时间复杂度：O(n)
     */
    void clear() {
        _heap.clear();
        _heap.resize(1);
        _size = 0;
    }
    
    // ---- 构建 ----
    
    /**
     * @brief 从数组构建堆（覆盖原有内容）
     * @param arr 数组
     * @param n 数组大小
     * 时间复杂度：O(n)
     */
    void build(const T* arr, std::size_t n) {
        _heap.resize(n + 1);
        _size = n;
        for (std::size_t i = 0; i < n; ++i) {
            _heap[i + 1] = arr[i];
        }
        for (std::size_t i = _size / 2; i >= 1; --i) {
            shiftDown(i);
        }
    }
    
    /**
     * @brief 从vector构建堆（覆盖原有内容）
     * @param vec vector
     * 时间复杂度：O(n)
     */
    void build(const std::vector<T>& vec) {
        build(vec.data(), vec.size());
    }
    
    // ---- 排序 ----
    
    /**
     * @brief 堆排序（返回排序后的数组）
     * @return std::vector<T> 排序后的数组（升序）
     * 时间复杂度：O(nlogn)
     */
    std::vector<T> sort() {
        std::vector<T> result;
        result.reserve(_size);
        Heap<T, Compare> temp = *this;
        while (!temp.empty()) {
            result.push_back(temp.pop());
        }
        // 对于最大堆，结果是降序；对于最小堆，结果是升序
        // 但这里统一返回降序（最大堆）或升序（最小堆）
        return result;
    }
    
    /**
     * @brief 堆排序并打印
     * @param os 输出流
     * 时间复杂度：O(nlogn)
     */
    void printSorted(std::ostream& os = std::cout) const {
        Heap<T, Compare> temp = *this;
        os << "堆排序结果：";
        while (!temp.empty()) {
            os << temp.pop() << " ";
        }
        os << "\n";
    }
    
    // ---- 遍历 ----
    
    /**
     * @brief 打印堆
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void print(std::ostream& os = std::cout) const {
        for (std::size_t i = 1; i <= _size; ++i) {
            os << _heap[i] << " ";
        }
        os << "\n";
    }
    
    /**
     * @brief 层序遍历（按层打印）
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void printLevelOrder(std::ostream& os = std::cout) const {
        if (empty()) {
            os << "Heap is empty\n";
            return;
        }
        
        std::size_t level = 0;
        std::size_t count = 0;
        std::size_t levelSize = 1;
        
        os << "层次遍历：\n";
        for (std::size_t i = 1; i <= _size; ++i) {
            os << _heap[i] << " ";
            ++count;
            if (count == levelSize) {
                os << "\n";
                levelSize *= 2;
                count = 0;
                ++level;
            }
        }
        os << "\n";
    }
    
    /**
     * @brief 遍历所有元素（不保证顺序）
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void forEach(std::function<void(const T&)> callback) const {
        for (std::size_t i = 1; i <= _size; ++i) {
            callback(_heap[i]);
        }
    }
    
    // ---- 工具 ----
    
    /**
     * @brief 验证堆性质
     * @return bool 是否满足堆性质
     */
    bool isValid() const {
        return validate();
    }
    
    /**
     * @brief 获取堆类型名称
     * @return std::string 类型名称
     */
    std::string type() const {
        return std::is_same<Compare, std::less<T>>::value ? "最大堆" : "最小堆";
    }
};

// ---- 类型别名 ----

/**
 * @brief 最大堆（默认）
 */
template<typename T>
using MaxHeap = Heap<T, std::less<T>>;

/**
 * @brief 最小堆
 */
template<typename T>
using MinHeap = Heap<T, std::greater<T>>;

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T, typename Compare>
void swap(Heap<T, Compare>& a, Heap<T, Compare>& b) {
    a.swap(b);
}

#endif // HEAP_HPP