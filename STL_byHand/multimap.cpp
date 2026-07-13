template<typename Key, typename Value>
class MyMultimap {
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

    // 查找第一个等于 key 的节点
    Node* find_node(const Key& key) const {
        Node* cur = root;
        while (cur) {
            if (key == cur->data.first) return cur;
            else if (key < cur->data.first) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

    // 查找第一个 >= key 的节点（lower_bound）
    Node* lower_bound_node(const Key& key) const {
        Node* cur = root;
        Node* result = nullptr;
        while (cur) {
            if (cur->data.first >= key) {
                result = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return result;
    }

    // 查找第一个 > key 的节点（upper_bound）
    Node* upper_bound_node(const Key& key) const {
        Node* cur = root;
        Node* result = nullptr;
        while (cur) {
            if (cur->data.first > key) {
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
        Node* new_node = new Node(node->data.first, node->data.second);
        new_node->color = node->color;
        new_node->parent = parent;
        new_node->left = copy_tree(node->left, new_node);
        new_node->right = copy_tree(node->right, new_node);
        return new_node;
    }

public:
    MyMultimap() : root(nullptr), _size(0) {}
    
    ~MyMultimap() { clear_tree(root); }

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

    // 插入 O(log n) - 允许重复键
    void insert(const Key& key, const Value& value) {
        Node* z = new Node(key, value);
        Node* y = nullptr;
        Node* x = root;
        
        while (x) {
            y = x;
            if (z->data.first < x->data.first) x = x->left;
            else x = x->right;  // 相等时往右走，允许重复
        }
        
        z->parent = y;
        if (!y) root = z;
        else if (z->data.first < y->data.first) y->left = z;
        else y->right = z;
        
        insert_fixup(z);
        ++_size;
    }

    // 查找 O(log n)
    bool find(const Key& key) const {
        return find_node(key) != nullptr;
    }

    // 计数 O(log n)
    std::size_t count(const Key& key) const {
        std::size_t cnt = 0;
        Node* cur = find_node(key);
        if (!cur) return 0;
        
        // 向左找
        Node* left = cur;
        while (left && left->data.first == key) {
            ++cnt;
            left = predecessor(left);
        }
        
        // 向右找（跳过已计数的当前节点）
        Node* right = cur->right;
        while (right && right->data.first == key) {
            ++cnt;
            right = right->right;
        }
        
        return cnt;
    }

    // lower_bound O(log n)
    Iterator lower_bound(const Key& key) const {
        return Iterator(lower_bound_node(key), root);
    }

    // upper_bound O(log n)
    Iterator upper_bound(const Key& key) const {
        return Iterator(upper_bound_node(key), root);
    }

    // 删除一个元素 O(log n)
    bool erase_one(const Key& key) {
        Node* z = find_node(key);
        if (!z) return false;
        delete_node(z);
        --_size;
        return true;
    }

    // 删除所有等于 key 的元素 O(k log n)
    std::size_t erase_all(const Key& key) {
        std::size_t cnt = 0;
        while (true) {
            Node* z = find_node(key);
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
    MyMultimap(const MyMultimap& other) : root(nullptr), _size(other._size) {
        root = copy_tree(other.root, nullptr);
    }

    // 移动构造
    MyMultimap(MyMultimap&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    // 拷贝赋值
    MyMultimap& operator=(const MyMultimap& other) {
        if (this != &other) {
            MyMultimap tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 移动赋值
    MyMultimap& operator=(MyMultimap&& other) noexcept {
        if (this != &other) {
            clear_tree(root);
            root = other.root;
            _size = other._size;
            other.root = nullptr;
            other._size = 0;
        }
        return *this;
    }

    void swap(MyMultimap& other) {
        std::swap(root, other.root);
        std::swap(_size, other._size);
    }

private:
    // 前驱节点（用于 count）
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
};

// 测试代码
#include <iostream>
int main() {
    MyMultimap<std::string, int> mm;
    
    mm.insert("apple", 5);
    mm.insert("banana", 3);
    mm.insert("apple", 8);   // 允许重复键
    mm.insert("orange", 7);
    mm.insert("banana", 2);  // 允许重复键
    mm.insert("apple", 1);   // 允许重复键
    
    std::cout << "Size: " << mm.size() << std::endl;  // 6
    
    std::cout << "Elements: ";
    for (auto it = mm.begin(); it != mm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // apple:5 apple:8 apple:1 banana:3 banana:2 orange:7
    
    std::cout << "Count of apple: " << mm.count("apple") << std::endl;   // 3
    std::cout << "Count of banana: " << mm.count("banana") << std::endl; // 2
    std::cout << "Count of peach: " << mm.count("peach") << std::endl;   // 0
    
    std::cout << "Find apple: " << mm.find("apple") << std::endl;   // 1
    std::cout << "Find peach: " << mm.find("peach") << std::endl;   // 0
    
    std::cout << "Lower bound 'banana': ";
    for (auto it = mm.lower_bound("banana"); it != mm.upper_bound("banana"); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // banana:3 banana:2
    
    mm.erase_one("apple");  // 删除一个 apple
    std::cout << "After erase one apple, size: " << mm.size() << std::endl;  // 5
    std::cout << "Elements: ";
    for (auto it = mm.begin(); it != mm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // apple:8 apple:1 banana:3 banana:2 orange:7
    
    mm.erase_all("banana");  // 删除所有 banana
    std::cout << "After erase all banana, size: " << mm.size() << std::endl;  // 3
    std::cout << "Elements: ";
    for (auto it = mm.begin(); it != mm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // apple:8 apple:1 orange:7
    
    // 使用 lower_bound 和 upper_bound 遍历所有 apple
    std::cout << "All apples: ";
    for (auto it = mm.lower_bound("apple"); it != mm.upper_bound("apple"); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;  // apple:8 apple:1
    
    return 0;
}