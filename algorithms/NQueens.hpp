/*
 *（回溯，剪枝，约束传播）。逐行放置皇后，检查列和对角线冲突，不满足时回溯。
 *
 * N皇后回溯思路：
 * 1. 从第一行开始，尝试在每一列放置皇后
 * 2. 检查当前位置是否与已放置的皇后冲突（列、主对角线、副对角线）
 * 3. 若不冲突，放置皇后并递归进入下一行
 * 4. 若某一行所有列都无法放置，则回溯到上一行
 * 5. 当所有行都放置了皇后，找到一种解法
 * 6. 继续尝试其他可能性，直到所有解法都被找到
 *
 * 问题	        时间复杂度	    空间复杂度	适用场景
 * N皇后问题	O(n!)	       O(n)	    组合优化、约束满足
 * 
 * 4皇后问题：
 * 
 * 棋盘：
 *   Q . . .
 *   . . Q .
 *   . . . Q
 *   . Q . .
 * 
 * 回溯过程：
 * 
 * 第1行（row=0）：
 *   尝试col=0：放置Q在(0,0)
 *   约束：col0已占，diag1[3]已占，diag2[0]已占
 * 
 * 第2行（row=1）：
 *   尝试col=0：col0冲突 → 跳过
 *   尝试col=1：col1冲突 → 跳过
 *   尝试col=2：不冲突 → 放置Q在(1,2)
 *   约束：col2已占，diag1[1]已占，diag2[3]已占
 * 
 * 第3行（row=2）：
 *   尝试col=0：col0冲突 → 跳过
 *   尝试col=1：col1冲突 → 跳过
 *   尝试col=2：col2冲突 → 跳过
 *   尝试col=3：不冲突 → 放置Q在(2,3)
 *   约束：col3已占，diag1[2]已占，diag2[5]已占
 * 
 * 第4行（row=3）：
 *   尝试col=0：col0冲突 → 跳过
 *   尝试col=1：col1冲突 → 跳过
 *   尝试col=2：col2冲突 → 跳过
 *   尝试col=3：col3冲突 → 跳过
 *   无解！回溯到第3行
 * 
 * 第3行（row=2）：
 *   移除(2,3)，继续尝试col=4（无）
 *   无解！回溯到第2行
 * 
 * 第2行（row=1）：
 *   移除(1,2)，继续尝试col=3
 *   尝试col=3：不冲突 → 放置Q在(1,3)
 *   约束：col3已占，diag1[2]已占，diag2[4]已占
 * 
 * 第3行（row=2）：
 *   尝试col=0：col0冲突 → 跳过
 *   尝试col=1：不冲突 → 放置Q在(2,1)
 *   约束：col1已占，diag1[0]已占，diag2[3]已占
 * 
 * 第4行（row=3）：
 *   尝试col=0：col0冲突 → 跳过
 *   尝试col=1：col1冲突 → 跳过
 *   尝试col=2：不冲突 → 放置Q在(3,2)
 *   所有行放置完成 → 找到一种解法！
 * 
 * 解法：[0, 3, 1, 2]（第0行col0，第1行col3，第2行col1，第3行col2）
 * 
 * 4皇后共有2种解法
 * 8皇后共有92种解法
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== N皇后问题 ====================

/**
 * @brief N皇后类
 * 
 * @param n          皇后数量
 * @param queens     每行皇后所在的列
 * @param cols       列占用标记
 * @param diag1      主对角线占用标记（row - col + n - 1）
 * @param diag2      副对角线占用标记（row + col）
 * @param solutions  所有解
 * @param count      解的数量
 */
class NQueens {
private:
    int n;
    vector<int> queens;
    vector<bool> cols;
    vector<bool> diag1;
    vector<bool> diag2;
    vector<vector<int>> solutions;
    int count;
    
public:
    NQueens(int n) : n(n), count(0) {
        queens.resize(n, -1);
        cols.resize(n, false);
        diag1.resize(2 * n - 1, false);
        diag2.resize(2 * n - 1, false);
    }
    
    /**
     * @brief 回溯求解（输出所有解）
     * 
     * @param row 当前行
     * 
     * @note 时间复杂度 O(n!)，空间复杂度 O(n)
     * @note 使用三个数组快速判断冲突
     */
    void Solve(int row) {
        if (row == n) {
            solutions.push_back(queens);
            count++;
            PrintSolution(queens);
            return;
        }
        
        for (int col = 0; col < n; col++) {
            if (cols[col] || diag1[row - col + n - 1] || diag2[row + col]) {
                continue;
            }
            
            queens[row] = col;
            cols[col] = true;
            diag1[row - col + n - 1] = true;
            diag2[row + col] = true;
            
            Solve(row + 1);
            
            cols[col] = false;
            diag1[row - col + n - 1] = false;
            diag2[row + col] = false;
            queens[row] = -1;
        }
    }
    
    void PrintSolution(vector<int> &q) {
        static int num = 1;
        printf("解法 %d：", num++);
        for (int i = 0; i < n; i++) {
            printf("%d ", q[i]);
        }
        printf("\n");
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (q[i] == j) {
                    printf("Q ");
                } else {
                    printf(". ");
                }
            }
            printf("\n");
        }
        printf("\n");
    }
    
    int GetCount() { return count; }
    
    /**
     * @brief 只计数不输出（用于较大N值）
     * 
     * @param row 当前行
     */
    void SolveCount(int row) {
        if (row == n) {
            count++;
            return;
        }
        
        for (int col = 0; col < n; col++) {
            if (cols[col] || diag1[row - col + n - 1] || diag2[row + col]) {
                continue;
            }
            
            cols[col] = true;
            diag1[row - col + n - 1] = true;
            diag2[row + col] = true;
            
            SolveCount(row + 1);
            
            cols[col] = false;
            diag1[row - col + n - 1] = false;
            diag2[row + col] = false;
        }
    }
};