#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 字典树（Trie Tree） ====================

class TrieNode {
public:
    TrieNode *children[26];
    bool isEnd;
    int count;  // 单词出现次数
    
    TrieNode() {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
        isEnd = false;
        count = 0;
    }
};

class Trie {
private:
    TrieNode *root;
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    // 插入单词
    void Insert(string word) {
        TrieNode *node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->isEnd = true;
        node->count++;
    }
    
    // 查找单词是否存在
    bool Search(string word) {
        TrieNode *node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                return false;
            }
            node = node->children[idx];
        }
        return node->isEnd;
    }
    
    // 查找单词出现次数
    int Count(string word) {
        TrieNode *node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                return 0;
            }
            node = node->children[idx];
        }
        return node->isEnd ? node->count : 0;
    }
    
    // 判断是否有以prefix为前缀的单词
    bool StartsWith(string prefix) {
        TrieNode *node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                return false;
            }
            node = node->children[idx];
        }
        return true;
    }
    
    // 获取所有以prefix为前缀的单词
    void GetAllWords(TrieNode *node, string current, vector<string> &result) {
        if (node->isEnd) {
            result.push_back(current);
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                GetAllWords(node->children[i], current + char('a' + i), result);
            }
        }
    }
    
    vector<string> GetWordsWithPrefix(string prefix) {
        vector<string> result;
        TrieNode *node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                return result;
            }
            node = node->children[idx];
        }
        GetAllWords(node, prefix, result);
        return result;
    }
    
    // 删除单词
    bool Delete(string word) {
        return DeleteHelper(root, word, 0);
    }
    
    bool DeleteHelper(TrieNode *node, string word, int depth) {
        if (depth == word.length()) {
            if (!node->isEnd) return false;
            node->isEnd = false;
            node->count = 0;
            // 如果没有子节点，可以删除
            for (int i = 0; i < 26; i++) {
                if (node->children[i] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        
        int idx = word[depth] - 'a';
        if (node->children[idx] == nullptr) return false;
        
        bool canDelete = DeleteHelper(node->children[idx], word, depth + 1);
        if (canDelete) {
            delete node->children[idx];
            node->children[idx] = nullptr;
            // 检查是否可以删除当前节点
            if (node->isEnd) return false;
            for (int i = 0; i < 26; i++) {
                if (node->children[i] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

int main() {
    printf("========================================\n");
    printf("    字典树（Trie Tree）\n");
    printf("========================================\n");
    printf("时间复杂度：O(L) L为字符串长度\n");
    printf("空间复杂度：O(n*L)\n");
    printf("适用：字符串查找、前缀匹配\n\n");
    
    Trie trie;
    vector<string> words = {"apple", "app", "apricot", "banana", "book", "boy"};
    
    printf("插入单词：");
    for (string w : words) {
        printf("%s ", w.c_str());
        trie.Insert(w);
    }
    printf("\n\n");
    
    printf("查找结果：\n");
    printf("查找 \"apple\"：%s\n", trie.Search("apple") ? "找到" : "未找到");
    printf("查找 \"app\"：%s\n", trie.Search("app") ? "找到" : "未找到");
    printf("查找 \"ape\"：%s\n", trie.Search("ape") ? "找到" : "未找到");
    
    printf("\n前缀匹配：\n");
    vector<string> results = trie.GetWordsWithPrefix("ap");
    printf("以 \"ap\" 为前缀的单词：");
    for (string w : results) {
        printf("%s ", w.c_str());
    }
    printf("\n");
    
    printf("以 \"b\" 为前缀的单词：");
    results = trie.GetWordsWithPrefix("b");
    for (string w : results) {
        printf("%s ", w.c_str());
    }
    printf("\n");
    
    trie.Delete("app");
    printf("\n删除 \"app\" 后：\n");
    printf("查找 \"app\"：%s\n", trie.Search("app") ? "找到" : "未找到");
    printf("以 \"ap\" 为前缀的单词：");
    results = trie.GetWordsWithPrefix("ap");
    for (string w : results) {
        printf("%s ", w.c_str());
    }
    printf("\n");
    
    getchar();
    return 0;
}