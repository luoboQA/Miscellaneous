/*
 *（状态，状态转移方程，最优子结构）。通过定义状态和状态转移方程，将大问题分解为重叠子问题，自底向上求解。
 *
 * 动态规划思路：
 * 1. 定义状态：确定dp数组的含义（如dp[i]表示第i个问题的解）
 * 2. 状态转移方程：找出状态之间的关系（如dp[i] = dp[i-1] + dp[i-2]）
 * 3. 初始化：设置边界条件
 * 4. 计算顺序：自底向上（迭代）或自顶向下（记忆化递归）
 * 5. 返回结果：最终状态即为问题的解
 *
 * 动态规划问题	    状态定义	            状态转移方程	                    时间复杂度	空间复杂度
 * 斐波那契数列	    dp[i]=第i项	        dp[i]=dp[i-1]+dp[i-2]	        O(n)	    O(1)
 * 0/1背包	        dp[i][w]=前i件物品容量w最大价值	dp[i][w]=max(dp[i-1][w], dp[i-1][w-wi]+vi)	O(nW)	O(W)
 * LCS（最长公共子序列）	dp[i][j]=s1前i,s2前j的LCS长度	dp[i][j]=max(dp[i-1][j], dp[i][j-1]) 或 +1	O(mn)	O(mn)
 * LIS（最长递增子序列）	dp[i]=以i结尾的最长递增子序列长度	dp[i]=max(dp[j]+1) for j<i and v[j]<v[i]	O(n²)	O(n)
 * 爬楼梯	        dp[i]=到第i阶的方法数	dp[i]=dp[i-1]+dp[i-2]	        O(n)	O(1)
 * 
 * 动态规划示例：
 * 
 * 【斐波那契数列】F(5)
 *   dp[0]=0, dp[1]=1
 *   dp[2]=dp[1]+dp[0]=1
 *   dp[3]=dp[2]+dp[1]=2
 *   dp[4]=dp[3]+dp[2]=3
 *   dp[5]=dp[4]+dp[3]=5
 * 
 * 【0/1背包】容量W=10，物品：(2,3),(3,4),(4,5),(5,6)
 *   dp[i][w]表（行=物品，列=容量）：
 *       0  1  2  3  4  5  6  7  8  9  10
 *   0   0  0  0  0  0  0  0  0  0  0  0
 *   1   0  0  3  3  3  3  3  3  3  3  3
 *   2   0  0  3  4  4  7  7  7  7  7  7
 *   3   0  0  3  4  5  7  8  9  9  12 12
 *   4   0  0  3  4  5  7  8  9  10 11 13
 *   最大价值 = 13
 * 
 * 【最长公共子序列】s1="ABCDGH", s2="AEDFHR"
 *   dp矩阵：
 *       A  E  D  F  H  R
 *    A  1  1  1  1  1  1
 *    B  1  1  1  1  1  1
 *    C  1  1  1  1  1  1
 *    D  1  1  2  2  2  2
 *    G  1  1  2  2  2  2
 *    H  1  1  2  2  3  3
 *   LCS长度 = 3（公共子序列：ADH）
 * 
 * 【最长递增子序列】[10,22,9,33,21,50,41,60,80]
 *   dp[0]=1
 *   dp[1]=2 (10<22)
 *   dp[2]=1 (9最小)
 *   dp[3]=3 (10<22<33 或 9<22<33)
 *   dp[4]=2 (9<21)
 *   dp[5]=4 (10<22<33<50 或 9<22<33<50)
 *   dp[6]=4 (10<22<33<41 或 9<22<33<41)
 *   dp[7]=5 (10<22<33<41<60)
 *   dp[8]=6 (10<22<33<41<60<80)
 *   LIS长度 = 6
 * 
 * 【爬楼梯】5阶楼梯，每次走1或2阶
 *   dp[1]=1, dp[2]=2
 *   dp[3]=dp[2]+dp[1]=3（1+1+1, 1+2, 2+1）
 *   dp[4]=dp[3]+dp[2]=5
 *   dp[5]=dp[4]+dp[3]=8
 */


using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== 1. 斐波那契数列 ====================

/**
 * @brief 斐波那契数列（递归版本）
 * 
 * @param n 项数
 * @return int 第n项的值
 * 
 * @note 时间复杂度 O(2^n)，空间复杂度 O(n)
 * @note 存在大量重复计算，效率极低
 */
int Fibonacci_Recursive(int n) {
    if (n <= 1) return n;
    return Fibonacci_Recursive(n - 1) + Fibonacci_Recursive(n - 2);
}

/**
 * @brief 斐波那契数列（动态规划版本）
 * 
 * @param n 项数
 * @return int 第n项的值
 * 
 * @note 时间复杂度 O(n)，空间复杂度 O(n)
 * @note 自底向上，记录每个子问题的解
 */
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

/**
 * @brief 斐波那契数列（空间优化版本）
 * 
 * @param n 项数
 * @return int 第n项的值
 * 
 * @note 时间复杂度 O(n)，空间复杂度 O(1)
 * @note 只保留前两个状态，优化空间
 */
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

/**
 * @brief 0/1背包问题（动态规划）
 * 
 * @param W       背包容量
 * @param weights 物品重量数组
 * @param values  物品价值数组
 * @param n       物品数量
 * @return int    最大总价值
 * 
 * @note 时间复杂度 O(nW)，空间复杂度 O(nW)
 * @note dp[i][w] = 前i件物品在容量w下的最大价值
 */
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

/**
 * @brief 0/1背包问题（空间优化版本）
 * 
 * @param W       背包容量
 * @param weights 物品重量数组
 * @param values  物品价值数组
 * @param n       物品数量
 * @return int    最大总价值
 * 
 * @note 时间复杂度 O(nW)，空间复杂度 O(W)
 * @note 逆序遍历容量，防止重复选择同一物品
 */
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

/**
 * @brief 最长公共子序列（LCS）
 * 
 * @param s1 字符串1
 * @param s2 字符串2
 * @return int LCS长度
 * 
 * @note 时间复杂度 O(mn)，空间复杂度 O(mn)
 * @note dp[i][j] = s1前i个字符和s2前j个字符的LCS长度
 */
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

/**
 * @brief 最长递增子序列（LIS）
 * 
 * @param v 序列
 * @return int LIS长度
 * 
 * @note 时间复杂度 O(n²)，空间复杂度 O(n)
 * @note dp[i] = 以v[i]结尾的最长递增子序列长度
 */
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

/**
 * @brief 爬楼梯（每次走1或2阶）
 * 
 * @param n 楼梯阶数
 * @return int 到达第n阶的方法数
 * 
 * @note 时间复杂度 O(n)，空间复杂度 O(1)
 * @note dp[i] = dp[i-1] + dp[i-2]
 */
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