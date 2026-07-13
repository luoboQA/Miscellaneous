template<typename T>
class MySet {
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

    // 左旋
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

    // 右旋
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

    // 插入修复
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

    // 查找最小节点
    Node* minimum(Node* x) {
        while (x->left) x = x->left;
        return x;
    }

    // 删除修复
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

    // 删除节点
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

    // 替换子树
    void transplant(Node* u, Node* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }

    // 查找节点
    Node* find_node(const T& val) const {
        Node* cur = root;
        while (cur) {
            if (val == cur->data) return cur;
            else if (val < cur->data) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

    // 清空子树
    void clear_tree(Node* node) {
        if (node) {
            clear_tree(node->left);
            clear_tree(node->right);
            delete node;
        }
    }

    // 拷贝子树
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
    MySet() : root(nullptr), _size(0) {}
    
    ~MySet() { clear_tree(root); }

    // 迭代器
    class Iterator {
    private:
        Node* ptr;
        Node* root;
        Node* find_min(Node* node) const {
            while (node && node->left) node = node->left;
            return node;
        }
        Node* find_max(Node* node) const {
            while (node && node->right) node = node->right;
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
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() const { 
        Node* min = root;
        while (min && min->left) min = min->left;
        return Iterator(min, root); 
    }
    Iterator end() const { return Iterator(nullptr, root); }

    // 容量
    bool empty() const { return _size == 0; }
    std::size_t size() const { return _size; }

    // 插入 O(log n)
    bool insert(const T& val) {
        if (find_node(val)) return false;
        
        Node* z = new Node(val);
        Node* y = nullptr;
        Node* x = root;
        
        while (x) {
            y = x;
            if (z->data < x->data) x = x->left;
            else x = x->right;
        }
        
        z->parent = y;
        if (!y) root = z;
        else if (z->data < y->data) y->left = z;
        else y->right = z;
        
        insert_fixup(z);
        ++_size;
        return true;
    }

    // 查找 O(log n)
    bool find(const T& val) const {
        return find_node(val) != nullptr;
    }

    // 删除 O(log n)
    bool erase(const T& val) {
        Node* z = find_node(val);
        if (!z) return false;
        delete_node(z);
        --_size;
        return true;
    }

    // 清空
    void clear() {
        clear_tree(root);
        root = nullptr;
        _size = 0;
    }

    // 拷贝构造
    MySet(const MySet& other) : root(nullptr), _size(other._size) {
        root = copy_tree(other.root, nullptr);
    }

    // 移动构造
    MySet(MySet&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    // 拷贝赋值
    MySet& operator=(const MySet& other) {
        if (this != &other) {
            MySet tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
    MySet& operator=(MySet&& other) noexcept {
        if (this != &other) {
            clear_tree(root);
            root = other.root;
            _size = other._size;
            other.root = nullptr;
            other._size = 0;
        }
        return *this;
    }

    void swap(MySet& other) {
        std::swap(root, other.root);
        std::swap(_size, other._size);
    }
};

// 测试代码
#include <iostream>
int main() {
    MySet<int> s;
    
    s.insert(5);
    s.insert(3);
    s.insert(7);
    s.insert(1);
    s.insert(9);
    s.insert(3);  // 重复插入，忽略
    
    std::cout << "Size: " << s.size() << std::endl;  // 5
    
    std::cout << "Elements (sorted): ";
    for (auto it = s.begin(); it != s.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 1 3 5 7 9
    
    std::cout << "Find 5: " << s.find(5) << std::endl;  // 1
    std::cout << "Find 6: " << s.find(6) << std::endl;  // 0
    
    s.erase(5);
    std::cout << "After erase 5: ";
    for (auto it = s.begin(); it != s.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 1 3 7 9
    
    MySet<int> s2;
    s2.insert(2);
    s2.insert(4);
    s2.insert(6);
    
    s = s2;
    std::cout << "After assignment: ";
    for (auto it = s.begin(); it != s.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;  // 2 4 6
    
    return 0;
}