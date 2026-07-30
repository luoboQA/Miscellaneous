/**
 * @brief 多重映射（Multimap）容器
 * 
 * 基于红黑树实现，允许键重复（一个键可以对应多个值）
 * 键值对自动按键排序（升序），键相同的元素保持插入顺序
 * 
 * 与 MyMap 的区别：
 * - MyMap：键唯一，插入重复键会失败
 * - MyMultimap：键可重复，插入永远不会失败
 * 
 * 红黑树性质：
 * 1. 每个节点是红色或黑色
 * 2. 根节点是黑色
 * 3. 所有叶子（NIL）是黑色
 * 4. 红色节点的两个子节点都是黑色
 * 5. 从任一节点到其每个叶子的所有路径都包含相同数目的黑色节点
 * 
 * 时间复杂度：
 * - 插入：O(log n)
 * - 删除：O(log n)
 * - 查找：O(log n)
 * - count：O(k + log n)，k 为相同键的数量
 * - lower_bound/upper_bound：O(log n)
 * 
 * @tparam Key 键类型
 * @tparam Value 值类型
 */
template<typename Key, typename Value>
class MyMultimap {
private:
    // ---- 颜色枚举 ----
    enum Color { RED, BLACK };
    
    /**
     * @brief 红黑树节点结构
     * 包含键值对、颜色、左右子节点和父节点指针
     */
    struct Node {
        std::pair<const Key, Value> data;  // 键值对（键不可修改）
        Node* left;                         // 左子节点
        Node* right;                        // 右子节点
        Node* parent;                       // 父节点
        Color color;                        // 节点颜色
        
        /**
         * @brief 节点构造函数
         * @param k 键
         * @param v 值（默认构造）
         * 新节点默认为红色
         */
        Node(const Key& k, const Value& v = Value()) 
            : data(k, v), left(nullptr), right(nullptr), 
              parent(nullptr), color(RED) {}

        /**
         * @brief 节点构造函数（移动语义）
         * @param k 键
         * @param v 值（移动构造）
         * 新节点默认为红色
         */
        Node(const Key& k, Value&& v) 
            : data(k, std::move(v)), left(nullptr), right(nullptr), 
              parent(nullptr), color(RED) {}
    };
    
    Node* root;           // 树根节点
    std::size_t _size;    // 元素个数

    // ---- 旋转操作 ----
    
    /**
     * @brief 左旋操作
     * @param x 旋转支点
     * 
     * 图示：       x                    y
     *             / \                  / \
     *            a   y       =>       x   c
     *               / \              / \
     *              b   c            a   b
     * 
     * 时间复杂度：O(1)
     */
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

    /**
     * @brief 右旋操作
     * @param y 旋转支点
     * 
     * 图示：       y                    x
     *             / \                  / \
     *            x   c       =>       a   y
     *           / \                      / \
     *          a   b                    b   c
     * 
     * 时间复杂度：O(1)
     */
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

    // ---- 插入修复 ----
    
    /**
     * @brief 插入后修复红黑树性质
     * @param z 新插入的节点
     * 时间复杂度：O(log n)
     */
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

    // ---- 辅助查找函数 ----
    
    /**
     * @brief 查找子树中的最小节点
     * @param x 子树的根节点
     * @return Node* 最小节点（最左节点）
     * 时间复杂度：O(log n)
     */
    Node* minimum(Node* x) {
        while (x->left) x = x->left;
        return x;
    }

    /**
     * @brief 查找子树中的最大节点
     * @param x 子树的根节点
     * @return Node* 最大节点（最右节点）
     * 时间复杂度：O(log n)
     */
    Node* maximum(Node* x) {
        while (x->right) x = x->right;
        return x;
    }

    // ---- 删除修复 ----
    
    /**
     * @brief 删除后修复红黑树性质
     * @param x 替代节点（可能为 nullptr）
     * @param parent x 的父节点
     * 时间复杂度：O(log n)
     */
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

    /**
     * @brief 移植节点（用 v 替换 u）
     * @param u 要被替换的节点
     * @param v 替换到 u 位置的节点
     * 时间复杂度：O(1)
     */
    void transplant(Node* u, Node* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }

    /**
     * @brief 删除节点
     * @param z 要删除的节点
     * 时间复杂度：O(log n)
     */
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

    // ---- 查找函数 ----
    
