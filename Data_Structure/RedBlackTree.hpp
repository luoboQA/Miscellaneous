// RedBlackTree.hpp
#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>

/**
 * @brief 红黑树模板
 * @tparam T 元素类型（必须支持比较操作 <, >, ==）
 * 
 * 红黑树性质：
 * 1. 每个节点要么是红色，要么是黑色
 * 2. 根节点是黑色
 * 3. 所有叶子节点（NIL）是黑色
 * 4. 红色节点的两个子节点都是黑色（不能有两个连续的红色节点）
 * 5. 从任意节点到其每个叶子节点的所有路径都包含相同数量的黑色节点
 * 
 * 时间复杂度：
 * - 插入：O(logn)
 * - 删除：O(logn)
 * - 查找：O(logn)
 * - 空间复杂度：O(n)
 */
template<typename T>
class RedBlackTree {
private:
    // 颜色枚举
    enum class Color { RED, BLACK };
    
    struct Node {
        T data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;
        
        Node() : data(T()), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
        explicit Node(const T& val) : data(val), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
        explicit Node(T&& val) : data(std::move(val)), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
        
        // 获取祖父节点
        Node* grandparent() const {
            return parent ? parent->parent : nullptr;
        }
        
        // 获取叔叔节点
        Node* uncle() const {
            Node* gp = grandparent();
            if (!gp) return nullptr;
            return (parent == gp->left) ? gp->right : gp->left;
        }
        
        // 获取兄弟节点
        Node* sibling() const {
            if (!parent) return nullptr;
            return (this == parent->left) ? parent->right : parent->left;
        }
        
        bool isLeftChild() const {
            return parent && this == parent->left;
        }
        
        bool isRightChild() const {
            return parent && this == parent->right;
        }
    };
    
    Node* _root;
    Node* _nil;           // 哨兵节点（表示空）
    std::size_t _size;
    
    /**
     * @brief 创建新节点
     * @param val 值
     * @return Node* 新节点
     */
    Node* createNode(const T& val) {
        Node* node = new Node(val);
        node->left = _nil;
        node->right = _nil;
        node->parent = _nil;
        return node;
    }
    
    Node* createNode(T&& val) {
        Node* node = new Node(std::move(val));
        node->left = _nil;
        node->right = _nil;
        node->parent = _nil;
        return node;
    }
    
