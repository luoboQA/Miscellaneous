// AVLTree.hpp
#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <algorithm>
#include <stdexcept>

/**
 * @brief AVL平衡二叉树模板
 * @tparam T 元素类型（必须支持比较操作 <, >, ==）
 * 
 * 时间复杂度：
 * - 插入/删除/查找：O(logn)
 * - 空间复杂度：O(n)
 */
template<typename T>
class AVLTree {
private:
    struct Node {
        T data;
        int height;
        Node* left;
        Node* right;
        
        Node(const T& val) 
            : data(val), height(1), left(nullptr), right(nullptr) {}
        
        Node(T&& val) 
            : data(std::move(val)), height(1), left(nullptr), right(nullptr) {}
    };
    
    Node* _root; // 根节点指针
    std::size_t _size; // 节点数量
    
    // ---- 辅助函数 ----
    
    /**
     * @brief 获取节点高度
     * @param node 节点指针
     * @return int 高度（空节点为0）
     */
    int getHeight(Node* node) const {
        return node ? node->height : 0;
    }
    
    /**
     * @brief 更新节点高度
     * @param node 节点指针
     */
    void updateHeight(Node* node) {
        if (node) {
            node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        }
    }
    
    /**
     * @brief 计算平衡因子
     * @param node 节点指针
     * @return int 左子树高度 - 右子树高度
     */
    int balanceFactor(Node* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    /**
     * @brief 右旋（LL旋转）
     * @param node 失衡节点
     * @return Node* 新的根节点
     * 
     * 旋转前：             旋转后：
     *        node                    y
     *       /    \                 /   \
     *      y      A               C     node
     *     / \                    / \    / \
     *    C   B                  D   E  B   A
     *   / \
     *  D   E
     */
    Node* rotateRight(Node* node) {
        Node* y = node->left;
        node->left = y->right;
        y->right = node;
        
        updateHeight(node);
        updateHeight(y);
        return y;
    }
    
    /**
     * @brief 左旋（RR旋转）
     * @param node 失衡节点
     * @return Node* 新的根节点
     * 
     * 旋转前：             旋转后：
     *      node                    y
     *     /    \                 /   \
     *    A      y               node  C
     *          / \              / \
     *         B   C            A   B
     *            / \
     *           D   E
     */
    Node* rotateLeft(Node* node) {
        Node* y = node->right;
        node->right = y->left;
        y->left = node;
        
        updateHeight(node);
        updateHeight(y);
        return y;
    }
    
    /**
     * @brief 递归插入
     * @param node 当前子树根节点
     * @param val 要插入的值
     * @return Node* 新的子树根节点
     */
    Node* insert(Node* node, const T& val) {
        if (!node) {
            ++_size;
            return new Node(val);
        }
        
        if (val < node->data) {
            node->left = insert(node->left, val);
            // 检查平衡
            if (balanceFactor(node) == 2) {
                if (balanceFactor(node->left) >= 0) {
                    // LL型：右旋
                    node = rotateRight(node);
                } else {
                    // LR型：先左旋再右旋
                    node->left = rotateLeft(node->left);
                    node = rotateRight(node);
                }
            }
        } else if (val > node->data) {
            node->right = insert(node->right, val);
            if (balanceFactor(node) == -2) {
                if (balanceFactor(node->right) <= 0) {
                    // RR型：左旋
                    node = rotateLeft(node);
                } else {
                    // RL型：先右旋再左旋
                    node->right = rotateRight(node->right);
                    node = rotateLeft(node);
                }
            }
        } else {
            // 元素已存在
            throw std::invalid_argument("Element already exists");
        }
        
        updateHeight(node);
        return node;
    }
    
    /**
     * @brief 查找最小节点
     * @param node 子树根节点
     * @return Node* 最小节点指针
     */
    Node* findMin(Node* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }
    
    /**
     * @brief 递归删除
     * @param node 当前子树根节点
     * @param val 要删除的值
     * @return Node* 新的子树根节点
     */
    Node* erase(Node* node, const T& val) {
        if (!node) return nullptr;
        
        if (val < node->data) {
            node->left = erase(node->left, val);
            if (balanceFactor(node) == -2) {
                if (balanceFactor(node->right) <= 0) {
                    node = rotateLeft(node);
                } else {
                    node->right = rotateRight(node->right);
                    node = rotateLeft(node);
                }
            }
        } else if (val > node->data) {
            node->right = erase(node->right, val);
            if (balanceFactor(node) == 2) {
                if (balanceFactor(node->left) >= 0) {
                    node = rotateRight(node);
                } else {
                    node->left = rotateLeft(node->left);
                    node = rotateRight(node);
                }
            }
        } else {
            // 找到要删除的节点
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                --_size;
                return temp;
            } else {
                Node* temp = findMin(node->right);
                node->data = temp->data;
                node->right = erase(node->right, temp->data);
                if (balanceFactor(node) == 2) {
                    if (balanceFactor(node->left) >= 0) {
                        node = rotateRight(node);
                    } else {
                        node->left = rotateLeft(node->left);
                        node = rotateRight(node);
                    }
                }
            }
        }
        
        if (node) updateHeight(node);
        return node;
    }
    
