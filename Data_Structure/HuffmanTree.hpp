// HuffmanTree.hpp
#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <functional>

/**
 * @brief 哈夫曼树模板
 * @tparam T 元素类型（通常为 char 或 int）
 * 
 * 哈夫曼树是一种带权路径长度最短的二叉树，用于数据压缩。
 * 
 * 特性：
 * - 使用最小堆选取权值最小的两个节点
 * - 生成前缀编码（无歧义解码）
 * - 支持编码和解码
 * 
 * 时间复杂度：
 * - 构建：O(nlogn)
 * - 编码：O(n)
 * - 解码：O(m)，m为编码长度
 * - 空间复杂度：O(n)
 */
template<typename T>
class HuffmanTree {
private:
    // 哈夫曼树节点
    struct Node {
        T data;              // 数据（叶子节点存储原始数据）
        int weight;          // 权值
        Node* left;
        Node* right;
        
        Node(const T& d, int w) 
            : data(d), weight(w), left(nullptr), right(nullptr) {}
        Node(int w) 
            : data(T()), weight(w), left(nullptr), right(nullptr) {}
        
        // 用于最小堆的比较
        struct Compare {
            bool operator()(const Node* a, const Node* b) const {
                return a->weight > b->weight;
            }
        };
        
        bool isLeaf() const {
            return left == nullptr && right == nullptr;
        }
    };
    
    Node* _root;                          // 根节点
    std::unordered_map<T, std::string> _codeTable;  // 编码表
    bool _built;                          // 是否已构建
    
    /**
     * @brief 递归销毁树
     * @param node 当前节点
     */
    void destroyTree(Node* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
    
    /**
     * @brief 递归拷贝树
     * @param node 当前节点
     * @return Node* 拷贝后的节点
     */
    Node* copyTree(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->data, node->weight);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }
    
    /**
     * @brief 递归生成编码表
     * @param node 当前节点
     * @param code 当前编码
     */
    void generateCodeTable(Node* node, const std::string& code) {
        if (!node) return;
        
        if (node->isLeaf()) {
            _codeTable[node->data] = code;
        } else {
            generateCodeTable(node->left, code + "0");
            generateCodeTable(node->right, code + "1");
        }
    }
    
    /**
     * @brief 递归打印树结构
     * @param node 当前节点
     * @param indent 缩进
     * @param os 输出流
     */
    void printTree(Node* node, const std::string& indent, std::ostream& os) const {
        if (!node) return;
        
        os << indent;
        if (node->isLeaf()) {
            os << "├─ " << node->data << " (" << node->weight << ")\n";
        } else {
            os << "├─ [内部节点] " << node->weight << "\n";
            printTree(node->left, indent + "│  ", os);
            printTree(node->right, indent + "│  ", os);
        }
    }

public:
    // ---- 构造函数 ----
    
    /**
     * @brief 默认构造函数
     */
    HuffmanTree() : _root(nullptr), _built(false) {}
    
    /**
     * @brief 从权值数组构建
     * @param data 数据数组
     * @param weights 权值数组
     * @param n 元素个数
     * @throws std::invalid_argument 如果数组为空
     */
    HuffmanTree(const T* data, const int* weights, std::size_t n) 
        : _root(nullptr), _built(false) {
        build(data, weights, n);
    }
    
    /**
     * @brief 从映射表构建
     * @param freqMap 数据到权值的映射
     * @throws std::invalid_argument 如果映射为空
     */
    HuffmanTree(const std::unordered_map<T, int>& freqMap) 
        : _root(nullptr), _built(false) {
        build(freqMap);
    }
    
    /**
     * @brief 从初始化列表构建
     * @param ilist 初始化列表 {数据, 权值}
     */
    HuffmanTree(std::initializer_list<std::pair<T, int>> ilist) 
        : _root(nullptr), _built(false) {
        std::unordered_map<T, int> freqMap;
        for (const auto& pair : ilist) {
            freqMap[pair.first] = pair.second;
        }
        build(freqMap);
    }
    
    // ---- 析构函数 ----
    
    /**
     * @brief 析构函数
     */
    ~HuffmanTree() {
        destroyTree(_root);
    }
    
    // ---- 拷贝控制 ----
    
    /**
     * @brief 拷贝构造
     * @param other 要拷贝的对象
     */
    HuffmanTree(const HuffmanTree& other) 
        : _root(copyTree(other._root)), 
          _codeTable(other._codeTable),
          _built(other._built) {}
    
