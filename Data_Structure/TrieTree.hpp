// TrieTree.hpp
#ifndef TRIE_TREE_HPP
#define TRIE_TREE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <functional>

/**
 * @brief 字典树（Trie Tree）模板
 * @tparam CharType 字符类型（默认 char）
 * 
 * 字典树是一种用于高效存储和检索字符串的数据结构。
 * 
 * 时间复杂度：
 * - 插入/查找/删除：O(L)，L为字符串长度
 * - 前缀匹配：O(L + k)，k为匹配结果数量
 * - 空间复杂度：O(n * L)
 */
template<typename CharType = char>
class TrieTree {
private:
    struct Node {
        std::unordered_map<CharType, std::unique_ptr<Node>> children;
        bool isEnd;
        int count;  // 单词出现次数
        
        Node() : isEnd(false), count(0) {}
    };
    
    std::unique_ptr<Node> _root;
    std::size_t _size;  // 单词个数
    
    /**
     * @brief 递归收集所有单词
     * @param node 当前节点
     * @param current 当前前缀
     * @param result 结果列表
     */
    void collectWords(Node* node, std::basic_string<CharType>& current, 
                      std::vector<std::basic_string<CharType>>& result) const {
        if (!node) return;
        
        if (node->isEnd) {
            result.push_back(current);
        }
        
        for (auto& pair : node->children) {
            current.push_back(pair.first);
            collectWords(pair.second.get(), current, result);
            current.pop_back();
        }
    }
    
    /**
     * @brief 递归删除
     * @param node 当前节点
     * @param word 要删除的单词
     * @param depth 当前深度
     * @return bool 是否可以删除当前节点
     */
    bool eraseHelper(Node* node, const std::basic_string<CharType>& word, std::size_t depth) {
        if (!node) return false;
        
        if (depth == word.length()) {
            if (!node->isEnd) return false;
            node->isEnd = false;
            node->count = 0;
            return node->children.empty();
        }
        
        CharType ch = word[depth];
        auto it = node->children.find(ch);
        if (it == node->children.end()) return false;
        
        bool canDelete = eraseHelper(it->second.get(), word, depth + 1);
        if (canDelete) {
            node->children.erase(it);
            return node->children.empty() && !node->isEnd;
        }
        return false;
    }

public:
    // ---- 构造函数 ----
    
    TrieTree() : _root(std::make_unique<Node>()), _size(0) {}
    
    TrieTree(std::initializer_list<std::basic_string<CharType>> ilist) 
        : TrieTree() {
        for (const auto& word : ilist) {
            insert(word);
        }
    }
    
    // ---- 拷贝控制 ----
    
    TrieTree(const TrieTree& other) : _root(std::make_unique<Node>()), _size(other._size) {
        copyNode(_root.get(), other._root.get());
    }
    
    TrieTree(TrieTree&& other) noexcept = default;
    
    TrieTree& operator=(const TrieTree& other) {
        if (this != &other) {
            TrieTree tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    TrieTree& operator=(TrieTree&& other) noexcept = default;
    
    void swap(TrieTree& other) {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
    }
    
    // ---- 容量 ----
    
    std::size_t size() const { return _size; }
    bool empty() const { return _size == 0; }
    
    // ---- 插入 ----
    
    void insert(const std::basic_string<CharType>& word) {
        Node* node = _root.get();
        for (CharType ch : word) {
            auto& child = node->children[ch];
            if (!child) {
                child = std::make_unique<Node>();
            }
            node = child.get();
        }
        if (!node->isEnd) {
            node->isEnd = true;
            ++_size;
        }
        ++node->count;
    }
    
    void insert(std::basic_string<CharType>&& word) {
        insert(word);
    }
    
    // ---- 查找 ----
    
    bool search(const std::basic_string<CharType>& word) const {
        Node* node = findNode(word);
        return node && node->isEnd;
    }
    
    int count(const std::basic_string<CharType>& word) const {
        Node* node = findNode(word);
        return node && node->isEnd ? node->count : 0;
    }
    
    bool startsWith(const std::basic_string<CharType>& prefix) const {
        return findNode(prefix) != nullptr;
    }
    
    // ---- 删除 ----
    
    bool erase(const std::basic_string<CharType>& word) {
        if (eraseHelper(_root.get(), word, 0)) {
            --_size;
            return true;
        }
        return false;
    }
    
    // ---- 前缀匹配 ----
    
    std::vector<std::basic_string<CharType>> getWordsWithPrefix(
        const std::basic_string<CharType>& prefix) const {
        std::vector<std::basic_string<CharType>> result;
        
        Node* node = findNode(prefix);
        if (!node) return result;
        
        std::basic_string<CharType> current = prefix;
        collectWords(node, current, result);
        return result;
    }
    
    // ---- 遍历 ----
    
    void print(std::ostream& os = std::cout) const {
        std::vector<std::basic_string<CharType>> words;
        std::basic_string<CharType> current;
        collectWords(_root.get(), current, words);
        
        for (const auto& word : words) {
            os << word << " ";
        }
        os << "\n";
    }
    
    void forEach(std::function<void(const std::basic_string<CharType>&)> callback) const {
        std::vector<std::basic_string<CharType>> words;
        std::basic_string<CharType> current;
        collectWords(_root.get(), current, words);
        
        for (const auto& word : words) {
            callback(word);
        }
    }

private:
    /**
     * @brief 查找节点
     * @param str 字符串
     * @return Node* 节点指针，未找到返回nullptr
     */
    Node* findNode(const std::basic_string<CharType>& str) const {
        Node* node = _root.get();
        for (CharType ch : str) {
            auto it = node->children.find(ch);
            if (it == node->children.end()) {
                return nullptr;
            }
            node = it->second.get();
        }
        return node;
    }
    
    /**
     * @brief 递归拷贝
     */
    void copyNode(Node* dst, const Node* src) {
        for (const auto& pair : src->children) {
            dst->children[pair.first] = std::make_unique<Node>();
            copyNode(dst->children[pair.first].get(), pair.second.get());
        }
        dst->isEnd = src->isEnd;
        dst->count = src->count;
    }
};

// 类型别名
using Trie = TrieTree<char>;

template<typename CharType>
void swap(TrieTree<CharType>& a, TrieTree<CharType>& b) {
    a.swap(b);
}

#endif // TRIE_TREE_HPP