    /**
     * @brief 左旋
     * @param x 旋转节点
     */
    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != _nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == _nil) {
            _root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }
    
    /**
     * @brief 右旋
     * @param x 旋转节点
     */
    void rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != _nil) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == _nil) {
            _root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }
    
    /**
     * @brief 插入修正
     * @param z 插入的节点
     */
    void insertFixup(Node* z) {
        while (z->parent->color == Color::RED) {
            if (z->parent->isLeftChild()) {
                Node* y = z->uncle();
                if (y->color == Color::RED) {
                    // Case 1: 叔叔是红色
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z->isRightChild()) {
                        // Case 2: z是右孩子
                        z = z->parent;
                        rotateLeft(z);
                    }
                    // Case 3: z是左孩子
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateRight(z->parent->parent);
                }
            } else {
                // 对称情况
                Node* y = z->uncle();
                if (y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z->isLeftChild()) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        _root->color = Color::BLACK;
    }
    
    /**
     * @brief 递归插入
     * @param node 当前节点
     * @param val 要插入的值
     */
    Node* insert(Node* node, const T& val) {
        if (node == _nil) {
            return createNode(val);
        }
        
        if (val < node->data) {
            node->left = insert(node->left, val);
            node->left->parent = node;
        } else if (val > node->data) {
            node->right = insert(node->right, val);
            node->right->parent = node;
        } else {
            throw std::invalid_argument("Element already exists");
        }
        return node;
    }
    
    /**
     * @brief 查找最小节点
     * @param node 起始节点
     * @return Node* 最小节点
     */
    Node* findMin(Node* node) const {
        while (node->left != _nil) {
            node = node->left;
        }
        return node;
    }
    
    /**
     * @brief 查找节点
     * @param node 当前节点
     * @param val 要查找的值
     * @return Node* 找到的节点，未找到返回_nil
     */
    Node* find(Node* node, const T& val) const {
        if (node == _nil || node->data == val) {
            return node;
        }
        if (val < node->data) {
            return find(node->left, val);
        }
        return find(node->right, val);
    }
    
    /**
     * @brief 删除修正
     * @param x 需要修正的节点
     */
    void deleteFixup(Node* x) {
        while (x != _root && x->color == Color::BLACK) {
            if (x->isLeftChild()) {
                Node* w = x->sibling();
                if (w->color == Color::RED) {
                    // Case 1: 兄弟是红色
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                    // Case 2: 兄弟的两个子节点都是黑色
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->right->color == Color::BLACK) {
                        // Case 3: 兄弟的左孩子是红色，右孩子是黑色
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    // Case 4: 兄弟的右孩子是红色
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    rotateLeft(x->parent);
                    x = _root;
                }
            } else {
                // 对称情况
                Node* w = x->sibling();
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    rotateRight(x->parent);
                    x = _root;
                }
            }
        }
        x->color = Color::BLACK;
    }
    
    /**
     * @brief 删除节点
     * @param z 要删除的节点
     */
    void eraseNode(Node* z) {
        Node* y = z;
        Node* x = nullptr;
        Color yOriginalColor = y->color;
        
        if (z->left == _nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == _nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = findMin(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        if (yOriginalColor == Color::BLACK) {
            deleteFixup(x);
        }
        delete z;
        --_size;
    }
    
    /**
     * @brief 用v子树替换u子树
     * @param u 被替换的子树
     * @param v 替换的子树
     */
    void transplant(Node* u, Node* v) {
        if (u->parent == _nil) {
            _root = v;
        } else if (u->isLeftChild()) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }
    
    /**
     * @brief 递归销毁树
     * @param node 当前节点
     */
    void destroyTree(Node* node) {
        if (node == _nil) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
    
    /**
     * @brief 递归拷贝树
     * @param node 当前节点
     * @param parent 父节点
     * @return Node* 拷贝后的节点
     */
    Node* copyTree(Node* node, Node* parent) {
        if (node == _nil) return _nil;
        
        Node* newNode = new Node(node->data);
        newNode->color = node->color;
        newNode->parent = parent;
        newNode->left = copyTree(node->left, newNode);
        newNode->right = copyTree(node->right, newNode);
        return newNode;
    }
    
    /**
     * @brief 递归中序遍历
     * @param node 当前节点
     * @param os 输出流
     */
    void inorder(Node* node, std::ostream& os) const {
        if (node == _nil) return;
        inorder(node->left, os);
        os << node->data << " ";
        inorder(node->right, os);
    }
    
    /**
     * @brief 递归打印树结构
     * @param node 当前节点
     * @param prefix 前缀
     * @param isLeft 是否为左子节点
     * @param os 输出流
     */
    void printTree(Node* node, const std::string& prefix, bool isLeft, std::ostream& os) const {
        if (node == _nil) return;
        
        os << prefix;
        os << (isLeft ? "├── " : "└── ");
        os << node->data << "(" << (node->color == Color::RED ? "R" : "B") << ")\n";
        
        std::string newPrefix = prefix + (isLeft ? "│   " : "    ");
        printTree(node->left, newPrefix, true, os);
        printTree(node->right, newPrefix, false, os);
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     */
    RedBlackTree() : _size(0) {
        _nil = new Node();
        _nil->color = Color::BLACK;
        _nil->left = _nil->right = _nil->parent = _nil;
        _root = _nil;
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    RedBlackTree(std::initializer_list<T> ilist) : RedBlackTree() {
        for (const T& val : ilist) {
            insert(val);
        }
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     */
    ~RedBlackTree() {
        destroyTree(_root);
        delete _nil;
    }
    
    // ---- 拷贝控制 ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     */
    RedBlackTree(const RedBlackTree& other) : _size(other._size) {
        _nil = new Node();
        _nil->color = Color::BLACK;
        _nil->left = _nil->right = _nil->parent = _nil;
        _root = copyTree(other._root, _nil);
    }
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     */
    RedBlackTree(RedBlackTree&& other) noexcept 
        : _root(other._root), _nil(other._nil), _size(other._size) {
        other._root = nullptr;
        other._nil = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return RedBlackTree&
     */
    RedBlackTree& operator=(const RedBlackTree& other) {
        if (this != &other) {
            RedBlackTree tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return RedBlackTree&
     */
    RedBlackTree& operator=(RedBlackTree&& other) noexcept {
        if (this != &other) {
            this->~RedBlackTree();
            _root = other._root;
            _nil = other._nil;
            _size = other._size;
            other._root = nullptr;
            other._nil = nullptr;
            other._size = 0;
        }
        return *this;
    }
    
    /**
     * @brief 交换两棵树
     * @param other 要交换的对象
     */
    void swap(RedBlackTree& other) {
        std::swap(_root, other._root);
        std::swap(_nil, other._nil);
        std::swap(_size, other._size);
    }
    
    // ---- 容量 ----
    
    /**
     * @brief 返回元素个数
     * @return std::size_t
     */
    std::size_t size() const { return _size; }
    
    /**
     * @brief 判断是否为空
     * @return bool
     */
    bool empty() const { return _size == 0; }
    
    // ---- 查找 ----
    
    /**
     * @brief 查找元素
     * @param val 要查找的值
     * @return bool 是否存在
     */
    bool contains(const T& val) const {
        return find(_root, val) != _nil;
    }
    
    /**
     * @brief 查找并返回元素引用
     * @param val 要查找的值
     * @return const T& 元素引用
     * @throws std::out_of_range 如果元素不存在
     */
    const T& at(const T& val) const {
        Node* node = find(_root, val);
        if (node == _nil) {
            throw std::out_of_range("Element not found");
        }
        return node->data;
    }
    
    // ---- 插入 ----
    
    /**
     * @brief 插入元素（拷贝版本）
     * @param val 要插入的值
     * @throws std::invalid_argument 如果元素已存在
     */
    void insert(const T& val) {
        if (contains(val)) {
            throw std::invalid_argument("Element already exists");
        }
        
        Node* z = createNode(val);
        Node* y = _nil;
        Node* x = _root;
        
        while (x != _nil) {
            y = x;
            if (z->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        
        z->parent = y;
        if (y == _nil) {
            _root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }
        
        ++_size;
        insertFixup(z);
    }
    
    /**
     * @brief 插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * @throws std::invalid_argument 如果元素已存在
     */
    void insert(T&& val) {
        if (contains(val)) {
            throw std::invalid_argument("Element already exists");
        }
        
        Node* z = createNode(std::move(val));
        Node* y = _nil;
        Node* x = _root;
        
        while (x != _nil) {
            y = x;
            if (z->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        
        z->parent = y;
        if (y == _nil) {
            _root = z;
        } else if (z->data < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }
        
        ++_size;
        insertFixup(z);
    }
    
    // ---- 删除 ----
    
    /**
     * @brief 删除元素
     * @param val 要删除的值
     * @return bool 是否成功删除
     */
    bool erase(const T& val) {
        Node* z = find(_root, val);
        if (z == _nil) {
            return false;
        }
        eraseNode(z);
        return true;
    }
    
    /**
     * @brief 清空所有元素
     */
    void clear() {
        destroyTree(_root);
        _root = _nil;
        _size = 0;
    }
    
    // ---- 遍历 ----
    
    /**
     * @brief 中序遍历
     * @param os 输出流
     */
    void inorder(std::ostream& os = std::cout) const {
        inorder(_root, os);
        os << "\n";
    }
    
    /**
     * @brief 打印树结构
     * @param os 输出流
     */
    void printTree(std::ostream& os = std::cout) const {
        if (_root == _nil) {
            os << "Tree is empty\n";
            return;
        }
        printTree(_root, "", false, os);
    }
    
    /**
     * @brief 遍历并执行回调函数（中序）
     * @param callback 回调函数
     */
    void forEach(std::function<void(const T&)> callback) const {
        std::function<void(Node*)> traverse = [&](Node* node) {
            if (node == _nil) return;
            traverse(node->left);
            callback(node->data);
            traverse(node->right);
        };
        traverse(_root);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(RedBlackTree<T>& a, RedBlackTree<T>& b) {
    a.swap(b);
}

#endif // RED_BLACK_TREE_HPP