    /**
     * @brief 查找等于 key 的第一个节点
     * @param key 要查找的键
     * @return Node* 找到的节点，未找到返回 nullptr
     * 时间复杂度：O(log n)
     */
    Node* find_node(const Key& key) const {
        Node* cur = root;
        while (cur) {
            if (key == cur->data.first) return cur;
            else if (key < cur->data.first) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

    /**
     * @brief 查找第一个 >= key 的节点（lower_bound）
     * @param key 要查找的键
     * @return Node* 第一个 >= key 的节点，不存在返回 nullptr
     * 时间复杂度：O(log n)
     */
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

    /**
     * @brief 查找第一个 > key 的节点（upper_bound）
     * @param key 要查找的键
     * @return Node* 第一个 > key 的节点，不存在返回 nullptr
     * 时间复杂度：O(log n)
     */
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

    // ---- 前驱节点 ----
    
    /**
     * @brief 查找前驱节点（中序遍历的前一个）
     * @param node 当前节点
     * @return Node* 前驱节点
     * 时间复杂度：O(log n)
     * 
     * 用于 count 函数中向左查找相同键的节点
     */
    Node* predecessor(Node* node) const {
        // 有左子树 → 左子树的最大节点
        if (node->left) {
            node = node->left;
            while (node->right) node = node->right;
            return node;
        }
        // 没有左子树 → 向上找
        Node* parent = node->parent;
        while (parent && node == parent->left) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    // ---- 树操作 ----
    
    /**
     * @brief 递归销毁整棵树
     * @param node 当前节点
     * 时间复杂度：O(n)
     */
    void clear_tree(Node* node) {
        if (node) {
            clear_tree(node->left);
            clear_tree(node->right);
            delete node;
        }
    }

    /**
     * @brief 递归拷贝整棵树
     * @param node 当前节点
     * @param parent 新树中的父节点
     * @return Node* 拷贝后的节点
     * 时间复杂度：O(n)
     */
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
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空的红黑树
     * 时间复杂度：O(1)
     */
    MyMultimap() : root(nullptr), _size(0) {}
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 递归释放所有节点
     * 时间复杂度：O(n)
     */
    ~MyMultimap() { clear_tree(root); }

    // ---- 迭代器 ----
    
    /**
     * @brief 红黑树迭代器
     * 按中序遍历顺序（键升序）
     * 允许键重复，相同键的节点按插入顺序排列
     */
    class Iterator {
    private:
        Node* ptr;    // 当前节点指针
        Node* root;   // 树根（用于找最小节点）
        
        /**
         * @brief 找子树最小节点
         * @param node 子树根节点
         * @return Node* 最小节点
         */
        Node* find_min(Node* node) const {
            while (node && node->left) node = node->left;
            return node;
        }
        
        /**
         * @brief 找后继节点（中序遍历的下一个）
         * @param node 当前节点
         * @return Node* 后继节点
         * 
         * 两种情况：
         * 1. 有右子树 → 右子树的最小节点
         * 2. 无右子树 → 向上找，直到当前节点是父节点的左子节点
         */
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
        
        /**
         * @brief 解引用运算符
         * @return std::pair<const Key, Value>& 键值对引用
         */
        std::pair<const Key, Value>& operator*() { return ptr->data; }
        
        /**
         * @brief 箭头运算符
         * @return std::pair<const Key, Value>* 键值对指针
         */
        std::pair<const Key, Value>* operator->() { return &ptr->data; }
        
        /**
         * @brief 前置递增（++it）
         * @return Iterator& 移动到下一个键值对
         * 时间复杂度：均摊 O(1)
         */
        Iterator& operator++() { 
            if (ptr) ptr = successor(ptr); 
            return *this; 
        }
        
        /**
         * @brief 后置递增（it++）
         * @return Iterator 移动前的迭代器副本
         * 时间复杂度：均摊 O(1)
         */
        Iterator operator++(int) { 
            Iterator tmp = *this; 
            if (ptr) ptr = successor(ptr); 
            return tmp; 
        }
        
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    /**
     * @brief 返回指向第一个元素的迭代器（最小值）
     * @return Iterator 指向最小键的迭代器
     * 时间复杂度：O(log n)
     */
    Iterator begin() const { 
        Node* min = root;
        while (min && min->left) min = min->left;
        return Iterator(min, root); 
    }
    
    /**
     * @brief 返回指向尾后位置的迭代器
     * @return Iterator nullptr 迭代器
     * 时间复杂度：O(1)
     */
    Iterator end() const { return Iterator(nullptr, root); }

    // ---- 容量 ----
    
    bool empty() const { return _size == 0; }
    std::size_t size() const { return _size; }

    // ---- 插入 ----
    
    /**
     * @brief 插入键值对（允许重复键）
     * @param key 键
     * @param value 值
     * 时间复杂度：O(log n)
     * 
     * 与 MyMap 不同，这里不会检查键是否已存在
     * 相等键的节点会插入到右子树中
     */
    void insert(const Key& key, const Value& value) {
        Node* z = new Node(key, value);
        Node* y = nullptr;
        Node* x = root;
        
        // 找插入位置
        while (x) {
            y = x;
            if (z->data.first < x->data.first) x = x->left;
            else x = x->right;  // 相等时往右走，允许重复
        }
        
        // 插入新节点
        z->parent = y;
        if (!y) root = z;
        else if (z->data.first < y->data.first) y->left = z;
        else y->right = z;  // 相等时作为右子节点
        
        insert_fixup(z);
        ++_size;
    }

    // ---- 查找 ----
    
    /**
     * @brief 查找键是否存在
     * @param key 要查找的键
     * @return bool 键是否存在
     * 时间复杂度：O(log n)
     */
    bool find(const Key& key) const {
        return find_node(key) != nullptr;
    }

    // ---- 计数 ----
    
    /**
     * @brief 统计键 key 出现的次数
     * @param key 要统计的键
     * @return std::size_t 出现次数
     * 时间复杂度：O(k + log n)，k 为相同键的数量
     * 
     * 实现细节：
     * 1. 找到一个等于 key 的节点
     * 2. 向左遍历所有相同键的节点
     * 3. 向右遍历所有相同键的节点
     */
    std::size_t count(const Key& key) const {
        std::size_t cnt = 0;
        Node* cur = find_node(key);
        if (!cur) return 0;
        
        // 向左找相同键的节点
        Node* left = cur;
        while (left && left->data.first == key) {
            ++cnt;
            left = predecessor(left);
        }
        
        // 向右找相同键的节点（跳过已计数的当前节点）
        Node* right = cur->right;
        while (right && right->data.first == key) {
            ++cnt;
            right = right->right;
        }
        
        return cnt;
    }

    // ---- lower_bound / upper_bound ----
    
    /**
     * @brief 返回第一个 >= key 的迭代器
     * @param key 要查找的键
     * @return Iterator 指向第一个 >= key 的元素
     * 时间复杂度：O(log n)
     * 
     * 用于范围查询：[lower_bound(key), upper_bound(key))
     */
    Iterator lower_bound(const Key& key) const {
        return Iterator(lower_bound_node(key), root);
    }

    /**
     * @brief 返回第一个 > key 的迭代器
     * @param key 要查找的键
     * @return Iterator 指向第一个 > key 的元素
     * 时间复杂度：O(log n)
     */
    Iterator upper_bound(const Key& key) const {
        return Iterator(upper_bound_node(key), root);
    }

    // ---- 删除 ----
    
    /**
     * @brief 删除一个等于 key 的元素
     * @param key 要删除的键
     * @return bool 是否删除成功
     * 时间复杂度：O(log n)
     */
    bool erase_one(const Key& key) {
        Node* z = find_node(key);
        if (!z) return false;
        delete_node(z);
        --_size;
        return true;
    }

    /**
     * @brief 删除所有等于 key 的元素
     * @param key 要删除的键
     * @return std::size_t 删除的元素个数
     * 时间复杂度：O(k log n)，k 为相同键的数量
     * 
     * 循环调用 erase_one，直到所有相同键被删除
     */
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

    // ---- 清空 ----
    
    /**
     * @brief 清空所有元素
     * 时间复杂度：O(n)
     */
    void clear() {
        clear_tree(root);
        root = nullptr;
        _size = 0;
    }

    // ---- 拷贝控制（五法则） ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    MyMultimap(const MyMultimap& other) : root(nullptr), _size(other._size) {
        root = copy_tree(other.root, nullptr);
    }

    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    MyMultimap(MyMultimap&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return MyMultimap& 当前对象的引用
     * 时间复杂度：O(n)
     */
    MyMultimap& operator=(const MyMultimap& other) {
        if (this != &other) {
            MyMultimap tmp(other);
            swap(tmp);
        }
        return *this;
    }

    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return MyMultimap& 当前对象的引用
     * 时间复杂度：O(1)
     */
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

    /**
     * @brief 交换两个 multimap
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(MyMultimap& other) {
        std::swap(root, other.root);
        std::swap(_size, other._size);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * 时间复杂度：O(1)
 */
template<typename Key, typename Value>
void swap(MyMultimap<Key, Value>& a, MyMultimap<Key, Value>& b) {
    a.swap(b);
}
