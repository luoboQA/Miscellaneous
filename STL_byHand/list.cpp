template<typename T>
class MyList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& val = T(), Node* p = nullptr, Node* n = nullptr) 
            : data(val), prev(p), next(n) {}
        Node(T&& val, Node* p = nullptr, Node* n = nullptr) 
            : data(std::move(val)), prev(p), next(n) {}
    };
    
    Node* head;  // 哨兵节点（不存储数据）

public:
    MyList() : head(new Node()) {
        head->prev = head;
        head->next = head;
    }
    
    ~MyList() {
        clear();
        delete head;
    }

    // 迭代器
    class Iterator {
    private:
        Node* ptr;
    public:
        Iterator(Node* p = nullptr) : ptr(p) {}
        T& operator*() { return ptr->data; }
        Iterator& operator++() { ptr = ptr->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ptr = ptr->next; return tmp; }
        Iterator& operator--() { ptr = ptr->prev; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; ptr = ptr->prev; return tmp; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        Node* get_node() const { return ptr; }
    };

    Iterator begin() { return Iterator(head->next); }
    Iterator end() { return Iterator(head); }

    // 容量
    bool empty() const { return head->next == head; }
    
    std::size_t size() const {
        std::size_t count = 0;
        Node* cur = head->next;
        while (cur != head) { ++count; cur = cur->next; }
        return count;
    }

    // 访问
    T& front() { return head->next->data; }
    const T& front() const { return head->next->data; }
    T& back() { return head->prev->data; }
    const T& back() const { return head->prev->data; }

    // 插入 O(1)
    void push_front(const T& val) { insert(begin(), val); }
    void push_front(T&& val) { insert(begin(), std::move(val)); }
    void push_back(const T& val) { insert(end(), val); }
    void push_back(T&& val) { insert(end(), std::move(val)); }

    void insert(Iterator pos, const T& val) {
        Node* p = pos.get_node();
        Node* new_node = new Node(val, p->prev, p);
        p->prev->next = new_node;
        p->prev = new_node;
    }

    void insert(Iterator pos, T&& val) {
        Node* p = pos.get_node();
        Node* new_node = new Node(std::move(val), p->prev, p);
        p->prev->next = new_node;
        p->prev = new_node;
    }

    // 删除 O(1)
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }

    void erase(Iterator pos) {
        Node* p = pos.get_node();
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
    }

    // 清空
    void clear() {
        Node* cur = head->next;
        while (cur != head) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        head->prev = head;
        head->next = head;
    }

    // 查找 O(n)
    Iterator find(const T& val) {
        Node* cur = head->next;
        while (cur != head) {
            if (cur->data == val) return Iterator(cur);
            cur = cur->next;
        }
        return end();
    }

    // 拷贝构造
    MyList(const MyList& other) : head(new Node()) {
        head->prev = head;
        head->next = head;
        Node* cur = other.head->next;
        while (cur != other.head) {
            push_back(cur->data);
            cur = cur->next;
        }
    }

    // 移动构造
    MyList(MyList&& other) noexcept : head(other.head) {
        other.head = nullptr;
    }

    // 拷贝赋值
    MyList& operator=(const MyList& other) {
        if (this != &other) {
            MyList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
    MyList& operator=(MyList&& other) noexcept {
        if (this != &other) {
            clear();
            delete head;
            head = other.head;
            other.head = nullptr;
        }
        return *this;
    }

    void swap(MyList& other) {
        std::swap(head, other.head);
    }

    // 合并两个有序链表
    void merge(MyList& other) {
        if (this == &other) return;
        Iterator it1 = begin();
        Iterator it2 = other.begin();
        while (it1 != end() && it2 != other.end()) {
            if (*it2 < *it1) {
                Node* p = it2.get_node();
                ++it2;
                p->prev->next = p->next;
                p->next->prev = p->prev;
                
                Node* pos = it1.get_node();
                p->prev = pos->prev;
                p->next = pos;
                pos->prev->next = p;
                pos->prev = p;
            } else {
                ++it1;
            }
        }
        while (it2 != other.end()) {
            Node* p = it2.get_node();
            ++it2;
            p->prev->next = p->next;
            p->next->prev = p->prev;
            
            p->prev = head->prev;
            p->next = head;
            head->prev->next = p;
            head->prev = p;
        }
        other.head->next = other.head;
        other.head->prev = other.head;
    }

    // 反转 O(n)
    void reverse() {
        Node* cur = head->next;
        while (cur != head) {
            Node* next = cur->next;
            std::swap(cur->prev, cur->next);
            cur = next;
        }
        std::swap(head->prev, head->next);
    }

    // 排序 O(n log n) 简单实现
    void sort() {
        if (size() <= 1) return;
        MyList left, right;
        Node* cur = head->next;
        std::size_t mid = size() / 2;
        for (std::size_t i = 0; i < mid; ++i) {
            left.push_back(cur->data);
            cur = cur->next;
        }
        while (cur != head) {
            right.push_back(cur->data);
            cur = cur->next;
        }
        left.sort();
        right.sort();
        clear();
        left.merge(right);
        Iterator it = left.begin();
        while (it != left.end()) {
            push_back(*it);
            ++it;
        }
    }
};

// ========== 测试代码 ==========
#include <iostream>
int main() {
    MyList<int> list;
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_front(0);
    list.push_front(-1);
    
    std::cout << "Size: " << list.size() << std::endl;  // 5
    std::cout << "Front: " << list.front() << std::endl;  // -1
    std::cout << "Back: " << list.back() << std::endl;    // 3
    
    std::cout << "Elements: ";
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // -1 0 1 2 3
    
    list.pop_front();
    list.pop_back();
    std::cout << "After pop: " << list.front() << ", " << list.back() << std::endl;  // 0, 2
    
    auto it = list.find(1);
    if (it != list.end()) {
        list.insert(it, 99);
    }
    
    std::cout << "After insert: ";
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 0 99 1 2
    
    list.reverse();
    std::cout << "After reverse: ";
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 2 1 99 0
    
    MyList<int> list2;
    list2.push_back(3);
    list2.push_back(4);
    list2.push_back(5);
    
    list.merge(list2);
    std::cout << "After merge: ";
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    return 0;
}