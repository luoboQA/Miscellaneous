/**
 * @brief 集合（Set）容器
 * 
 * 基于红黑树实现，元素唯一，自动排序（升序）
 * 插入重复元素会被拒绝
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
 * - 遍历：O(n)
 * 
 * @tparam T 元素类型（必须支持比较运算符 < 和 ==）
 */
template<typename T>
class MySet {
private:
    // ---- 颜色枚举 ----
    enum Color { RED, BLACK };
    
    /**
     * @brief 红黑树节点结构
     * 包含数据、颜色、左右子节点和父节点指针
     */
    struct Node {
        T data;         // 存储的数据
        Node* left;     // 左子节点
        Node* right;    // 右子节点
        Node* parent;   // 父节点
        Color color;    // 节点颜色
        
        /**
         * @brief 节点构造函数
         * @param val 存储的值（默认构造）
         * 新节点默认为红色（更容易修复红黑树性质）
         */
        Node(const T& val = T()) 
            : data(val), left(nullptr), right(nullptr), 
              parent(nullptr), color(RED) {}
        
        /**
         *  @brief 节点构造函数（移动语义）
         *  @param val 存储的值（右值引用）
         *  新节点默认为红色
         */
        Node(T&& val) 
            : data(std::move(val)), left(nullptr), right(nullptr), 
              parent(nullptr), color(RED) {}
    };
    
    Node* root;           // 树根节点
    std::size_t _size;    // 元素个数

    // ---- 旋转操作 ----
    
    /**
     * @brief 左旋操作
     * @param x 旋转支点
     * 时间复杂度：O(1)
     * 
     * 图示：       x                    y
     *             / \                  / \
     *            a   y       =>       x   c
     *               / \              / \
     *              b   c            a   b
     * 
     * 用于插入/删除修复中恢复红黑树平衡
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
     * 时间复杂度：O(1)
     * 
     * 图示：       y                    x
     *             / \                  / \
     *            x   c       =>       a   y
     *           / \                      / \
     *          a   b                    b   c
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
     * 
     * 三种情况：
     * 1. 叔叔是红色 → 颜色翻转，向上回溯
     * 2. 叔叔是黑色，当前节点是右子节点 → 左旋，转为情况3
     * 3. 叔叔是黑色，当前节点是左子节点 → 右旋 + 颜色翻转，结束
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
     * 时间复杂度：O(log n)
     * 
     * 四种情况：
     * 1. 兄弟是红色 → 左旋 + 颜色翻转
     * 2. 兄弟是黑色，两个子都是黑色 → 兄弟变红，上移
     * 3. 兄弟是黑色，左子红右子黑 → 右旋 + 颜色翻转
     * 4. 兄弟是黑色，右子红 → 左旋 + 颜色翻转，结束
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

    // ---- 查找函数 ----
    
    /**
     * @brief 查找等于 val 的节点
     * @param val 要查找的值
     * @return Node* 找到的节点，未找到返回 nullptr
     * 时间复杂度：O(log n)
     */
    Node* find_node(const T& val) const {
        Node* cur = root;
        while (cur) {
            if (val == cur->data) return cur;
            else if (val < cur->data) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
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
        Node* new_node = new Node(node->data);
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
    MySet() : root(nullptr), _size(0) {}
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 递归释放所有节点
     * 时间复杂度：O(n)
     */
    ~MySet() { clear_tree(root); }

    // ---- 迭代器 ----
    
    /**
     * @brief 红黑树迭代器
     * 按中序遍历顺序（升序）
     * 支持正向遍历（++）
     */
    class Iterator {
    private:
        Node* ptr;    // 当前节点指针
        Node* root;   // 树根（保留备用）
        
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
         * @return T& 当前元素的引用
         */
        T& operator*() { return ptr->data; }
        
        /**
         * @brief 前置递增（++it）
         * @return Iterator& 移动到下一个元素
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
     * @return Iterator 指向最小元素的迭代器
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
    
    /**
     * @brief 判断集合是否为空
     * @return bool 是否为空
     * 时间复杂度：O(1)
     */
    bool empty() const { return _size == 0; }
    
    /**
     * @brief 返回集合中的元素个数
     * @return std::size_t 元素个数
     * 时间复杂度：O(1)
     */
    std::size_t size() const { return _size; }

    // ---- 插入 ----
    
    /**
     * @brief 插入元素
     * @param val 要插入的值
     * @return bool 是否插入成功（元素已存在时返回 false）
     * 时间复杂度：O(log n)
     * 
     * 实现细节：
     * 1. 先查找元素是否已存在
     * 2. 如果不存在，按 BST 规则插入
     * 3. 调用 insert_fixup 修复红黑树性质
     */
    bool insert(const T& val) {
        if (find_node(val)) return false;  // 元素已存在，拒绝插入
        
        Node* z = new Node(val);
        Node* y = nullptr;
        Node* x = root;
        
        // 找插入位置
        while (x) {
            y = x;
            if (z->data < x->data) x = x->left;
            else x = x->right;
        }
        
        // 插入新节点
        z->parent = y;
        if (!y) root = z;
        else if (z->data < y->data) y->left = z;
        else y->right = z;
        
        insert_fixup(z);
        ++_size;
        return true;
    }

    // ---- 查找 ----
    
    /**
     * @brief 查找值是否存在
     * @param val 要查找的值
     * @return bool 值是否存在
     * 时间复杂度：O(log n)
     */
    bool find(const T& val) const {
        return find_node(val) != nullptr;
    }

    // ---- 删除 ----
    
    /**
     * @brief 删除指定值的元素
     * @param val 要删除的值
     * @return bool 是否删除成功（值不存在时返回 false）
     * 时间复杂度：O(log n)
     */
    bool erase(const T& val) {
        Node* z = find_node(val);
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
    MySet(const MySet& other) : root(nullptr), _size(other._size) {
        root = copy_tree(other.root, nullptr);
    }

    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    MySet(MySet&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return MySet& 当前对象的引用
     * 时间复杂度：O(n)
     * 使用 copy-and-swap 惯用法
     */
    MySet& operator=(const MySet& other) {
        if (this != &other) {
            MySet tmp(other);
            swap(tmp);
        }
        return *this;
    }

    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return MySet& 当前对象的引用
     * 时间复杂度：O(1)
     */
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

    /**
     * @brief 交换两个集合
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(MySet& other) {
        std::swap(root, other.root);
        std::swap(_size, other._size);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 * 时间复杂度：O(1)
 */
template<typename T>
void swap(MySet<T>& a, MySet<T>& b) {
    a.swap(b);
}