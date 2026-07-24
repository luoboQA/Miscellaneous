#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;

/*
HT数组（n=5，m=9）:
下标:    0    1    2    3    4    5    6    7    8    9
      [未用] [A]  [B]  [C]  [D]  [E]  [F]  [G]  [H]  [I]
权值:    -    5    4    3    2    1    3    6    9   15
父结点:  -    8    8    7    6    6    7    9    9    0
左孩子:  -    0    0    0    0    0    5    3    2    7
右孩子:  -    0    0    0    0    0    4    6    1    8

6	[F]	第一个内部结点（合并 D 和 E）
7	[G]	第二个内部结点（合并 C 和 F）
8	[H]	第三个内部结点（合并 B 和 A）
9	[I]	第四个内部结点（根，合并 G 和 H）

树形结构:
              9 (15)  ← 根结点
             /      \
           7 (6)    8 (9)
          /   \    /   \
        3 (3) 6(3)2(4) 1(5)
              / \
            5(1) 4(2)

编码表（左0右1，从根到叶子）:
A(5):  11
B(4):  10
C(3):  00
D(2):  011
E(1):  010
*/

// ========== 哈夫曼树的存储结构 ==========

typedef struct {
    unsigned int weight;
    unsigned int parent, lchild, rchild;
    char ch;  // 存储字符，用于解码
} HTNode, *HuffmanTree;

typedef char **HuffmanCode;  // 动态分配数组存储哈夫曼编码表

// ========== 选取两个权值最小的结点 ==========

void Select(HuffmanTree HT, int n, int &s1, int &s2) {
    int i;
    unsigned int min1 = 0xFFFFFFFF, min2 = 0xFFFFFFFF;
    s1 = s2 = 0;
    
    for (i = 1; i <= n; i++) {
        if (HT[i].parent == 0 && HT[i].weight < min1) {
            min1 = HT[i].weight;
            s1 = i;
        }
    }
    
    for (i = 1; i <= n; i++) {
        if (HT[i].parent == 0 && i != s1 && HT[i].weight < min2) {
            min2 = HT[i].weight;
            s2 = i;
        }
    }
}

// ========== 构建哈夫曼树 ==========

void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, char *chars, int n) {
    if (n <= 1) return;
    
    int m = 2 * n - 1; // 哈夫曼树的结点总数,n是叶子结点数
    HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
    
    // 初始化叶子节点
    int i;
    for (i = 1; i <= n; i++) {
        HT[i].weight = w[i];
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
        HT[i].ch = chars[i];  // 新增：存储字符
    }
    // 初始化非叶子节点
    for (i = n + 1; i <= m; i++) {
        HT[i].weight = 0;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
        HT[i].ch = '\0';  // 新增：内部结点无字符
    }
    
    // 构造哈夫曼树
    for (i = n + 1; i <= m; i++) {
        int s1, s2;
        Select(HT, i - 1, s1, s2); // 选两个最小的结点
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
    
    // 从叶子到根逆向求每个字符的哈夫曼编码
    HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));
    char *cd = (char *)malloc(n * sizeof(char));
    cd[n - 1] = '\0';
    
    for (i = 1; i <= n; i++) {
        int start = n - 1;
        int c = i;
        int f = HT[i].parent;
        while (f != 0) {
            start--;
            if (HT[f].lchild == c) {
                cd[start] = '0';
            } else {
                cd[start] = '1';
            }
            c = f;
            f = HT[f].parent;
        }
        HC[i] = (char *)malloc((n - start) * sizeof(char));
        strcpy(HC[i], &cd[start]);
    }
    free(cd);
}

// ========== 打印哈夫曼树 ==========

void PrintHuffmanTree(HuffmanTree HT, int n) {
    int m = 2 * n - 1;
    printf("结点\t权值\t字符\t父结点\t左孩子\t右孩子\n");
    for (int i = 1; i <= m; i++) {
        char ch = HT[i].ch ? HT[i].ch : '-';
        printf("%d\t%d\t%c\t%d\t%d\t%d\n", 
               i, HT[i].weight, ch, HT[i].parent, HT[i].lchild, HT[i].rchild);
    }
}

// ========== 打印编码表 ==========

void PrintHuffmanCode(HuffmanCode HC, int n, char *chars) {
    printf("\n哈夫曼编码表：\n");
    for (int i = 1; i <= n; i++) {
        printf("%c: %s\n", chars[i], HC[i]);
    }
}

// ========== 编码 ==========

void Encode(HuffmanCode HC, int n, char *chars, char *text) {
    printf("\n编码结果：");
    for (int i = 0; text[i] != '\0'; i++) {
        for (int j = 1; j <= n; j++) {
            if (text[i] == chars[j]) {
                printf("%s", HC[j]);
                break;
            }
        }
    }
    printf("\n");
}

// ========== 解码 ==========

void Decode(HuffmanTree HT, int n, char *code) {
    printf("\n解码结果：");
    int root = 2 * n - 1;
    int p = root;
    for (int i = 0; code[i] != '\0'; i++) {
        if (code[i] == '0') {
            p = HT[p].lchild;
        } else {
            p = HT[p].rchild;
        }
        if (HT[p].lchild == 0 && HT[p].rchild == 0) {
            // 叶子结点
            printf("%c", HT[p].ch);  // 修改：输出字符
            p = root;
        }
    }
    printf("\n");
}

int main() {
    printf("========================================\n");
    printf("    哈夫曼树（Huffman Tree）\n");
    printf("========================================\n\n");
    
    // 示例：字符频次统计
    char chars[] = {' ', 'A', 'B', 'C', 'D', 'E'};
    int w[6] = {0, 5, 4, 3, 2, 1};  // 权值
    int n = 5;
    
    // 展示字符和权值
    printf("字符和权值：\n");
    for (int i = 1; i <= n; i++) {
        printf("%c: %d\n", chars[i], w[i]);
    }
    printf("\n");
    
    HuffmanTree HT;
    HuffmanCode HC;
    
    // 构建哈夫曼树（传入 chars）
    HuffmanCoding(HT, HC, w, chars, n);
    
    // 打印哈夫曼树
    PrintHuffmanTree(HT, n);
    
    // 打印编码表
    PrintHuffmanCode(HC, n, chars);
    
    // 编码测试
    char text[] = "ABCDE";
    Encode(HC, n, chars, text);
    
    // 解码测试
    char code[] = "111000011010";
    Decode(HT, n, code);
    
    printf("\n");
    printf("【哈夫曼树应用】\n");
    printf("1. 数据压缩（文件压缩）\n");
    printf("2. 编码长度最短（前缀编码）\n");
    printf("3. 解码唯一性\n");
    
    // 释放内存
    for (int i = 1; i <= n; i++) {
        free(HC[i]);
    }
    free(HC);
    free(HT);
    
    getchar();
    return 0;
}