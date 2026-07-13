#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 1. 斐波那契数列 ====================

// 递归（重复计算）
int Fibonacci_Recursive(int n) {
    if (n <= 1) return n;
    return Fibonacci_Recursive(n - 1) + Fibonacci_Recursive(n - 2);
}

// 动态规划（自底向上）
int Fibonacci_DP(int n) {
    if (n <= 1) return n;
    int *dp = new int[n + 1];
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    int result = dp[n];
    delete[] dp;
    return result;
}

// 动态规划（空间优化）
int Fibonacci_DP_Opt(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// ==================== 2. 0/1背包问题 ====================

int Knapsack01(int W, vector<int> weights, vector<int> values, int n) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    return dp[n][W];
}

// 0/1背包（空间优化）
int Knapsack01_Opt(int W, vector<int> weights, vector<int> values, int n) {
    vector<int> dp(W + 1, 0);
    
    for (int i = 0; i < n; i++) {
        for (int w = W; w >= weights[i]; w--) {
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    return dp[W];
}

// ==================== 3. 最长公共子序列 ====================

int LCS(string s1, string s2) {
    int m = s1.length(), n = s2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

// ==================== 4. 最长递增子序列 ====================

int LIS(vector<int> &v) {
    int n = v.size();
    vector<int> dp(n, 1);
    int maxLen = 1;
    
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (v[i] > v[j]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        maxLen = max(maxLen, dp[i]);
    }
    return maxLen;
}

// ==================== 5. 爬楼梯 ====================

int ClimbStairs(int n) {
    if (n <= 2) return n;
    int a = 1, b = 2;
    for (int i = 3; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    printf("========================================\n");
    printf("    动态规划（Dynamic Programming）\n");
    printf("========================================\n");
    printf("思想：分解子问题，记录子问题解\n");
    printf("适用：重叠子问题 + 最优子结构\n\n");
    
    // 1. 斐波那契数列
    printf("【1. 斐波那契数列】\n");
    int n = 10;
    printf("F(%d) = %d（递归）\n", n, Fibonacci_Recursive(n));
    printf("F(%d) = %d（DP）\n", n, Fibonacci_DP(n));
    printf("F(%d) = %d（DP优化）\n\n", n, Fibonacci_DP_Opt(n));
    
    // 2. 0/1背包问题
    printf("【2. 0/1背包问题】\n");
    int W = 10;
    vector<int> weights = {2, 3, 4, 5};
    vector<int> values = {3, 4, 5, 6};
    printf("背包容量：%d\n", W);
    printf("物品重量：");
    for (int x : weights) printf("%d ", x);
    printf("\n物品价值：");
    for (int x : values) printf("%d ", x);
    printf("\n最大价值：%d\n", Knapsack01(W, weights, values, weights.size()));
    printf("最大价值（优化）：%d\n\n", Knapsack01_Opt(W, weights, values, weights.size()));
    
    // 3. 最长公共子序列
    printf("【3. 最长公共子序列】\n");
    string s1 = "ABCDGH", s2 = "AEDFHR";
    printf("字符串1：%s\n", s1.c_str());
    printf("字符串2：%s\n", s2.c_str());
    printf("LCS长度：%d\n\n", LCS(s1, s2));
    
    // 4. 最长递增子序列
    printf("【4. 最长递增子序列】\n");
    vector<int> v = {10, 22, 9, 33, 21, 50, 41, 60, 80};
    printf("序列：");
    for (int x : v) printf("%d ", x);
    printf("\nLIS长度：%d\n\n", LIS(v));
    
    // 5. 爬楼梯
    printf("【5. 爬楼梯】\n");
    int steps = 5;
    printf("%d阶楼梯，每次走1或2阶，方法数：%d\n", steps, ClimbStairs(steps));
    
    getchar();
    return 0;
}