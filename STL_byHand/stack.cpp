template<typename T, typename Container = MyDeque<T>>
class MyStack {
private:
    Container c;  // 底层容器

public:
    // 容量
    bool empty() const { return c.empty(); }
    std::size_t size() const { return c.size(); }

    // 元素访问
    T& top() { return c.back(); }
    const T& top() const { return c.back(); }

    // 插入删除 O(1)
    void push(const T& val) { c.push_back(val); }
    void push(T&& val) { c.push_back(std::move(val)); }
    void pop() { c.pop_back(); }

    // 交换
    void swap(MyStack& other) { c.swap(other.c); }
};

// 测试代码
#include <iostream>
int main() {
    MyStack<int> s;
    
    s.push(1);
    s.push(2);
    s.push(3);
    
    std::cout << "Size: " << s.size() << std::endl;  // 3
    std::cout << "Top: " << s.top() << std::endl;    // 3
    
    s.pop();
    std::cout << "After pop top: " << s.top() << std::endl;  // 2
    
    while (!s.empty()) {
        std::cout << s.top() << " ";
        s.pop();
    }
    std::cout << std::endl;  // 2 1
    
    return 0;
}