template<typename T>
class MyMultiset {
private:
    enum Color { RED, BLACK };
    
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node* parent;
        Color color;
        Node(const T& val = T()) 
            : data(val), left(nullptr), right(nullptr), 
              parent(nullptr), color(RED) {}
    };
    
    Node* root;
    std::size_t _size;

    void left_rotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void right_rotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right) x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent) root = x;
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    void insert_fixup(Node* z) {
        while (z->parent && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        left_rotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    right_rotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        right_rotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    left_rotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    Node* minimum(Node* x) {
        while (x->left) x = x->left;
        return x;
    }

    Node* maximum(Node* x) {
        while (x->right) x = x->right;
        return x;
    }

    void delete_fixup(Node* x, Node* parent) {
        while (x != root && (!x || x->color == BLACK)) {
            if (x == parent->left) {
                Node* w = parent->right;
                if (w && w->color == RED) {
                    w->color = BLACK;
                    parent->color = RED;
                    left_rotate(parent);
                    w = parent->right;
                }
                if ((!w->left || w->left->color == BLACK) && 
                    (!w->right || w->right->color == BLACK)) {
                    if (w) w->color = RED;
                    x = parent;
                    parent = x->parent;
                } else {
                    if (!w->right || w->right->color == BLACK) {
                        if (w->left) w->left->color = BLACK;
                        if (w) w->color = RED;
                        if (w) right_rotate(w);
                        w = parent->right;
                    }
                    if (w) w->color = parent->color;
                    parent->color = BLACK;
                    if (w && w->right) w->right->color = BLACK;
                    left_rotate(parent);
                    x = root;
                    break;
                }
            } else {
                Node* w = parent->left;
                if (w && w->color == RED) {
                    w->color = BLACK;
                    parent->color = RED;
                    right_rotate(parent);
                    w = parent->left;
                }
                if ((!w->right || w->right->color == BLACK) && 
                    (!w->left || w->left->color == BLACK)) {
                    if (w) w->color = RED;
                    x = parent;
                    parent = x->parent;
                } else {
                    if (!w->left || w->left->color == BLACK) {
                        if (w->right) w->right->color = BLACK;
                        if (w) w->color = RED;
                        if (w) left_rotate(w);
                        w = parent->left;
                    }
                    if (w) w->color = parent->color;
                    parent->color = BLACK;
                    if (w && w->left) w->left->color = BLACK;
                    right_rotate(parent);
                    x = root;
                    break;
                }
            }
        }
        if (x) x->color = BLACK;
    }

    void transplant(Node* u, Node* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }

    void delete_node(Node* z) {
        Node* y = z;
        Node* x = nullptr;
        Node* parent = nullptr;
        Color y_original_color = y->color;
        
        if (!z->left) {
            x = z->right;
            parent = z->parent;
            transplant(z, z->right);
        } else if (!z->right) {
            x = z->left;
            parent = z->parent;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            parent = y;
            if (y->parent == z) {
                if (x) x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
                parent = y->parent;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        delete z;
        if (y_original_color == BLACK) {
            delete_fixup(x, parent);
        }
    }

    // 查找第一个等于 val 的节点
    Node* find_node(const T& val) const {
        Node* cur = root;
        while (cur) {
            if (val == cur->data) return cur;
            else if (val < cur->data) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

    // 查找第一个 >= val 的节点（lower_bound）
    Node* lower_bound_node(const T& val) const {
        Node* cur = root;
        Node* result = nullptr;
        while (cur) {
            if (cur->data >= val) {
                result = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return result;
    }

    // 查找第一个 > val 的节点（upper_bound）
    Node* upper_bound_node(const T& val) const {
        Node* cur = root;
        Node* result = nullptr;
        while (cur) {
            if (cur->data > val) {
                result = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return result;
    }

    void clear_tree(Node* node) {
        if (node) {
            clear_tree(node->left);
            clear_tree(node->right);
            delete node;
        }
    }

    Node* copy_tree(Node* node, Node* parent) {
        if (!node) return nullptr;
        Node* new_node = new Node(node->data);
        new_node->color = node->color;
        new_node->parent = parent;
        new_node->left = copy_tree(node->left, new_node);
        new_node->right = copy_tree(node->right, new_node);
        return new_node;
    }

public:
    MyMultiset() : root(nullptr), _size(0) {}
    
    ~MyMultiset() { clear_tree(root); }

    // 迭代器
    class Iterator {
    private:
        Node* ptr;
        Node* root;
        Node* find_min(Node* node) const {
            while (node && node->left) node = node->left;
            return node;
        }
        Node* successor(Node* node) const {
            if (node->right) return find_min(node->right);
            Node* parent = node->parent;
            while (parent && node == parent->right) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
        Node* predecessor(Node* node) const {
            if (node->left) {
                node = node->left;
                while (node->right) node = node->right;
                return node;
            }
            Node* parent = node->parent;
            while (parent && node == parent->left) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
    public:
        Iterator(Node* p = nullptr, Node* r = nullptr) : ptr(p), root(r) {}
        T& operator*() { return ptr->data; }
        Iterator& operator++() { 
            if (ptr) ptr = successor(ptr); 
            return *this; 
        }
        Iterator operator++(int) { 
            Iterator tmp = *this; 
            if (ptr) ptr = successor(ptr); 
            return tmp; 
        }
        Iterator& operator--() {
            if (ptr) ptr = predecessor(ptr);
            return *this;
        }
        Iterator operator--(int) {
            Iterator tmp = *this;
            if (ptr) ptr = predecessor(ptr);
            return tmp;
        }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() const { 
        Node* min = root;
        while (min && min->left) min = min->left;
        return Iterator(min, root); 
    }
    Iterator end() const { return Iterator(nullptr, root); }

    // 反向迭代器（简化）
    Iterator rbegin() const {
        Node* max = root;
        while (max && max->right) max = max->right;
        return Iterator(max, root);
    }
    Iterator rend() const { return Iterator(nullptr, root); }

    // 容量
    bool empty() const { return _size == 0; }
    std::size_t size() const { return _size; }

    // 插入 O(log n) - 允许重复
    void insert(const T& val) {
        Node* z = new Node(val);
        Node* y = nullptr;
        Node* x = root;
        
        while (x) {
            y = x;
            if (z->data < x->data) x = x->left;
            else x = x->right;  // 相等时往右走，允许重复
        }
        
        z->parent = y;
        if (!y) root = z;
        else if (z->data < y->data) y->left = z;
        else y->right = z;
        
        insert_fixup(z);
        ++_size;
    }

    // 查找 O(log n)
    bool find(const T& val) const {
        return find_node(val) != nullptr;
    }

    // 计数 O(log n)
    std::size_t count(const T& val) const {
        std::size_t cnt = 0;
        Node* cur = find_node(val);
        if (!cur) return 0;
        
        // 向左找
        Node* left = cur;
        while (left && left->data == val) {
            ++cnt;
            left = predecessor(left);
        }
        
        // 向右找（跳过已计数的当前节点）
        Node* right = cur->right;
        while (right && right->data == val) {
            ++cnt;
            right = right->right;
        }
        
        return cnt;
    }

    // lower_bound O(log n)
    Iterator lower_bound(const T& val) const {
        return Iterator(lower_bound_node(val), root);
    }

    // upper_bound O(log n)
    Iterator upper_bound(const T& val) const {
        return Iterator(upper_bound_node(val), root);
    }

    // 删除一个元素 O(log n)
    bool erase_one(const T& val) {
        Node* z = find_node(val);
        if (!z) return false;
        delete_node(z);
        --_size;
        return true;
    }

    // 删除所有等于 val 的元素 O(k log n)
    std::size_t erase_all(const T& val) {
        std::size_t cnt = 0;
        while (true) {
            Node* z = find_node(val);
            if (!z) break;
            delete_node(z);
            --_size;
            ++cnt;
        }
        return cnt;
    }

    // 清空
    void clear() {
        clear_tree(root);
        root = nullptr;
        _size = 0;
    }

    // 拷贝构造
    MyMultiset(const MyMultiset& other) : root(nullptr), _size(other._size) {
        root = copy_tree(other.root, nullptr);
    }

    // 移动构造
    MyMultiset(MyMultiset&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    // 拷贝赋值
    MyMultiset& operator=(const MyMultiset& other) {
        if (this != &other) {
            MyMultiset tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
    MyMultiset& operator=(MyMultiset&& other) noexcept {
        if (this != &other) {
            clear_tree(root);
            root = other.root;
            _size = other._size;
            other.root = nullptr;
            other._size = 0;
        }
        return *this;
    }

    void swap(MyMultiset& other) {
        std::swap(root, other.root);
        std::swap(_size, other._size);
    }
};

// 测试代码
#include <iostream>
int main() {
    MyMultiset<int> ms;
    
    ms.insert(5);
    ms.insert(3);
    ms.insert(7);
    ms.insert(3);  // 允许重复
    ms.insert(3);  // 允许重复
    ms.insert(9);
    ms.insert(5);  // 允许重复
    
    std::cout << "Size: " << ms.size() << std::endl;  // 7
    
    std::cout << "Elements (sorted): ";
    for (auto it = ms.begin(); it != ms.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 3 3 3 5 5 7 9
    
    std::cout << "Count of 3: " << ms.count(3) << std::endl;   // 3
    std::cout << "Count of 5: " << ms.count(5) << std::endl;   // 2
    std::cout << "Count of 8: " << ms.count(8) << std::endl;   // 0
    
    std::cout << "Find 5: " << ms.find(5) << std::endl;  // 1
    std::cout << "Find 6: " << ms.find(6) << std::endl;  // 0
    
    std::cout << "Lower bound 4: " << *ms.lower_bound(4) << std::endl;  // 5
    std::cout << "Upper bound 5: " << *ms.upper_bound(5) << std::endl;  // 7
    
    ms.erase_one(5);
    std::cout << "After erase one 5, size: " << ms.size() << std::endl;  // 6
    std::cout << "Elements: ";
    for (auto it = ms.begin(); it != ms.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 3 3 3 5 7 9
    
    ms.erase_all(3);
    std::cout << "After erase all 3, size: " << ms.size() << std::endl;  // 3
    std::cout << "Elements: ";
    for (auto it = ms.begin(); it != ms.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 5 7 9
    
    // 反向遍历
    std::cout << "Reverse: ";
    for (auto it = ms.rbegin(); it != ms.rend(); --it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 9 7 5
    
    return 0;
}