template<typename T>
class MyForwardList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val = T(), Node* n = nullptr) : data(val), next(n) {}
        Node(T&& val, Node* n = nullptr) : data(std::move(val)), next(n) {}
    };
    
    Node* head;  // 哨兵节点

public:
    MyForwardList() : head(new Node()) {}
    
    ~MyForwardList() {
        clear();
        delete head;
    }

    // 迭代器（简单实现）
    class Iterator {
    private:
        Node* ptr;
    public:
        Iterator(Node* p = nullptr) : ptr(p) {}
        T& operator*() { return ptr->data; }
        Iterator& operator++() { ptr = ptr->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ptr = ptr->next; return tmp; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        Node* get_node() const { return ptr; }
    };

    Iterator begin() { return Iterator(head->next); }
    Iterator end() { return Iterator(nullptr); }

    // 容量
    bool empty() const { return head->next == nullptr; }
    
    std::size_t size() const {
        std::size_t count = 0;
        Node* cur = head->next;
        while (cur) { ++count; cur = cur->next; }
        return count;
    }

    // 访问
    T& front() { return head->next->data; }
    const T& front() const { return head->next->data; }

    // 插入 O(1)
    void push_front(const T& val) {
        head->next = new Node(val, head->next);
    }
    
    void push_front(T&& val) {
        head->next = new Node(std::move(val), head->next);
    }

    // 在指定位置后插入 O(1)
    void insert_after(Iterator pos, const T& val) {
        Node* p = pos.get_node();
        p->next = new Node(val, p->next);
    }

    // 删除 O(1)
    void pop_front() {
        if (head->next) {
            Node* tmp = head->next;
            head->next = tmp->next;
            delete tmp;
        }
    }

    void erase_after(Iterator pos) {
        Node* p = pos.get_node();
        if (p->next) {
            Node* tmp = p->next;
            p->next = tmp->next;
            delete tmp;
        }
    }

    // 清空
    void clear() {
        Node* cur = head->next;
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        head->next = nullptr;
    }

    // 查找（O(n)）
    Iterator find(const T& val) {
        Node* cur = head->next;
        while (cur) {
            if (cur->data == val) return Iterator(cur);
            cur = cur->next;
        }
        return end();
    }

    // 拷贝构造
    MyForwardList(const MyForwardList& other) : head(new Node()) {
        Node* cur = other.head->next;
        Node* tail = head;
        while (cur) {
            tail->next = new Node(cur->data);
            tail = tail->next;
            cur = cur->next;
        }
    }

    // 移动构造
    MyForwardList(MyForwardList&& other) noexcept : head(other.head) {
        other.head = nullptr;
    }

    // 拷贝赋值
    MyForwardList& operator=(const MyForwardList& other) {
        if (this != &other) {
            MyForwardList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
    MyForwardList& operator=(MyForwardList&& other) noexcept {
        if (this != &other) {
            clear();
            delete head;
            head = other.head;
            other.head = nullptr;
        }
        return *this;
    }

    void swap(MyForwardList& other) {
        std::swap(head, other.head);
    }

    // 反转链表 O(n)
    void reverse() {
        Node* prev = nullptr;
        Node* cur = head->next;
        while (cur) {
            Node* next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        head->next = prev;
    }
};

// ========== 测试代码 ==========
#include <iostream>
int main() {
    MyForwardList<int> fl;
    
    fl.push_front(3);
    fl.push_front(2);
    fl.push_front(1);
    
    std::cout << "Front: " << fl.front() << std::endl;  // 1
    std::cout << "Size: " << fl.size() << std::endl;    // 3
    
    std::cout << "Elements: ";
    for (auto it = fl.begin(); it != fl.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 1 2 3
    
    fl.pop_front();
    std::cout << "After pop: " << fl.front() << std::endl;  // 2
    
    auto it = fl.find(3);
    if (it != fl.end()) {
        fl.insert_after(it, 4);
    }
    
    std::cout << "After insert after 3: ";
    for (auto it = fl.begin(); it != fl.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 2 3 4
    
    fl.reverse();
    std::cout << "After reverse: ";
    for (auto it = fl.begin(); it != fl.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 4 3 2
    
    return 0;
}