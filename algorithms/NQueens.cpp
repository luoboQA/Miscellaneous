#include <iostream>
#include <vector>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

// ==================== N皇后问题 ====================

class NQueens {
private:
    int n;
    vector<int> queens;       // queens[i] 表示第i行皇后在第几列
    vector<bool> cols;        // 列是否被占用
    vector<bool> diag1;       // 主对角线（行-列）
    vector<bool> diag2;       // 副对角线（行+列）
    vector<vector<int>> solutions;
    int count;
    
public:
    NQueens(int n) : n(n), count(0) {
        queens.resize(n, -1);
        cols.resize(n, false);
        diag1.resize(2 * n - 1, false);
        diag2.resize(2 * n - 1, false);
    }
    
    // 回溯求解
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
            
            // 放置皇后
            queens[row] = col;
            cols[col] = true;
            diag1[row - col + n - 1] = true;
            diag2[row + col] = true;
            
            Solve(row + 1);
            
            // 回溯
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
        
        // 打印棋盘
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
    
    // 简化版（只计数）
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

int main() {
    printf("========================================\n");
    printf("    N皇后问题（回溯法）\n");
    printf("========================================\n");
    printf("时间复杂度：O(n!)\n");
    printf("空间复杂度：O(n)\n");
    printf("适用：组合优化、约束满足问题\n\n");
    
    int n = 4;
    printf("【%d皇后问题】\n\n", n);
    NQueens nq(n);
    nq.Solve(0);
    printf("共有 %d 种解法\n", nq.GetCount());
    
    printf("\n【8皇后问题（只计数）】\n");
    NQueens nq8(8);
    nq8.SolveCount(0);
    printf("8皇后共有 %d 种解法\n", nq8.GetCount());
    
    getchar();
    return 0;
}