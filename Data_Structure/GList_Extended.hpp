// GeneralizedList.hpp
#ifndef GENERALIZED_LIST_HPP
#define GENERALIZED_LIST_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <sstream>

/**
 * @brief 广义表模板（扩展线性链表存储表示）
 * @tparam T 原子类型
 * 
 * 广义表是递归定义的数据结构：
 * - 原子：单个元素
 * - 列表：由多个元素组成，每个元素可以是原子或子表
 * 
 * 时间复杂度：
 * - 求长度/深度：O(n)
 * - 复制：O(n)
 * - 销毁：O(n)
 * - 空间复杂度：O(n)
 */
template<typename T>
class GeneralizedList {
private:
    // 节点类型枚举
    enum class NodeType { ATOM, LIST };
    
    struct Node {
        NodeType type;
        union {
            T atom;              // 原子值
            Node* head;          // 子表头指针
        } data;
        Node* next;              // 下一个兄弟节点
        
        // 构造原子节点
        Node(const T& val) : type(NodeType::ATOM), next(nullptr) {
            new (&data.atom) T(val);
        }
        
        // 构造原子节点（移动版本）
        Node(T&& val) : type(NodeType::ATOM), next(nullptr) {
            new (&data.atom) T(std::move(val));
        }
        
        // 构造列表节点
        Node(Node* headNode, Node* nextNode = nullptr) 
            : type(NodeType::LIST), next(nextNode) {
            data.head = headNode;
        }
        
        // 析构节点
        ~Node() {
            if (type == NodeType::ATOM) {
                data.atom.~T();
            }
        }
        
        // 禁止拷贝
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };
    
    Node* _root;          // 根节点指针
    std::size_t _size;    // 元素个数（顶层元素）
    
    /**
     * @brief 递归复制广义表
     * @param node 当前节点
     * @return Node* 复制后的节点
     */
    Node* copyNode(Node* node) {
        if (!node) return nullptr;
        
        Node* newNode = nullptr;
        if (node->type == NodeType::ATOM) {
            newNode = new Node(node->data.atom);
        } else {
            Node* newHead = copyNode(node->data.head);
            newNode = new Node(newHead);
        }
        newNode->next = copyNode(node->next);
        return newNode;
    }
    
    /**
     * @brief 递归销毁广义表
     * @param node 当前节点
     */
    void destroyNode(Node* node) {
        if (!node) return;
        
        destroyNode(node->next);
        if (node->type == NodeType::LIST) {
            destroyNode(node->data.head);
        }
        delete node;
    }
    
    /**
     * @brief 递归计算深度
     * @param node 当前节点
     * @return int 深度
     */
    int getDepth(Node* node) const {
        if (!node) return 1;
        if (node->type == NodeType::ATOM) return 0;
        
        int maxDepth = 0;
        Node* p = node;
        while (p) {
            if (p->type == NodeType::LIST) {
                int depth = getDepth(p->data.head);
                if (depth > maxDepth) maxDepth = depth;
            }
            p = p->next;
        }
        return maxDepth + 1;
    }
    
    /**
     * @brief 递归计算长度
     * @param node 当前节点
     * @return int 长度
     */
    int getLength(Node* node) const {
        if (!node) return 0;
        if (node->type == NodeType::ATOM) return 1;
        
        int len = 0;
        Node* p = node;
        while (p) {
            len++;
            p = p->next;
        }
        return len;
    }
    
    /**
     * @brief 递归打印广义表
     * @param node 当前节点
     * @param os 输出流
     */
    void printNode(Node* node, std::ostream& os) const {
        if (!node) {
            os << "()";
            return;
        }
        
        if (node->type == NodeType::ATOM) {
            os << node->data.atom;
            return;
        }
        
        os << "(";
        Node* p = node;
        bool first = true;
        while (p) {
            if (!first) os << ",";
            first = false;
            
            if (p->type == NodeType::LIST) {
                printNode(p->data.head, os);
            } else {
                os << p->data.atom;
            }
            p = p->next;
        }
        os << ")";
    }
    
    /**
     * @brief 递归转换为字符串
     * @param node 当前节点
     * @return std::string 字符串表示
     */
    std::string toString(Node* node) const {
        std::ostringstream oss;
        printNode(node, oss);
        return oss.str();
    }
    