    /**
     * @brief 递归查找
     * @param node 当前子树根节点
     * @param val 要查找的值
     * @return Node* 找到的节点指针，未找到返回nullptr
     */
    Node* search(Node* node, const T& val) const {
        if (!node || node->data == val) return node;
        if (val < node->data) return search(node->left, val);
        return search(node->right, val);
    }
    
    /**
     * @brief 递归中序遍历
     * @param node 当前子树根节点
     */
    void inOrder(Node* node, std::ostream& os = std::cout) const {
        if (node) {
            inOrder(node->left, os);
            os << node->data << "(h=" << node->height << ") ";
            inOrder(node->right, os);
        }
    }
    
    /**
     * @brief 递归拷贝
     * @param node 要拷贝的节点
     * @return Node* 拷贝后的节点
     */
    Node* copyTree(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->data);
        newNode->height = node->height;
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }
    
    /**
     * @brief 递归销毁树
     * @param node 要销毁的节点
     */
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空AVL树
     */
    AVLTree() : _root(nullptr), _size(0) {}
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    AVLTree(std::initializer_list<T> ilist) : _root(nullptr), _size(0) {
        for (const T& val : ilist) {
            insert(val);
        }
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 释放所有节点内存
     */
    ~AVLTree() {
        destroyTree(_root);
    }
    
    // ---- 拷贝控制（五法则） ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    AVLTree(const AVLTree& other) : _root(nullptr), _size(other._size) {
        _root = copyTree(other._root);
    }
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    AVLTree(AVLTree&& other) noexcept 
        : _root(other._root), _size(other._size) {
        other._root = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return AVLTree&
     * 时间复杂度：O(n)
     */
    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            AVLTree tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return AVLTree&
     * 时间复杂度：O(1)
     */
    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this != &other) {
            this->~AVLTree();
            _root = other._root;
            _size = other._size;
            other._root = nullptr;
            other._size = 0;
        }
        return *this;
    }
    
    /**
     * @brief 交换两个树
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(AVLTree& other) {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
    }
    
    // ---- 容量 ----
    
    /**
     * @brief 返回元素个数
     * @return std::size_t
     * 时间复杂度：O(1)
     */
    std::size_t size() const { return _size; }
    
    /**
     * @brief 判断是否为空
     * @return bool
     * 时间复杂度：O(1)
     */
    bool empty() const { return _size == 0; }
    
    /**
     * @brief 获取树的高度
     * @return int
     * 时间复杂度：O(1)
     */
    int height() const { return getHeight(_root); }
    
    // ---- 插入操作 ----
    
    /**
     * @brief 插入元素（拷贝版本）
     * @param val 要插入的值
     * @throws std::invalid_argument 如果元素已存在
     * 时间复杂度：O(logn)
     */
    void insert(const T& val) {
        _root = insert(_root, val);
    }
    
    /**
     * @brief 插入元素（移动版本）
     * @param val 要插入的值（右值引用）
     * @throws std::invalid_argument 如果元素已存在
     * 时间复杂度：O(logn)
     */
    void insert(T&& val) {
        _root = insert(_root, std::move(val));
    }
    
    // ---- 删除操作 ----
    
    /**
     * @brief 删除元素
     * @param val 要删除的值
     * @return bool 是否成功删除
     * 时间复杂度：O(logn)
     */
    bool erase(const T& val) {
        std::size_t oldSize = _size;
        _root = erase(_root, val);
        return _size < oldSize;
    }
    
    // ---- 查找操作 ----
    
    /**
     * @brief 查找元素
     * @param val 要查找的值
     * @return bool 是否存在
     * 时间复杂度：O(logn)
     */
    bool contains(const T& val) const {
        return search(_root, val) != nullptr;
    }
    
    /**
     * @brief 查找并返回元素引用
     * @param val 要查找的值
     * @return const T& 元素引用
     * @throws std::out_of_range 如果元素不存在
     * 时间复杂度：O(logn)
     */
    const T& at(const T& val) const {
        Node* node = search(_root, val);
        if (!node) {
            throw std::out_of_range("Element not found");
        }
        return node->data;
    }
    
    // ---- 遍历 ----
    
    /**
     * @brief 中序遍历输出
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void print(std::ostream& os = std::cout) const {
        inOrder(_root, os);
    }
    
    // ---- 清空 ----
    
    /**
     * @brief 清空所有元素
     * 时间复杂度：O(n)
     */
    void clear() {
        destroyTree(_root);
        _root = nullptr;
        _size = 0;
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(AVLTree<T>& a, AVLTree<T>& b) {
    a.swap(b);
}

#endif // AVL_TREE_HPP