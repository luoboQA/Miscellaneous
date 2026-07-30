/*
 *（前缀函数，next数组，回溯）。通过预处理模式串的next数组，在匹配失败时跳过已匹配的部分，避免重复比较。
 *
 * KMP算法思路：
 * 1. 预处理模式串，计算next数组（最长相同前后缀长度）
 * 2. 匹配时，主串指针i不回溯，模式串指针j根据next数组回溯
 * 3. 若匹配成功，i和j同时后移
 * 4. 若匹配失败，j = next[j]（利用已匹配的信息）
 * 5. 若j = -1，则i后移，j归零
 *
 * 字符串匹配算法	平均时间复杂度	最坏时间复杂度	空间复杂度	适用场景
 * 朴素匹配	    O(n*m)	        O(n*m)	        O(1)	    模式串较短
 * KMP匹配	    O(n+m)	        O(n+m)	        O(m)	    模式串较长，有重复前缀
 * 
 * 模式串："ABABCABAB"
 * 
 * 构建next数组：
 *   next[0] = -1
 *   i=0, j=-1 → i=1, j=0, next[1]=0  (A)
 *   i=1, j=0, pattern[1]=B vs pattern[0]=A → 不匹配 → j=next[0]=-1
 *   i=1, j=-1 → i=2, j=0, next[2]=0  (AB)
 *   i=2, j=0, pattern[2]=A vs pattern[0]=A → 匹配 → i=3, j=1, next[3]=1  (ABA)
 *   i=3, j=1, pattern[3]=B vs pattern[1]=B → 匹配 → i=4, j=2, next[4]=2  (ABAB)
 *   i=4, j=2, pattern[4]=C vs pattern[2]=A → 不匹配 → j=next[2]=0
 *   i=4, j=0, pattern[4]=C vs pattern[0]=A → 不匹配 → j=next[0]=-1
 *   i=4, j=-1 → i=5, j=0, next[5]=0  (ABABC)
 *   i=5, j=0, pattern[5]=A vs pattern[0]=A → 匹配 → i=6, j=1, next[6]=1  (ABABCA)
 *   i=6, j=1, pattern[6]=B vs pattern[1]=B → 匹配 → i=7, j=2, next[7]=2  (ABABCAB)
 *   i=7, j=2, pattern[7]=A vs pattern[2]=A → 匹配 → i=8, j=3, next[8]=3  (ABABCABA)
 * 
 * next数组：[-1, 0, 0, 1, 2, 0, 1, 2, 3]
 * 
 * 匹配过程（文本串："ABABDABACDABABCABAB"）：
 *   第1轮：text[0..4]="ABABD" vs pattern[0..4]="ABABC"
 *     匹配到text[4]='D', pattern[4]='C' → 不匹配
 *     j=next[4]=2 → 模式串从索引2继续匹配（已匹配"AB"）
 * 
 *   第2轮：text[4]='D' vs pattern[2]='A' → 不匹配
 *     j=next[2]=0 → 模式串从索引0继续匹配
 *     text[4]='D' vs pattern[0]='A' → 不匹配
 *     j=next[0]=-1 → i=5, j=0
 * 
 *   第3轮：从text[5]='A'开始匹配
 *     text[5..13]="ABACDABAB" vs pattern[0..8]="ABABCABAB"
 *     匹配到text[13]='B', pattern[8]='B' → 匹配成功
 * 
 *   匹配位置：13 - 8 = 5
 * 
 * KMP优势：主串指针不回溯，利用next数组跳过无效比较
 * 优化版nextval：在next基础上进一步优化，避免连续相同字符的重复比较
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== KMP算法 ====================

/**
 * @brief 构建next数组
 * 
 * @param pattern 模式串
 * @param next    输出next数组
 * 
 * @note next[i]表示pattern[0..i-1]的最长相同前后缀长度
 * @note next[0] = -1
 */
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

/**
 * @brief 构建优化nextval数组
 * 
 * @param pattern   模式串
 * @param nextval   输出nextval数组
 * 
 * @note 在next基础上进一步优化，减少不必要的比较
 */
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

/**
 * @brief KMP匹配
 * 
 * @param text    文本串
 * @param pattern 模式串
 * @return int    匹配成功返回起始位置，失败返回-1
 * 
 * @note 时间复杂度 O(n+m)，空间复杂度 O(m)
 */
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

/**
 * @brief KMP匹配（优化版）
 * 
 * @param text    文本串
 * @param pattern 模式串
 * @return int    匹配成功返回起始位置，失败返回-1
 * 
 * @note 使用nextval数组，进一步优化
 */
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

/**
 * @brief 统计所有匹配位置
 * 
 * @param text    文本串
 * @param pattern 模式串
 * @return vector<int> 所有匹配的起始位置
 * 
 * @note 找到匹配后，继续从next[j-1]开始查找
 */
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