    /**
     * @brief 递归检查是否包含某个值
     * @param node 当前节点
     * @param val 要查找的值
     * @return bool 是否包含
     */
    bool containsNode(Node* node, const T& val) const {
        if (!node) return false;
        
        if (node->type == NodeType::ATOM) {
            if (node->data.atom == val) return true;
        } else {
            if (containsNode(node->data.head, val)) return true;
        }
        return containsNode(node->next, val);
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     * 创建空广义表
     */
    GeneralizedList() : _root(nullptr), _size(0) {}
    
    /**
     * @brief 从原子构造
     * @param val 原子值
     */
    explicit GeneralizedList(const T& val) : _root(nullptr), _size(1) {
        _root = new Node(val);
    }
    
    /**
     * @brief 从原子构造（移动版本）
     * @param val 原子值（右值引用）
     */
    explicit GeneralizedList(T&& val) : _root(nullptr), _size(1) {
        _root = new Node(std::move(val));
    }
    
    /**
     * @brief 从初始化列表构造
     * @param ilist 初始化列表
     */
    GeneralizedList(std::initializer_list<T> ilist) : _root(nullptr), _size(0) {
        Node** current = &_root;
        for (const T& val : ilist) {
            *current = new Node(val);
            current = &((*current)->next);
            ++_size;
        }
    }
    
    /**
     * @brief 从子表构造
     * @param subList 子表
     */
    explicit GeneralizedList(const GeneralizedList& subList) : _root(nullptr), _size(1) {
        if (!subList._root) {
            throw std::invalid_argument("Cannot create list from empty sublist");
        }
        _root = new Node(copyNode(subList._root));
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     */
    ~GeneralizedList() {
        destroyNode(_root);
    }
    
    // ---- 拷贝控制 ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     */
    GeneralizedList(const GeneralizedList& other) 
        : _root(copyNode(other._root)), _size(other._size) {}
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     */
    GeneralizedList(GeneralizedList&& other) noexcept 
        : _root(other._root), _size(other._size) {
        other._root = nullptr;
        other._size = 0;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return GeneralizedList&
     */
    GeneralizedList& operator=(const GeneralizedList& other) {
        if (this != &other) {
            GeneralizedList tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return GeneralizedList&
     */
    GeneralizedList& operator=(GeneralizedList&& other) noexcept {
        if (this != &other) {
            this->~GeneralizedList();
            _root = other._root;
            _size = other._size;
            other._root = nullptr;
            other._size = 0;
        }
        return *this;
    }
    
    /**
     * @brief 交换两个广义表
     * @param other 要交换的对象
     */
    void swap(GeneralizedList& other) {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
    }
    
    // ---- 容量 ----
    
    /**
     * @brief 返回顶层元素个数
     * @return std::size_t
     */
    std::size_t size() const { return _size; }
    
    /**
     * @brief 判断是否为空
     * @return bool
     */
    bool empty() const { return _root == nullptr; }
    
    /**
     * @brief 获取广义表的深度
     * @return int
     */
    int depth() const { return getDepth(_root); }
    
    /**
     * @brief 获取广义表的长度（同size）
     * @return int
     */
    int length() const { return getLength(_root); }
    
    // ---- 访问 ----
    
    /**
     * @brief 判断是否为原子
     * @return bool
     */
    bool isAtom() const {
        return _root && _root->type == NodeType::ATOM && _root->next == nullptr;
    }
    
    /**
     * @brief 判断是否为列表
     * @return bool
     */
    bool isList() const {
        return !isAtom() && !empty();
    }
    
    /**
     * @brief 获取原子值
     * @return const T& 原子引用
     * @throws std::runtime_error 如果不是原子
     */
    const T& getAtom() const {
        if (!isAtom()) {
            throw std::runtime_error("Not an atom");
        }
        return _root->data.atom;
    }
    
    /**
     * @brief 获取表头
     * @return GeneralizedList 表头
     * @throws std::runtime_error 如果是原子或空表
     */
    GeneralizedList head() const {
        if (empty() || isAtom()) {
            throw std::runtime_error("Cannot get head of atom or empty list");
        }
        if (_root->type == NodeType::ATOM) {
            return GeneralizedList(_root->data.atom);
        }
        return GeneralizedList(_root->data.head);
    }
    
    /**
     * @brief 获取表尾
     * @return GeneralizedList 表尾
     * @throws std::runtime_error 如果是原子或空表
     */
    GeneralizedList tail() const {
        if (empty() || isAtom()) {
            throw std::runtime_error("Cannot get tail of atom or empty list");
        }
        GeneralizedList result;
        result._root = copyNode(_root->next);
        result._size = getLength(result._root);
        return result;
    }
    
    // ---- 构建操作 ----
    
    /**
     * @brief 创建原子节点
     * @param val 原子值
     * @return GeneralizedList 原子广义表
     */
    static GeneralizedList makeAtom(const T& val) {
        return GeneralizedList(val);
    }
    
    /**
     * @brief 创建列表节点
     * @param elements 元素列表
     * @return GeneralizedList 列表广义表
     */
    static GeneralizedList makeList(const std::vector<GeneralizedList>& elements) {
        GeneralizedList result;
        Node** current = &result._root;
        for (const auto& elem : elements) {
            if (elem.isAtom()) {
                *current = new Node(elem._root->data.atom);
            } else {
                *current = new Node(copyNode(elem._root));
            }
            current = &((*current)->next);
            ++result._size;
        }
        return result;
    }
    
    /**
     * @brief 追加元素到顶层
     * @param elem 要追加的元素
     */
    void append(const GeneralizedList& elem) {
        Node** current = &_root;
        while (*current) {
            current = &((*current)->next);
        }
        
        if (elem.isAtom()) {
            *current = new Node(elem._root->data.atom);
        } else {
            *current = new Node(copyNode(elem._root));
        }
        ++_size;
    }
    
    /**
     * @brief 在顶层插入元素
     * @param pos 位置（从0开始）
     * @param elem 要插入的元素
     * @throws std::out_of_range 如果位置无效
     */
    void insert(std::size_t pos, const GeneralizedList& elem) {
        if (pos > _size) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* newNode = nullptr;
        if (elem.isAtom()) {
            newNode = new Node(elem._root->data.atom);
        } else {
            newNode = new Node(copyNode(elem._root));
        }
        
        if (pos == 0) {
            newNode->next = _root;
            _root = newNode;
        } else {
            Node* p = _root;
            for (std::size_t i = 0; i < pos - 1; ++i) {
                p = p->next;
            }
            newNode->next = p->next;
            p->next = newNode;
        }
        ++_size;
    }
    
    /**
     * @brief 删除顶层元素
     * @param pos 位置（从0开始）
     * @return GeneralizedList 被删除的元素
     * @throws std::out_of_range 如果位置无效
     */
    GeneralizedList erase(std::size_t pos) {
        if (pos >= _size) {
            throw std::out_of_range("Position out of range");
        }
        
        Node* toDelete = nullptr;
        if (pos == 0) {
            toDelete = _root;
            _root = _root->next;
        } else {
            Node* p = _root;
            for (std::size_t i = 0; i < pos - 1; ++i) {
                p = p->next;
            }
            toDelete = p->next;
            p->next = toDelete->next;
        }
        
        GeneralizedList result;
        if (toDelete->type == NodeType::ATOM) {
            result._root = new Node(toDelete->data.atom);
        } else {
            result._root = new Node(copyNode(toDelete->data.head));
        }
        result._size = 1;
        
        toDelete->next = nullptr;
        if (toDelete->type == NodeType::LIST) {
            // 避免删除子表节点
            toDelete->data.head = nullptr;
        }
        delete toDelete;
        --_size;
        
        return result;
    }
    
    // ---- 查找 ----
    
    /**
     * @brief 查找元素（递归）
     * @param val 要查找的值
     * @return bool 是否包含
     */
    bool contains(const T& val) const {
        return containsNode(_root, val);
    }
    
    // ---- 遍历 ----
    
    /**
     * @brief 打印广义表
     * @param os 输出流
     */
    void print(std::ostream& os = std::cout) const {
        if (empty()) {
            os << "()";
        } else {
            printNode(_root, os);
        }
    }
    
    /**
     * @brief 转换为字符串
     * @return std::string
     */
    std::string toString() const {
        if (empty()) {
            return "()";
        }
        return toString(_root);
    }
    
    /**
     * @brief 遍历所有原子（深度优先）
     * @param callback 回调函数
     */
    void forEachAtom(std::function<void(const T&)> callback) const {
        std::function<void(Node*)> traverse = [&](Node* node) {
            if (!node) return;
            if (node->type == NodeType::ATOM) {
                callback(node->data.atom);
            } else {
                traverse(node->data.head);
            }
            traverse(node->next);
        };
        traverse(_root);
    }
    
    /**
     * @brief 遍历顶层元素
     * @param callback 回调函数
     */
    void forEach(std::function<void(const GeneralizedList&)> callback) const {
        Node* p = _root;
        while (p) {
            GeneralizedList elem;
            if (p->type == NodeType::ATOM) {
                elem._root = new Node(p->data.atom);
            } else {
                elem._root = new Node(copyNode(p->data.head));
            }
            elem._size = 1;
            callback(elem);
            p = p->next;
        }
    }
    
    // ---- 操作 ----
    
    /**
     * @brief 清空广义表
     */
    void clear() {
        destroyNode(_root);
        _root = nullptr;
        _size = 0;
    }
    
    // ---- 工厂方法 ----
    
    /**
     * @brief 创建示例广义表
     * @return GeneralizedList<char>
     */
    static GeneralizedList<char> createSample() {
        // 创建广义表: (a,(b,c),d)
        auto atomA = makeAtom('a');
        auto atomB = makeAtom('b');
        auto atomC = makeAtom('c');
        auto atomD = makeAtom('d');
        
        auto listBC = makeList({atomB, atomC});
        return makeList({atomA, listBC, atomD});
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(GeneralizedList<T>& a, GeneralizedList<T>& b) {
    a.swap(b);
}

#endif // GENERALIZED_LIST_HPP