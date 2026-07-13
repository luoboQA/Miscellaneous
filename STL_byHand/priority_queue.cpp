template<typename T, typename Container = MyVector<T>>
class MyPriorityQueue {
private:
    Container c;  // 底层容器

    // 堆算法（最大堆）
    void heapify_up(std::size_t idx) {
        while (idx > 0) {
            std::size_t parent = (idx - 1) / 2;
            if (c[parent] < c[idx]) {  // 子节点大于父节点，上浮
                std::swap(c[parent], c[idx]);
                idx = parent;
            } else break;
        }
    }

    void heapify_down(std::size_t idx) {
        std::size_t size = c.size();
        while (true) {
            std::size_t largest = idx;
            std::size_t left = 2 * idx + 1;
            std::size_t right = 2 * idx + 2;
            
            if (left < size && c[largest] < c[left]) largest = left;
            if (right < size && c[largest] < c[right]) largest = right;
            
            if (largest != idx) {
                std::swap(c[idx], c[largest]);
                idx = largest;
            } else break;
        }
    }

public:
    // 容量
    bool empty() const { return c.empty(); }
    std::size_t size() const { return c.size(); }

    // 元素访问 O(1)
    const T& top() const { return c.front(); }

    // 插入 O(log n)
    void push(const T& val) {
        c.push_back(val);
        heapify_up(c.size() - 1);
    }
    
    void push(T&& val) {
        c.push_back(std::move(val));
        heapify_up(c.size() - 1);
    }

    // 删除 O(log n)
    void pop() {
        if (c.empty()) return;
        std::swap(c.front(), c.back());
        c.pop_back();
        if (!c.empty()) heapify_down(0);
    }

    // 交换
    void swap(MyPriorityQueue& other) { c.swap(other.c); }
};

// 测试代码
#include <iostream>
int main() {
    MyPriorityQueue<int> pq;
    
    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(1);
    pq.push(5);
    pq.push(9);
    pq.push(2);
    
    std::cout << "Size: " << pq.size() << std::endl;  // 7
    std::cout << "Top: " << pq.top() << std::endl;    // 9
    
    std::cout << "Priority order: ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << std::endl;  // 9 5 4 3 2 1 1
    
    return 0;
}