    /**
     * @brief 移动构造
     * @param other 要移动的对象
     */
    HuffmanTree(HuffmanTree&& other) noexcept 
        : _root(other._root), 
          _codeTable(std::move(other._codeTable)),
          _built(other._built) {
        other._root = nullptr;
        other._built = false;
    }
    
    /**
     * @brief 拷贝赋值
     * @param other 要拷贝的对象
     * @return HuffmanTree&
     */
    HuffmanTree& operator=(const HuffmanTree& other) {
        if (this != &other) {
            HuffmanTree tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    /**
     * @brief 移动赋值
     * @param other 要移动的对象
     * @return HuffmanTree&
     */
    HuffmanTree& operator=(HuffmanTree&& other) noexcept {
        if (this != &other) {
            this->~HuffmanTree();
            _root = other._root;
            _codeTable = std::move(other._codeTable);
            _built = other._built;
            other._root = nullptr;
            other._built = false;
        }
        return *this;
    }
    
    /**
     * @brief 交换两棵树
     * @param other 要交换的对象
     */
    void swap(HuffmanTree& other) {
        std::swap(_root, other._root);
        std::swap(_codeTable, other._codeTable);
        std::swap(_built, other._built);
    }
    
    // ---- 构建 ----
    
    /**
     * @brief 构建哈夫曼树
     * @param data 数据数组
     * @param weights 权值数组
     * @param n 元素个数
     * @throws std::invalid_argument 如果数组为空或大小不匹配
     */
    void build(const T* data, const int* weights, std::size_t n) {
        if (!data || !weights || n == 0) {
            throw std::invalid_argument("Invalid input data");
        }
        
        std::unordered_map<T, int> freqMap;
        for (std::size_t i = 0; i < n; ++i) {
            freqMap[data[i]] = weights[i];
        }
        build(freqMap);
    }
    
    /**
     * @brief 构建哈夫曼树
     * @param freqMap 数据到权值的映射
     * @throws std::invalid_argument 如果映射为空
     */
    void build(const std::unordered_map<T, int>& freqMap) {
        if (freqMap.empty()) {
            throw std::invalid_argument("Frequency map is empty");
        }
        
        // 清理旧树
        destroyTree(_root);
        _root = nullptr;
        _codeTable.clear();
        _built = false;
        
        // 使用最小堆
        std::priority_queue<Node*, std::vector<Node*>, typename Node::Compare> minHeap;
        
        // 创建叶子节点并加入堆
        for (const auto& pair : freqMap) {
            if (pair.second <= 0) {
                throw std::invalid_argument("Weight must be positive");
            }
            minHeap.push(new Node(pair.first, pair.second));
        }
        
        // 构建哈夫曼树
        while (minHeap.size() > 1) {
            Node* left = minHeap.top();
            minHeap.pop();
            Node* right = minHeap.top();
            minHeap.pop();
            
            Node* parent = new Node(left->weight + right->weight);
            parent->left = left;
            parent->right = right;
            
            minHeap.push(parent);
        }
        
        _root = minHeap.top();
        minHeap.pop();
        
        // 生成编码表
        generateCodeTable(_root, "");
        _built = true;
    }
    
    // ---- 查询 ----
    
    /**
     * @brief 判断是否已构建
     * @return bool
     */
    bool isBuilt() const { return _built; }
    
    /**
     * @brief 判断是否为空
     * @return bool
     */
    bool empty() const { return _root == nullptr; }
    
    /**
     * @brief 获取编码表
     * @return const std::unordered_map<T, std::string>& 编码表引用
     */
    const std::unordered_map<T, std::string>& codeTable() const {
        if (!_built) {
            throw std::runtime_error("Huffman tree not built yet");
        }
        return _codeTable;
    }
    
    /**
     * @brief 获取某个数据的编码
     * @param data 数据
     * @return std::string 编码
     * @throws std::out_of_range 如果数据不存在
     */
    std::string getCode(const T& data) const {
        if (!_built) {
            throw std::runtime_error("Huffman tree not built yet");
        }
        auto it = _codeTable.find(data);
        if (it == _codeTable.end()) {
            throw std::out_of_range("Data not found in code table");
        }
        return it->second;
    }
    
    /**
     * @brief 获取树的深度
     * @return int 树的深度
     */
    int depth() const {
        return depthRecursive(_root);
    }
    
    /**
     * @brief 获取总权值
     * @return int 总权值
     */
    int totalWeight() const {
        return _root ? _root->weight : 0;
    }
    
    // ---- 编码 ----
    
    /**
     * @brief 编码文本
     * @param text 要编码的文本
     * @return std::string 编码后的二进制字符串
     * @throws std::runtime_error 如果树未构建
     * @throws std::out_of_range 如果文本包含未编码的字符
     */
    std::string encode(const std::vector<T>& text) const {
        if (!_built) {
            throw std::runtime_error("Huffman tree not built yet");
        }
        
        std::string result;
        for (const T& ch : text) {
            auto it = _codeTable.find(ch);
            if (it == _codeTable.end()) {
                throw std::out_of_range("Character not in code table");
            }
            result += it->second;
        }
        return result;
    }
    
    /**
     * @brief 编码文本（C风格字符串）
     * @param text 要编码的文本
     * @return std::string 编码后的二进制字符串
     */
    std::string encode(const T* text) const {
        std::vector<T> vec;
        while (*text) {
            vec.push_back(*text++);
        }
        return encode(vec);
    }
    
    // ---- 解码 ----
    
    /**
     * @brief 解码二进制字符串
     * @param code 二进制字符串
     * @return std::vector<T> 解码后的数据
     * @throws std::runtime_error 如果树未构建或编码无效
     */
    std::vector<T> decode(const std::string& code) const {
        if (!_built) {
            throw std::runtime_error("Huffman tree not built yet");
        }
        
        std::vector<T> result;
        Node* current = _root;
        
        for (char bit : code) {
            if (bit == '0') {
                current = current->left;
            } else if (bit == '1') {
                current = current->right;
            } else {
                throw std::invalid_argument("Invalid code: must contain only '0' and '1'");
            }
            
            if (!current) {
                throw std::runtime_error("Invalid code sequence");
            }
            
            if (current->isLeaf()) {
                result.push_back(current->data);
                current = _root;
            }
        }
        
        // 检查是否有未完成的编码
        if (!current->isLeaf()) {
            throw std::runtime_error("Incomplete code sequence");
        }
        
        return result;
    }
    
    /**
     * @brief 解码并返回字符串（适用于 char 类型）
     * @param code 二进制字符串
     * @return std::string 解码后的字符串
     */
    std::string decodeToString(const std::string& code) const {
        auto result = decode(code);
        std::string str;
        for (const auto& ch : result) {
            str += ch;
        }
        return str;
    }
    
    // ---- 打印 ----
    
    /**
     * @brief 打印哈夫曼树
     * @param os 输出流
     */
    void print(std::ostream& os = std::cout) const {
        if (!_built || !_root) {
            os << "Huffman tree is empty\n";
            return;
        }
        
        os << "Huffman Tree:\n";
        printTree(_root, "", os);
        os << "\n";
    }
    
    /**
     * @brief 打印编码表
     * @param os 输出流
     */
    void printCodeTable(std::ostream& os = std::cout) const {
        if (!_built) {
            os << "Huffman tree not built yet\n";
            return;
        }
        
        os << "Huffman Code Table:\n";
        for (const auto& pair : _codeTable) {
            os << pair.first << ": " << pair.second << "\n";
        }
        os << "\n";
    }
    
    /**
     * @brief 遍历所有叶子节点
     * @param callback 回调函数
     */
    void forEachLeaf(std::function<void(const T&, int, const std::string&)> callback) const {
        if (!_built) return;
        
        for (const auto& pair : _codeTable) {
            callback(pair.first, getWeight(pair.first), pair.second);
        }
    }

private:
    /**
     * @brief 递归计算深度
     */
    int depthRecursive(Node* node) const {
        if (!node) return 0;
        if (node->isLeaf()) return 1;
        return 1 + std::max(depthRecursive(node->left), depthRecursive(node->right));
    }
    
    /**
     * @brief 获取数据的权值
     */
    int getWeight(const T& data) const {
        Node* node = findNode(_root, data);
        return node ? node->weight : 0;
    }
    
    /**
     * @brief 递归查找节点
     */
    Node* findNode(Node* node, const T& data) const {
        if (!node) return nullptr;
        if (node->isLeaf() && node->data == data) return node;
        
        Node* found = findNode(node->left, data);
        if (found) return found;
        return findNode(node->right, data);
    }
};

/**
 * @brief 外部 swap 重载（支持 ADL）
 */
template<typename T>
void swap(HuffmanTree<T>& a, HuffmanTree<T>& b) {
    a.swap(b);
}

#endif // HUFFMAN_TREE_HPP