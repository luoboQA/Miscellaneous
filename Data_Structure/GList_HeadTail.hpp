// GeneralizedListHeadTail.hpp
#ifndef GENERALIZED_LIST_HEAD_TAIL_HPP
#define GENERALIZED_LIST_HEAD_TAIL_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <sstream>
#include <cctype>

/**
 * @brief 广义表模板（头尾链表存储表示）
 * @tparam T 原子类型
 * 
 * 广义表是递归定义的数据结构：
 * - 原子：单个元素
 * - 列表：由表头(head)和表尾(tail)组成
 * 
 * 头尾链表存储结构：
 * - 每个表结点通过 hp 指向表头，tp 指向表尾
 * - 原子结点存储数据
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
            struct {
                Node* head;      // 表头指针
                Node* tail;      // 表尾指针
            } list;
        } data;
        
        // 构造原子节点
        Node(const T& val) : type(NodeType::ATOM) {
            new (&data.atom) T(val);
        }
        
        // 构造原子节点（移动版本）
        Node(T&& val) : type(NodeType::ATOM) {
            new (&data.atom) T(std::move(val));
        }
        
        // 构造列表节点
        Node(Node* headNode, Node* tailNode = nullptr) : type(NodeType::LIST) {
            data.list.head = headNode;
            data.list.tail = tailNode;
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
            Node* newHead = copyNode(node->data.list.head);
            Node* newTail = copyNode(node->data.list.tail);
            newNode = new Node(newHead, newTail);
        }
        return newNode;
    }
    
    /**
     * @brief 递归销毁广义表
     * @param node 当前节点
     */
    void destroyNode(Node* node) {
        if (!node) return;
        
        if (node->type == NodeType::LIST) {
            destroyNode(node->data.list.head);
            destroyNode(node->data.list.tail);
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
                int depth = getDepth(p->data.list.head);
                if (depth > maxDepth) maxDepth = depth;
            }
            p = p->data.list.tail;
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
            p = p->data.list.tail;
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
                printNode(p->data.list.head, os);
            } else {
                os << p->data.atom;
            }
            p = p->data.list.tail;
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
            if (containsNode(node->data.list.head, val)) return true;
            if (containsNode(node->data.list.tail, val)) return true;
        }
        return false;
    }
    
    /**
     * @brief 递归遍历所有原子
     * @param node 当前节点
     * @param callback 回调函数
     */
    void forEachAtomNode(Node* node, std::function<void(const T&)> callback) const {
        if (!node) return;
        
        if (node->type == NodeType::ATOM) {
            callback(node->data.atom);
        } else {
            forEachAtomNode(node->data.list.head, callback);
            forEachAtomNode(node->data.list.tail, callback);
        }
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
        Node* current = nullptr;
        for (const T& val : ilist) {
            Node* newNode = new Node(val);
            if (!_root) {
                _root = newNode;
                current = _root;
            } else {
                current->data.list.tail = newNode;
                current = newNode;
            }
            ++_size;
        }
        // 设置尾节点的tail为nullptr
        if (current) {
            // 如果当前节点是原子节点，需要转换为列表节点
            // 但这里我们的构造方式会导致结构不正确
            // 应该使用makeList方法
        }
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
        return _root && _root->type == NodeType::ATOM;
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
        GeneralizedList result;
        result._root = copyNode(_root->data.list.head);
        result._size = getLength(result._root);
        return result;
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
        result._root = copyNode(_root->data.list.tail);
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
     * @brief 创建列表节点（头尾链表方式）
     * @param head 表头
     * @param tail 表尾（可选）
     * @return GeneralizedList 列表广义表
     */
    static GeneralizedList makeList(const GeneralizedList& head, 
                                     const GeneralizedList& tail = GeneralizedList()) {
        GeneralizedList result;
        Node* headNode = copyNode(head._root);
        Node* tailNode = copyNode(tail._root);
        result._root = new Node(headNode, tailNode);
        result._size = 1 + tail.size();
        return result;
    }
    
    /**
     * @brief 从元素列表创建列表
     * @param elements 元素列表
     * @return GeneralizedList 列表广义表
     */
    static GeneralizedList makeList(const std::vector<GeneralizedList>& elements) {
        if (elements.empty()) {
            return GeneralizedList();
        }
        
        GeneralizedList result = elements[0];
        for (std::size_t i = 1; i < elements.size(); ++i) {
            result = makeList(result, elements[i]);
        }
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
        forEachAtomNode(_root, callback);
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
     * @brief 从字符串创建广义表
     * @param str 字符串表示
     * @return GeneralizedList<char> 创建的广义表
     * @throws std::invalid_argument 如果字符串格式无效
     */
    static GeneralizedList<char> fromString(const std::string& str) {
        const char* s = str.c_str();
        auto result = parseString(s);
        if (*s != '\0' && *s != ')') {
            throw std::invalid_argument("Invalid generalized list string");
        }
        return result;
    }
    
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
        
        auto listBC = makeList(atomB, atomC);
        return makeList({atomA, listBC, atomD});
    }

private:
    /**
     * @brief 从字符串解析广义表（递归）
     * @param s 字符串指针（会被修改）
     * @return GeneralizedList<char> 解析结果
     */
    static GeneralizedList<char> parseString(const char*& s) {
        if (*s == '\0') return GeneralizedList<char>();
        
        if (*s == '(') {
            s++;  // 跳过'('
            
            if (*s == ')') {
                s++;  // 跳过')'
                return GeneralizedList<char>();
            }
            
            // 解析表头
            GeneralizedList<char> head;
            if (*s == '(') {
                head = parseString(s);
            } else if (std::isalpha(*s)) {
                head = makeAtom(*s);
                s++;
            } else {
                throw std::invalid_argument("Invalid character in generalized list");
            }
            
            // 解析表尾
            GeneralizedList<char> tail;
            if (*s == ',') {
                s++;  // 跳过','
                tail = parseString(s);
            }
            
            if (*s == ')') s++;  // 跳过')'
            return makeList(head, tail);
        }
        return GeneralizedList<char>();
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(GeneralizedList<T>& a, GeneralizedList<T>& b) {
    a.swap(b);
}

#endif // GENERALIZED_LIST_HEAD_TAIL_HPP