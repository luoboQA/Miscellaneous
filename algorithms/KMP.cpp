#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== KMP算法 ====================

// 构建next数组
void GetNext(string pattern, vector<int> &next) {
    int i = 0, j = -1;
    next[0] = -1;
    int len = pattern.length();
    
    while (i < len - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

// KMP优化next数组（改进版）
void GetNextVal(string pattern, vector<int> &nextval) {
    int i = 0, j = -1;
    nextval[0] = -1;
    int len = pattern.length();
    
    while (i < len - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            i++;
            j++;
            if (pattern[i] != pattern[j]) {
                nextval[i] = j;
            } else {
                nextval[i] = nextval[j];
            }
        } else {
            j = nextval[j];
        }
    }
}

// KMP匹配
int KMP(string text, string pattern) {
    int n = text.length();
    int m = pattern.length();
    if (m == 0) return 0;
    if (n < m) return -1;
    
    vector<int> next(m);
    GetNext(pattern, next);
    
    int i = 0, j = 0;
    while (i < n && j < m) {
        if (j == -1 || text[i] == pattern[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    
    if (j == m) {
        return i - j;
    }
    return -1;
}

// KMP匹配（优化版）
int KMP_Optimized(string text, string pattern) {
    int n = text.length();
    int m = pattern.length();
    if (m == 0) return 0;
    if (n < m) return -1;
    
    vector<int> nextval(m);
    GetNextVal(pattern, nextval);
    
    int i = 0, j = 0;
    while (i < n && j < m) {
        if (j == -1 || text[i] == pattern[j]) {
            i++;
            j++;
        } else {
            j = nextval[j];
        }
    }
    
    if (j == m) {
        return i - j;
    }
    return -1;
}

// 统计所有匹配位置
vector<int> KMP_All(string text, string pattern) {
    vector<int> result;
    int n = text.length();
    int m = pattern.length();
    if (m == 0) return result;
    if (n < m) return result;
    
    vector<int> next(m);
    GetNext(pattern, next);
    
    int i = 0, j = 0;
    while (i < n) {
        if (j == -1 || text[i] == pattern[j]) {
            i++;
            j++;
            if (j == m) {
                result.push_back(i - j);
                j = next[j - 1];
            }
        } else {
            j = next[j];
        }
    }
    return result;
}

int main() {
    printf("========================================\n");
    printf("    KMP字符串匹配算法\n");
    printf("========================================\n");
    printf("时间复杂度：O(n+m)\n");
    printf("空间复杂度：O(m)\n\n");
    
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    
    printf("文本串：%s\n", text.c_str());
    printf("模式串：%s\n", pattern.c_str());
    
    int pos = KMP(text, pattern);
    printf("匹配位置：%d\n", pos);
    
    string pattern2 = "ABA";
    vector<int> positions = KMP_All(text, pattern2);
    printf("\n查找所有 \"%s\" 的位置：", pattern2.c_str());
    for (int p : positions) {
        printf("%d ", p);
    }
    printf("\n");
    
    getchar();
    return 0;
}