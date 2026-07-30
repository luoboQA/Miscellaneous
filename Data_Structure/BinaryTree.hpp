// BinaryTree.hpp
#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>
#include <functional>

/**
 * @brief 二叉树模板
 * @tparam T 元素类型
 * 
 * 时间复杂度：
 * - 遍历：O(n)
 * - 查找：O(n)（最坏）
 * - 空间复杂度：O(n)
 */
template<typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
        Node(T&& val) : data(std::move(val)), left(nullptr), right(nullptr) {}
    };
    
    Node* _root; 
    std::size_t _size;
    
    // ---- 辅助函数 ----
    
    /**
     * @brief 递归拷贝子树
     * @param node 要拷贝的节点
     * @return Node* 拷贝后的节点
     */
    Node* copyTree(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->data);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }
    
    /**
     * @brief 递归销毁子树
     * @param node 要销毁的节点
     */
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
    /**
     * @brief 递归前序遍历
     * @param node 当前节点
     * @param os 输出流
     * @param callback 回调函数（可选）
     */
    void preOrder(Node* node, std::ostream& os, 
                  std::function<void(const T&)> callback = nullptr) const {
        if (node) {
            if (callback) callback(node->data);
            else os << node->data << " ";
            preOrder(node->left, os, callback);
            preOrder(node->right, os, callback);
        }
    }
    
    /**
     * @brief 递归中序遍历
     * @param node 当前节点
     * @param os 输出流
     * @param callback 回调函数（可选）
     */
    void inOrder(Node* node, std::ostream& os,
                 std::function<void(const T&)> callback = nullptr) const {
        if (node) {
            inOrder(node->left, os, callback);
            if (callback) callback(node->data);
            else os << node->data << " ";
            inOrder(node->right, os, callback);
        }
    }
    
    /**
     * @brief 递归后序遍历
     * @param node 当前节点
     * @param os 输出流
     * @param callback 回调函数（可选）
     */
    void postOrder(Node* node, std::ostream& os,
                   std::function<void(const T&)> callback = nullptr) const {
        if (node) {
            postOrder(node->left, os, callback);
            postOrder(node->right, os, callback);
            if (callback) callback(node->data);
            else os << node->data << " ";
        }
    }
    
    /**
     * @brief 递归查找节点
     * @param node 当前节点
     * @param val 要查找的值
     * @return Node* 找到的节点指针
     */
    Node* findNode(Node* node, const T& val) const {
        if (!node) return nullptr;
        if (node->data == val) return node;
        
        Node* found = findNode(node->left, val);
        if (found) return found;
        return findNode(node->right, val);
    }
    
    /**
     * @brief 递归获取深度
     * @param node 当前节点
     * @return int 子树深度
     */
    int getDepth(Node* node) const {
        if (!node) return 0;
        int leftDepth = getDepth(node->left);
        int rightDepth = getDepth(node->right);
        return (leftDepth > rightDepth) ? leftDepth + 1 : rightDepth + 1;
    }
    
    /**
     * @brief 递归获取叶子节点数
     * @param node 当前节点
     * @return std::size_t 叶子节点数
     */
    std::size_t getLeafCount(Node* node) const {
        if (!node) return 0;
        if (!node->left && !node->right) return 1;
        return getLeafCount(node->left) + getLeafCount(node->right);
    }
    
    /**
     * @brief 递归获取度为2的节点数
     * @param node 当前节点
     * @return std::size_t 度为2的节点数
     */
    std::size_t getDegree2Count(Node* node) const {
        if (!node) return 0;
        std::size_t count = (node->left && node->right) ? 1 : 0;
        return count + getDegree2Count(node->left) + getDegree2Count(node->right);
    }
    
    /**
     * @brief 递归获取节点总数
     * @param node 当前节点
     * @return std::size_t 节点总数
     */
    std::size_t getNodeCount(Node* node) const {
        if (!node) return 0;
        return 1 + getNodeCount(node->left) + getNodeCount(node->right);
    }
    
    /**
     * @brief 递归交换左右子树
     * @param node 当前节点
     */
    void swapTree(Node* node) {
        if (!node) return;
        std::swap(node->left, node->right);
        swapTree(node->left);
        swapTree(node->right);
    }
    
    /**
     * @brief 递归判断是否满二叉树
     * @param node 当前节点
     * @return bool 是否满二叉树
     */
    bool isFullBinaryTree(Node* node) const {
        if (!node) return true;
        if (node->left && !node->right) return false;
        if (!node->left && node->right) return false;
        return isFullBinaryTree(node->left) && isFullBinaryTree(node->right);
    }
    
    /**
     * @brief 判断是否完全二叉树
     * @param node 根节点
     * @return bool 是否完全二叉树
     */
    bool isCompleteBinaryTree(Node* node) const {
        if (!node) return true;
        
        std::queue<Node*> q;
        q.push(node);
        bool hasNullChild = false;
        
        while (!q.empty()) {
            Node* p = q.front();
            q.pop();
            
            if (!p) {
                hasNullChild = true;
            } else {
                if (hasNullChild) return false;
                q.push(p->left);
                q.push(p->right);
            }
        }
        return true;
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空二叉树
     */
    BinaryTree() : _root(nullptr), _size(0) {}
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表（前序遍历顺序）
     */
    BinaryTree(std::initializer_list<T> ilist) : _root(nullptr), _size(0) {
        // 注意：初始化列表构造需要特定顺序，这里仅作演示
        for (const T& val : ilist) {
            // 简化实现：不支持从普通初始化列表直接构造
            // 建议使用 BuildFromTraversal 方法
        }
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     * 释放所有节点内存
     */
    ~BinaryTree() {
        destroyTree(_root);
    }
    
    // ---- 拷贝控制（五法则） ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     * 时间复杂度：O(n)
     */
    BinaryTree(const BinaryTree& other) : _root(nullptr), _size(other._size) {
        _root = copyTree(other._root);
    }
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     * 时间复杂度：O(1)
     */
    BinaryTree(BinaryTree&& other) noexcept 
        : _root(other._root), _size(other._size) {
        other._root = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return BinaryTree&
     * 时间复杂度：O(n)
     */
    BinaryTree& operator=(const BinaryTree& other) {
        if (this != &other) {
            BinaryTree tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return BinaryTree&
     * 时间复杂度：O(1)
     */
    BinaryTree& operator=(BinaryTree&& other) noexcept {
        if (this != &other) {
            this->~BinaryTree();
            _root = other._root;
            _size = other._size;
            other._root = nullptr;
            other._size = 0;
        }
        return *this;
    }
    
    /**
     * @brief 交换两棵树
     * @param other 要交换的对象
     * 时间复杂度：O(1)
     */
    void swap(BinaryTree& other) {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
    }
    
    // ---- 容量 ----
    
    /**
     * @brief 返回节点个数
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
     * 时间复杂度：O(n)
     */
    int height() const { return getDepth(_root); }
    
    /**
     * @brief 获取叶子节点数
     * @return std::size_t
     * 时间复杂度：O(n)
     */
    std::size_t leafCount() const { return getLeafCount(_root); }
    
    /**
     * @brief 获取度为2的节点数
     * @return std::size_t
     * 时间复杂度：O(n)
     */
    std::size_t degree2Count() const { return getDegree2Count(_root); }
    
    /**
     * @brief 获取节点总数（同 size）
     * @return std::size_t
     * 时间复杂度：O(n)
     */
    std::size_t nodeCount() const { return getNodeCount(_root); }
    
    // ---- 遍历 ----
    
    /**
     * @brief 前序遍历（递归）
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void preOrder(std::ostream& os = std::cout) const {
        preOrder(_root, os);
    }
    
    /**
     * @brief 中序遍历（递归）
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void inOrder(std::ostream& os = std::cout) const {
        inOrder(_root, os);
    }
    
    /**
     * @brief 后序遍历（递归）
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void postOrder(std::ostream& os = std::cout) const {
        postOrder(_root, os);
    }
    
    /**
     * @brief 前序遍历（非递归）
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void preOrderNonRecursive(std::ostream& os = std::cout) const {
        if (!_root) return;
        
        std::stack<Node*> s;
        Node* p = _root;
        
        while (p || !s.empty()) {
            while (p) {
                os << p->data << " ";
                s.push(p);
                p = p->left;
            }
            if (!s.empty()) {
                p = s.top();
                s.pop();
                p = p->right;
            }
        }
    }
    
    /**
     * @brief 中序遍历（非递归）
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void inOrderNonRecursive(std::ostream& os = std::cout) const {
        if (!_root) return;
        
        std::stack<Node*> s;
        Node* p = _root;
        
        while (p || !s.empty()) {
            while (p) {
                s.push(p);
                p = p->left;
            }
            if (!s.empty()) {
                p = s.top();
                s.pop();
                os << p->data << " ";
                p = p->right;
            }
        }
    }
    
    /**
     * @brief 后序遍历（非递归）
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void postOrderNonRecursive(std::ostream& os = std::cout) const {
        if (!_root) return;
        
        std::stack<Node*> s1, s2;
        s1.push(_root);
        
        while (!s1.empty()) {
            Node* p = s1.top();
            s1.pop();
            s2.push(p);
            if (p->left) s1.push(p->left);
            if (p->right) s1.push(p->right);
        }
        
        while (!s2.empty()) {
            os << s2.top()->data << " ";
            s2.pop();
        }
    }
    
    /**
     * @brief 层次遍历
     * @param os 输出流
     * 时间复杂度：O(n)
     */
    void levelOrder(std::ostream& os = std::cout) const {
        if (!_root) return;
        
        std::queue<Node*> q;
        q.push(_root);
        
        while (!q.empty()) {
            Node* p = q.front();
            q.pop();
            os << p->data << " ";
            if (p->left) q.push(p->left);
            if (p->right) q.push(p->right);
        }
    }
    
    /**
     * @brief 使用回调函数遍历（前序）
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void preOrder(std::function<void(const T&)> callback) const {
        preOrder(_root, std::cout, callback);
    }
    
    /**
     * @brief 使用回调函数遍历（中序）
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void inOrder(std::function<void(const T&)> callback) const {
        inOrder(_root, std::cout, callback);
    }
    
    /**
     * @brief 使用回调函数遍历（后序）
     * @param callback 回调函数
     * 时间复杂度：O(n)
     */
    void postOrder(std::function<void(const T&)> callback) const {
        postOrder(_root, std::cout, callback);
    }
    
    // ---- 查找 ----
    
    /**
     * @brief 查找元素
     * @param val 要查找的值
     * @return bool 是否存在
     * 时间复杂度：O(n)
     */
    bool contains(const T& val) const {
        return findNode(_root, val) != nullptr;
    }
    
    /**
     * @brief 查找并返回元素引用
     * @param val 要查找的值
     * @return const T& 元素引用
     * @throws std::out_of_range 如果元素不存在
     * 时间复杂度：O(n)
     */
    const T& at(const T& val) const {
        Node* node = findNode(_root, val);
        if (!node) {
            throw std::out_of_range("Element not found");
        }
        return node->data;
    }
    
    // ---- 判断性质 ----
    
    /**
     * @brief 判断是否满二叉树
     * @return bool
     * 时间复杂度：O(n)
     */
    bool isFull() const {
        return isFullBinaryTree(_root);
    }
    
    /**
     * @brief 判断是否完全二叉树
     * @return bool
     * 时间复杂度：O(n)
     */
    bool isComplete() const {
        return isCompleteBinaryTree(_root);
    }
    
    /**
     * @brief 验证性质 n0 = n2 + 1
     * @return bool 是否满足性质
     * 时间复杂度：O(n)
     */
    bool verifyProperty() const {
        return leafCount() == degree2Count() + 1;
    }
    
    // ---- 操作 ----
    
    /**
     * @brief 交换左右子树
     * 时间复杂度：O(n)
     */
    void swapLeftRight() {
        swapTree(_root);
    }
    
    /**
     * @brief 清空所有元素
     * 时间复杂度：O(n)
     */
    void clear() {
        destroyTree(_root);
        _root = nullptr;
        _size = 0;
    }
    
    // ---- 构建 ----
    
    /**
     * @brief 从前序和中序遍历序列重建二叉树
     * @param pre 前序遍历序列
     * @param in 中序遍历序列
     * @return BinaryTree 重建的二叉树
     * @throws std::invalid_argument 如果序列无效
     * 时间复杂度：O(n)
     */
    static BinaryTree buildFromPreIn(const std::vector<T>& pre, 
                                      const std::vector<T>& in) {
        if (pre.empty() || in.empty()) return BinaryTree();
        if (pre.size() != in.size()) {
            throw std::invalid_argument("Preorder and inorder sizes do not match");
        }
        
        BinaryTree tree;
        tree._root = tree.buildFromPreIn(pre, in, 0, 0, pre.size());
        tree._size = pre.size();
        return tree;
    }
    
    /**
     * @brief 从中序和后序遍历序列重建二叉树
     * @param in 中序遍历序列
     * @param post 后序遍历序列
     * @return BinaryTree 重建的二叉树
     * @throws std::invalid_argument 如果序列无效
     * 时间复杂度：O(n)
     */
    static BinaryTree buildFromInPost(const std::vector<T>& in,
                                       const std::vector<T>& post) {
        if (in.empty() || post.empty()) return BinaryTree();
        if (in.size() != post.size()) {
            throw std::invalid_argument("Inorder and postorder sizes do not match");
        }
        
        BinaryTree tree;
        tree._root = tree.buildFromInPost(in, post, 0, 0, in.size());
        tree._size = in.size();
        return tree;
    }
    
    /**
     * @brief 创建满二叉树示例
     * @return BinaryTree 满二叉树
     */
    static BinaryTree createSampleTree() {
        BinaryTree tree;
        /*
                 A
               /   \
              B     C
             / \   / \
            D   E F   G
        */
        tree._root = new Node('A');
        tree._root->left = new Node('B');
        tree._root->right = new Node('C');
        tree._root->left->left = new Node('D');
        tree._root->left->right = new Node('E');
        tree._root->right->left = new Node('F');
        tree._root->right->right = new Node('G');
        tree._size = 7;
        return tree;
    }

private:
    // 辅助构建函数（前序+中序）
    Node* buildFromPreIn(const std::vector<T>& pre, const std::vector<T>& in,
                         int preStart, int inStart, int len) {
        if (len <= 0) return nullptr;
        
        Node* node = new Node(pre[preStart]);
        
        // 在中序中找到根的位置
        int rootIdx = inStart;
        while (rootIdx < inStart + len && in[rootIdx] != pre[preStart]) {
            rootIdx++;
        }
        if (rootIdx >= inStart + len) {
            throw std::invalid_argument("Invalid traversal sequence");
        }
        
        int leftLen = rootIdx - inStart;
        int rightLen = len - leftLen - 1;
        
        node->left = buildFromPreIn(pre, in, preStart + 1, inStart, leftLen);
        node->right = buildFromPreIn(pre, in, preStart + leftLen + 1, 
                                      rootIdx + 1, rightLen);
        
        return node;
    }
    
    // 辅助构建函数（中序+后序）
    Node* buildFromInPost(const std::vector<T>& in, const std::vector<T>& post,
                          int inStart, int postStart, int len) {
        if (len <= 0) return nullptr;
        
        Node* node = new Node(post[postStart + len - 1]);
        
        // 在中序中找到根的位置
        int rootIdx = inStart;
        while (rootIdx < inStart + len && in[rootIdx] != post[postStart + len - 1]) {
            rootIdx++;
        }
        if (rootIdx >= inStart + len) {
            throw std::invalid_argument("Invalid traversal sequence");
        }
        
        int leftLen = rootIdx - inStart;
        int rightLen = len - leftLen - 1;
        
        node->left = buildFromInPost(in, post, inStart, postStart, leftLen);
        node->right = buildFromInPost(in, post, inStart + leftLen + 1,
                                       postStart + leftLen, rightLen);
        
        return node;
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(BinaryTree<T>& a, BinaryTree<T>& b) {
    a.swap(b);
}

#endif // BINARY_TREE_HPP