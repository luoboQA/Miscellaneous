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
权值:    -    65   66   67   68   69   137  204  131  335
父结点:  -    8    8    7    6    6    7    9    9    0
左孩子:  -    0    0    0    0    0    5    3    2    7
右孩子:  -    0    0    0    0    0    4    6    1    8

6	[F]	第一个内部结点（合并 D 和 E，68+69=137）
7	[G]	第二个内部结点（合并 C 和 F，67+137=204）
8	[H]	第三个内部结点（合并 B 和 A，66+65=131）
9	[I]	第四个内部结点（根，合并 G 和 H，204+131=335）

树形结构:
              9 (335)  ← 根结点
             /       \
           7 (204)   8 (131)
          /   \     /   \
        3 (67) 6(137) 2(66) 1(65)
               / \
             5(69) 4(68)

编码表（左0右1，从根到叶子）:
A(65):  11
B(66):  10
C(67):  00
D(68):  011
E(69):  010
*/

// ========== 哈夫曼树的存储结构 ==========

typedef struct {
    unsigned int weight;  // 叶子结点存储 ASCII 码，内部结点存储权值和
    unsigned int parent, lchild, rchild;
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
/** @param HT: 哈夫曼树数组
 *  @param HC: 哈夫曼编码表
 *  @param w: 权值数组（从下标1开始，存储 ASCII 码）
 *  @param n: 叶子结点数
 */
void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, int n) {
    if (n <= 1) return;
    
    int m = 2 * n - 1; // 哈夫曼树的结点总数,n是叶子结点数
    HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
    
    // 初始化叶子节点
    int i;
    for (i = 1; i <= n; i++) {
        HT[i].weight = w[i];  // w[i] 是 ASCII 码
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    // 初始化非叶子节点
    for (i = n + 1; i <= m; i++) {
        HT[i].weight = 0;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
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
    printf("结点\t权值\t父结点\t左孩子\t右孩子\n");
    for (int i = 1; i <= m; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", 
               i, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild);
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
// 叶子结点权值直接存储 ASCII 码
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
            // 叶子结点：权值直接就是 ASCII 码
            printf("%c", (char)HT[p].weight);
            p = root;
        }
    }
    printf("\n");
}

int main() {
    printf("========================================\n");
    printf("    哈夫曼树（Huffman Tree）\n");
    printf("========================================\n\n");
    
    // 示例：字符频次统计（权值直接使用 ASCII 码）
    char chars[] = {' ', 'A', 'B', 'C', 'D', 'E'};
    int w[6] = {0, 65, 66, 67, 68, 69};  // ← 权值 = ASCII 码！
    int n = 5;
    
    // 展示字符和权值
    printf("字符和权值（ASCII码）：\n");
    for (int i = 1; i <= n; i++) {
        printf("%c: %d\n", chars[i], w[i]);
    }
    printf("\n");
    
    HuffmanTree HT;
    HuffmanCode HC;
    
    // 构建哈夫曼树
    HuffmanCoding(HT, HC, w, n);
    
    // 打印哈夫曼树
    PrintHuffmanTree(HT, n);
    
    // 打印编码表
    PrintHuffmanCode(HC, n, chars);
    
    // 编码测试
    char text[] = "ABCDE";
    Encode(HC, n, chars, text);
    
    // 解码测试
    char code[] = "110111000110";
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