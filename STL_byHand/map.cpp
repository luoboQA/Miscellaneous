/**
 * @brief 红黑树实现的关联容器（Map）
 * 键值对存储，键唯一，自动排序
 * 
 * 红黑树性质：
 * 1. 每个节点是红色或黑色
 * 2. 根节点是黑色
 * 3. 所有叶子（NIL）是黑色
 * 4. 红色节点的两个子节点都是黑色（不能有连续的红色节点）\
 * 新节点 “插入” 默认是红色，所以它可能破坏的性质只有一条：不能有连续的红色节点（性质4），将全局问题变为局部问题
 * 5. 从任一节点到其每个叶子的所有路径都包含相同数目的黑色节点
 * 全局的 “黑高” 平衡。为了修复它，你可能需要沿着整棵树向上回溯，进行大量的旋转和颜色翻转，甚至可能需要调整根节点
 * 
 * 颜色是用来"约束树形"的规则，让树自动保持平衡，不会退化成链表
 * bad:
 * 1
 *  \
 *   2
 *    \
 *     3
 *      \
 *       4
 *        \
 *         5
 * 
 * 时间复杂度：
 * - 插入：O(log n)
 * - 删除：O(log n)
 * - 查找：O(log n)
 */
template<typename Key, typename Value>
class MyMap {
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
         * 新节点默认为红色（满足红黑树性质更容易修复）
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
        Node(Key&& k, Value&& v) 
            : data(std::move(k), std::move(v)), left(nullptr), right(nullptr), 
              parent(nullptr), color(RED) {}
    };
    
    Node* root;           // 树根节点
    std::size_t _size;    // 元素个数

    // ---- 旋转操作 ----
    
    /**
     * @brief 左旋操作
     * @param x 旋转支点
     * 二叉搜索树的核心规则是：左子树所有节点 < 根节点 < 右子树所有节点,a < x < b < y < c
     * 为了保持二叉搜索树（BST）的性质把右子节点 y 的右子树 c 留下来，而把它的左子树 b 过继给 x 
     * 图示：       x                    y
     *             / \                  / \
     *            a   y       =>       x   c
     *               / \              / \
     *              b   c            a   b
     * 
     * 时间复杂度：O(1)
     */
    void left_rotate(Node* x) {
        Node* y = x->right;          // y 是 x 的右子节点
        x->right = y->left;          // y 的左子树变成 x 的右子树
        if (y->left) y->left->parent = x;  // 更新父指针
        y->parent = x->parent;       // y 继承 x 的父节点
        
        // 更新父节点对 y 的引用
        if (!x->parent) root = y;    // x 是根节点
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        
        y->left = x;                 // x 变成 y 的左子节点
        x->parent = y;               // 更新 x 的父指针
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
        Node* x = y->left;           // x 是 y 的左子节点
        y->left = x->right;          // x 的右子树变成 y 的左子树
        if (x->right) x->right->parent = y;  // 更新父指针
        x->parent = y->parent;       // x 继承 y 的父节点
        
        // 更新父节点对 x 的引用
        if (!y->parent) root = x;    // y 是根节点
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        
        x->right = y;                // y 变成 x 的右子节点
        y->parent = x;               // 更新 y 的父指针
    }

    // ---- 插入修复 ----
    
    /**
     * @brief 插入后修复红黑树性质
     * @param z 新插入的节点
     * 
     * 时间复杂度：O(log n)
     *    祖父 (g)
     *     /    \
     *   叔 (u)  父 (p)   ← 叔就是祖父的另一个子节点
     *           \
     *            z (当前节点)
     * 
     * 三种情况处理：
     * 1. 叔叔是红色 → 颜色翻转
     *  修复前：                修复后：
     *    g (红)                g (红)
     *    /  \                  /  \
     *  p (红) u (红)    →    p (黑) u (黑)
     *    /                      /
     *  z (红)                z (红)
     * 
     * 2. 叔叔是黑色，当前节点是右子节点 → 左旋
     *  修复前：                左旋后：
     *    g (黑)                 g (黑)
     *   /  \                   /  \
     *  p (红) u (黑)    →    z (红) u (黑)
     *  \                      /
     *  z (红)                p (红) 
     * 
     * 3. 叔叔是黑色，当前节点是左子节点 → 右旋 + 颜色翻转
     *  修复前：                右旋 + 颜色翻转后：
     *    g (红)                p (黑)
     *   /  \                   /  \
     *  p (红) u (黑)    →    z (红) g (红)
     *  /                            \
     * z (红)                        u (黑)
     *
     */
    void insert_fixup(Node* z) {
        // 当父节点存在且为红色时需要修复
        while (z->parent && z->parent->color == RED) {
            // 父节点是祖父节点的左子节点
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;  // 叔叔节点
                
                // 情况1：叔叔是红色 → 颜色翻转
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;  // 向上回溯
                } else {
                    // 情况2：当前节点是右子节点 → 左旋
                    if (z == z->parent->right) {
                        z = z->parent;
                        left_rotate(z);
                    }
                    // 情况3：当前节点是左子节点 → 右旋 + 颜色翻转
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    right_rotate(z->parent->parent);
                }
            } else {  // 父节点是祖父节点的右子节点（对称情况）
                Node* y = z->parent->parent->left;   // 叔叔节点
                
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
        root->color = BLACK;  // 根节点始终为黑色
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

    // ---- 删除修复 ----
    
    /**
     * @brief 删除后修复红黑树性质
     * @param x 替代节点（可能为 nullptr）
     * @param parent x 的父节点
     * 
     * 时间复杂度：O(log n)
     */
    void delete_fixup(Node* x, Node* parent) {
        while (x != root && (!x || x->color == BLACK)) {
            if (x == parent->left) {
                Node* w = parent->right;  // 兄弟节点
                
                // 情况1：兄弟是红色 → 左旋 + 颜色翻转
                if (w && w->color == RED) {
                    w->color = BLACK;
                    parent->color = RED;
                    left_rotate(parent);
                    w = parent->right;
                }
                
                // 情况2：兄弟的两个子节点都是黑色
                if ((!w->left || w->left->color == BLACK) && 
                    (!w->right || w->right->color == BLACK)) {
                    if (w) w->color = RED;
                    x = parent;
                    parent = x->parent;
                } else {
                    // 情况3：兄弟的左子节点是红色，右子节点是黑色
                    if (!w->right || w->right->color == BLACK) {
                        if (w->left) w->left->color = BLACK;
                        if (w) w->color = RED;
                        if (w) right_rotate(w);
                        w = parent->right;
                    }
                    // 情况4：兄弟的右子节点是红色
                    if (w) w->color = parent->color;
                    parent->color = BLACK;
                    if (w && w->right) w->right->color = BLACK;
                    left_rotate(parent);
                    x = root;
                    break;
                }
            } else {  // 对称情况
                Node* w = parent->left;   // 兄弟节点
                
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
        if (!u->parent) root = v;       // u 是根节点
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }

    /**
     * @brief 删除节点
     * @param z 要删除的节点
     * 时间复杂度：O(log n)
     * 
     * 三种情况：
     * 1. 没有左子节点 → 用右子节点替换
     * 2. 没有右子节点 → 用左子节点替换
     * 3. 有两个子节点 → 用后继节点替换
     */
    void delete_node(Node* z) {
        Node* y = z;                    // y 是真正被删除的节点
        Node* x = nullptr;              // x 是 y 的替代节点
        Node* parent = nullptr;
        Color y_original_color = y->color;  // 保存原始颜色
        
        // 情况1：没有左子节点
        if (!z->left) {
            x = z->right;
            parent = z->parent;
            transplant(z, z->right);
        }
        // 情况2：没有右子节点
        else if (!z->right) {
            x = z->left;
            parent = z->parent;
            transplant(z, z->left);
        }
        // 情况3：有两个子节点 → 找后继
        else {
            y = minimum(z->right);       // y 是 z 的后继
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
        
        // 如果删除的是黑色节点，需要修复
        if (y_original_color == BLACK) {
            delete_fixup(x, parent);
        }
    }

    /**
     * @brief 查找键对应的节点
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
    MyMap() : root(nullptr), _size(0) {}
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 递归释放所有节点
     * 时间复杂度：O(n)
     */
    ~MyMap() { clear_tree(root); }

    // ---- 迭代器 ----
    
    /**
     * @brief 红黑树迭代器
     * 按中序遍历顺序（键升序）
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
     * @brief 插入键值对
     * @param key 键
     * @param value 值
     * @return bool 是否插入成功（键已存在时返回 false）
     * 时间复杂度：O(log n)
     */
    bool insert(const Key& key, const Value& value) {
        if (find_node(key)) return false;  // 键已存在
        
        Node* z = new Node(key, value);
        Node* y = nullptr;
        Node* x = root;
        
        // 找插入位置
        while (x) {
            y = x;
            if (z->data.first < x->data.first) x = x->left;
            else x = x->right;
        }
        
        // 插入新节点
        z->parent = y;
        if (!y) root = z;
        else if (z->data.first < y->data.first) y->left = z;
        else y->right = z;
        
        insert_fixup(z);  // 修复红黑树性质
        ++_size;
        return true;
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

    // ---- [] 操作符 ----
    
    /**
     * @brief 键值访问操作符
     * @param key 要访问的键
     * @return Value& 对应值的引用
     * 时间复杂度：O(log n)
     * 
     * 如果键不存在，则插入默认值并返回引用
     */
    Value& operator[](const Key& key) {
        Node* node = find_node(key);
        if (node) return node->data.second;
        
        // 键不存在，插入默认值
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

    // ---- at() 带边界检查 ----
    
    /**
     * @brief 带边界检查的键值访问（非常量版本）
     * @param key 要访问的键
     * @return Value& 对应值的引用
     * @throws std::out_of_range 如果键不存在
     * 时间复杂度：O(log n)
     */
    Value& at(const Key& key) {
        Node* node = find_node(key);
        if (!node) throw std::out_of_range("key not found");
        return node->data.second;
    }

    /**
     * @brief 带边界检查的键值访问（常量版本）
     * @param key 要访问的键
     * @return const Value& 对应值的常量引用
     * @throws std::out_of_range 如果键不存在
     * 时间复杂度：O(log n)
     */
    const Value& at(const Key& key) const {
        Node* node = find_node(key);
        if (!node) throw std::out_of_range("key not found");
        return node->data.second;
    }

    // ---- 删除 ----
    
    /**
     * @brief 删除指定键的元素
     * @param key 要删除的键
     * @return bool 是否删除成功（键不存在时返回 false）
     * 时间复杂度：O(log n)
     */
    bool erase(const Key& key) {
        Node* z = find_node(key);
        if (!z) return false;
        delete_node(z);
        --_size;
        return true;
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
    MyMap(const MyMap& other) : root(nullptr), _size(other._size) {
        root = copy_tree(other.root, nullptr);
    }

    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    MyMap(MyMap&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return MyMap& 当前对象的引用
     * 时间复杂度：O(n)
     * 使用 copy-and-swap 惯用法
     */
    MyMap& operator=(const MyMap& other) {
        if (this != &other) {
            MyMap tmp(other);
            swap(tmp);
        }
        return *this;
    }

    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return MyMap& 当前对象的引用
     * 时间复杂度：O(1)
     */
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

    /**
     * @brief 交换两个 map
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(MyMap& other) {
        std::swap(root, other.root);
        std::swap(_size, other._size);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * 时间复杂度：O(1)
 */
template<typename Key, typename Value>
void swap(MyMap<Key, Value>& a, MyMap<Key, Value>& b) {
    a.swap(b);
}

