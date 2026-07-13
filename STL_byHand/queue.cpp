template<typename T, typename Container = MyDeque<T>>
class MyQueue {
private:
    Container c;  // 底层容器

public:
    // 容量
    bool empty() const { return c.empty(); }
    std::size_t size() const { return c.size(); }

    // 元素访问
    T& front() { return c.front(); }
    const T& front() const { return c.front(); }
    T& back() { return c.back(); }
    const T& back() const { return c.back(); }

    // 插入删除 O(1)
    void push(const T& val) { c.push_back(val); }
    void push(T&& val) { c.push_back(std::move(val)); }
    void pop() { c.pop_front(); }

    // 交换
    void swap(MyQueue& other) { c.swap(other.c); }
};

// 测试代码
#include <iostream>
int main() {
    MyQueue<int> q;
    
    q.push(1);
    q.push(2);
    q.push(3);
    
    std::cout << "Size: " << q.size() << std::endl;    // 3
    std::cout << "Front: " << q.front() << std::endl;  // 1
    std::cout << "Back: " << q.back() << std::endl;    // 3
    
    q.pop();
    std::cout << "After pop front: " << q.front() << std::endl;  // 2
    
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;  // 2 3
    
    return 0;
}