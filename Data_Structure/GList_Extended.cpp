#include <iostream>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef char AtomType;

// 广义表的扩展线性链表存储表示
typedef enum { ATOM, LIST } ElemTag;
// ATOM==0：原子，LIST==1：子表

typedef struct GLNode1 {
    ElemTag tag;
    union {
        AtomType atom;          // 原子结点的值域
        struct GLNode1 *hp;     // 表结点的表头指针
    } a;
    struct GLNode1 *tp;         // 相当于线性链表的next，指向下一个元素结点
} GLNode1, *GList1;

// ========== 扩展线性链表的基本操作 ==========

// 1. 创建原子结点
GList1 MakeAtom1(AtomType e) {
    GList1 p = (GList1)malloc(sizeof(GLNode1));
    if (!p) return NULL;
    p->tag = ATOM;
    p->a.atom = e;
    p->tp = NULL;
    return p;
}

// 2. 创建表结点
GList1 MakeList1(GList1 hp, GList1 tp) {
    GList1 p = (GList1)malloc(sizeof(GLNode1));
    if (!p) return NULL;
    p->tag = LIST;
    p->a.hp = hp;
    p->tp = tp;
    return p;
}

// 3. 求广义表的深度
int GListDepth1(GList1 L) {
    if (NULL == L) return 1;
    if (ATOM == L->tag) return 0;
    
    int maxDepth = 0;
    GList1 p = L;
    while (p) {
        if (LIST == p->tag) {
            int depth = GListDepth1(p->a.hp);
            if (depth > maxDepth) maxDepth = depth;
        }
        p = p->tp;
    }
    return maxDepth + 1;
}

// 4. 求广义表的长度
int GListLength1(GList1 L) {
    if (NULL == L) return 0;
    if (ATOM == L->tag) return 1;
    
    int len = 0;
    GList1 p = L;
    while (p) {
        len++;
        p = p->tp;
    }
    return len;
}

// 5. 取表头
GList1 GetHead1(GList1 L) {
    if (NULL == L || ATOM == L->tag) return NULL;
    return L->a.hp;
}

// 6. 取表尾
GList1 GetTail1(GList1 L) {
    if (NULL == L || ATOM == L->tag) return NULL;
    return L->tp;
}

// 7. 打印广义表
void PrintGList1(GList1 L) {
    if (NULL == L) {
        printf("()");
        return;
    }
    
    if (ATOM == L->tag) {
        printf("%c", L->a.atom);
        return;
    }
    
    printf("(");
    GList1 p = L;
    while (p) {
        if (LIST == p->tag) {
            PrintGList1(p->a.hp);
        } else {
            printf("%c", p->a.atom);
        }
        if (p->tp) printf(",");
        p = p->tp;
    }
    printf(")");
}

// 8. 复制广义表
Status CopyGList1(GList1 &T, GList1 L) {
    if (NULL == L) {
        T = NULL;
        return OK;
    }
    
    T = (GList1)malloc(sizeof(GLNode1));
    if (!T) return OVERFLOW;
    T->tag = L->tag;
    
    if (ATOM == L->tag) {
        T->a.atom = L->a.atom;
    } else {
        CopyGList1(T->a.hp, L->a.hp);
    }
    CopyGList1(T->tp, L->tp);
    return OK;
}

// 9. 销毁广义表
Status DestroyGList1(GList1 &L) {
    if (NULL == L) return OK;
    
    if (LIST == L->tag) {
        DestroyGList1(L->a.hp);
    }
    DestroyGList1(L->tp);
    free(L);
    L = NULL;
    return OK;
}

// 10. 创建示例广义表
GList1 CreateSimpleGList1() {
    // 创建广义表: (a,(b,c),d)
    GList1 atomB = MakeAtom1('b');
    GList1 atomC = MakeAtom1('c');
    GList1 listBC = MakeList1(atomB, atomC);
    
    GList1 atomA = MakeAtom1('a');
    GList1 atomD = MakeAtom1('d');
    
    GList1 list = MakeList1(atomA, MakeList1(listBC, atomD));
    return list;
}


int main() {
    printf("========================================\n");
    printf("    广义表-扩展线性链表存储表示\n");
    printf("========================================\n\n");
    
    GList1 L = CreateSimpleGList1();
    
    printf("广义表：");
    PrintGList1(L);
    printf("\n");
    
    printf("长度：%d\n", GListLength1(L));
    printf("深度：%d\n", GListDepth1(L));
    
    // 取表头
    GList1 head = GetHead1(L);
    printf("表头：");
    PrintGList1(head);
    printf("\n");
    
    // 取表尾
    GList1 tail = GetTail1(L);
    printf("表尾：");
    PrintGList1(tail);
    printf("\n");
    
    // 复制
    GList1 L2;
    CopyGList1(L2, L);
    printf("复制后：");
    PrintGList1(L2);
    printf("\n");
    
    DestroyGList1(L);
    DestroyGList1(L2);
    
    printf("\n");
    printf("【两种存储结构对比】\n");
    printf("广义表头尾链表：hp指向表头，tp指向表尾（递归嵌套）\n");
    printf("广义表扩展线性链表：hp指向子表，tp指向下一个兄弟结点\n");
    printf("扩展线性链表更适合遍历和操作\n");
    


    getchar();
    return 0;
}