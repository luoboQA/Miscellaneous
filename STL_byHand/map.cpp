template<typename Key, typename Value>
class MyMap {
private:
    enum Color { RED, BLACK };
    
    struct Node {
        std::pair<const Key, Value> data;
        Node* left;
        Node* right;
        Node* parent;
        Color color;
        Node(const Key& k, const Value& v = Value()) 
            : data(k, v), left(nullptr), right(nullptr), 
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

    Node* find_node(const Key& key) const {
        Node* cur = root;
        while (cur) {
            if (key == cur->data.first) return cur;
            else if (key < cur->data.first) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
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
        Node* new_node = new Node(node->data.first, node->data.second);
        new_node->color = node->color;
        new_node->parent = parent;
        new_node->left = copy_tree(node->left, new_node);
        new_node->right = copy_tree(node->right, new_node);
        return new_node;
    }

public:
    MyMap() : root(nullptr), _size(0) {}
    
    ~MyMap() { clear_tree(root); }

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
    public:
        Iterator(Node* p = nullptr, Node* r = nullptr) : ptr(p), root(r) {}
        std::pair<const Key, Value>& operator*() { return ptr->data; }
        std::pair<const Key, Value>* operator->() { return &ptr->data; }
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
    bool insert(const Key& key, const Value& value) {
        if (find_node(key)) return false;
        
        Node* z = new Node(key, value);
        Node* y = nullptr;
        Node* x = root;
        
        while (x) {
            y = x;
            if (z->data.first < x->data.first) x = x->left;
            else x = x->right;
        }
        
        z->parent = y;
        if (!y) root = z;
        else if (z->data.first < y->data.first) y->left = z;
        else y->right = z;
        
        insert_fixup(z);
        ++_size;
        return true;
    }

    // 查找 O(log n)
    bool find(const Key& key) const {
        return find_node(key) != nullptr;
    }

    // [] 操作符 O(log n)
    Value& operator[](const Key& key) {
        Node* node = find_node(key);
        if (node) return node->data.second;
        
        // 插入新元素
        Node* z = new Node(key, Value());
        Node* y = nullptr;
        Node* x = root;
        
        while (x) {
            y = x;
            if (z->data.first < x->data.first) x = x->left;
            else x = x->right;
        }
        
        z->parent = y;
        if (!y) root = z;
        else if (z->data.first < y->data.first) y->left = z;
        else y->right = z;
        
        insert_fixup(z);
        ++_size;
        return z->data.second;
    }

    // at() 带边界检查 O(log n)
    Value& at(const Key& key) {
        Node* node = find_node(key);
        if (!node) throw std::out_of_range("key not found");
        return node->data.second;
    }

    const Value& at(const Key& key) const {
        Node* node = find_node(key);
        if (!node) throw std::out_of_range("key not found");
        return node->data.second;
    }

    // 删除 O(log n)
    bool erase(const Key& key) {
        Node* z = find_node(key);
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
    MyMap(const MyMap& other) : root(nullptr), _size(other._size) {
        root = copy_tree(other.root, nullptr);
    }

    // 移动构造
    MyMap(MyMap&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    // 拷贝赋值
    MyMap& operator=(const MyMap& other) {
        if (this != &other) {
            MyMap tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
    MyMap& operator=(MyMap&& other) noexcept {
        if (this != &other) {
            clear_tree(root);
            root = other.root;
            _size = other._size;
            other.root = nullptr;
            other._size = 0;
        }
        return *this;
    }

    void swap(MyMap& other) {
        std::swap(root, other.root);
        std::swap(_size, other._size);
    }
};

// 测试代码
#include <iostream>
int main() {
    MyMap<std::string, int> map;
    
    map.insert("apple", 5);
    map.insert("banana", 3);
    map.insert("orange", 7);
    map.insert("grape", 2);
    
    std::cout << "Size: " << map.size() << std::endl;  // 4
    
    std::cout << "Elements: ";
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // apple:5 banana:3 grape:2 orange:7
    
    std::cout << "Find 'banana': " << map.find("banana") << std::endl;  // 1
    std::cout << "Find 'peach': " << map.find("peach") << std::endl;    // 0
    
    std::cout << "apple = " << map["apple"] << std::endl;  // 5
    map["apple"] = 10;
    std::cout << "apple = " << map["apple"] << std::endl;  // 10
    
    map["peach"] = 8;  // 插入新元素
    std::cout << "After adding peach: ";
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;
    
    try {
        std::cout << map.at("watermelon") << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    map.erase("banana");
    std::cout << "After erase banana: ";
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;
    
    MyMap<std::string, int> map2;
    map2.insert("x", 1);
    map2.insert("y", 2);
    map2.insert("z", 3);
    
    map = map2;
    std::cout << "After assignment: ";
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // x:1 y:2 z:3
    
    return 0;
}