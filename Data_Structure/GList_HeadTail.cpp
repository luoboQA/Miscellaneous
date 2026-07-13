#include <iostream>
#include <cstring>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef char AtomType;

// 广义表的头尾链表存储表示
typedef enum { ATOM, LIST } ElemTag;
// ATOM==0：原子，LIST==1：子表

typedef struct GLNode {
    ElemTag tag;
    // 公共部分，用于区分原子结点和表结点
    union {
        AtomType atom;  // 原子结点的值域
        struct {
            struct GLNode *hp;  // 表头指针
            struct GLNode *tp;  // 表尾指针
        } ptr;
    } a;
} GLNode, *GList;

// ========== 广义表的基本操作 ==========

// 1. 创建空表
Status InitGList(GList &L) {
    L = NULL;
    return OK;
}

// 2. 创建原子结点
GList MakeAtom(AtomType e) {
    GList p = (GList)malloc(sizeof(GLNode));
    if (!p) return NULL;
    p->tag = ATOM;
    p->a.atom = e;
    return p;
}

// 3. 创建表结点（头尾链表）
GList MakeList(GList hp, GList tp) {
    GList p = (GList)malloc(sizeof(GLNode));
    if (!p) return NULL;
    p->tag = LIST;
    p->a.ptr.hp = hp;
    p->a.ptr.tp = tp;
    return p;
}

// 4. 求广义表的深度
int GListDepth(GList L) {
    if (NULL == L) return 1;
    if (ATOM == L->tag) return 0;
    
    int maxDepth = 0;
    GList p = L;
    while (p) {
        if (LIST == p->tag) {
            int depth = GListDepth(p->a.ptr.hp);
            if (depth > maxDepth) maxDepth = depth;
        }
        p = p->a.ptr.tp;
    }
    return maxDepth + 1;
}

// 5. 求广义表的长度
int GListLength(GList L) {
    if (NULL == L) return 0;
    if (ATOM == L->tag) return 1;
    
    int len = 0;
    GList p = L;
    while (p) {
        len++;
        p = p->a.ptr.tp;
    }
    return len;
}

// 6. 取表头
GList GetHead(GList L) {
    if (NULL == L || ATOM == L->tag) return NULL;
    return L->a.ptr.hp;
}

// 7. 取表尾
GList GetTail(GList L) {
    if (NULL == L || ATOM == L->tag) return NULL;
    return L->a.ptr.tp;
}

// 8. 打印广义表
void PrintGList(GList L) {
    if (NULL == L) {
        printf("()");
        return;
    }
    
    if (ATOM == L->tag) {
        printf("%c", L->a.atom);
        return;
    }
    
    printf("(");
    GList p = L;
    while (p) {
        if (LIST == p->tag) {
            PrintGList(p->a.ptr.hp);
        } else {
            printf("%c", p->a.atom);
        }
        if (p->a.ptr.tp) printf(",");
        p = p->a.ptr.tp;
    }
    printf(")");
}

// 9. 复制广义表
Status CopyGList(GList &T, GList L) {
    if (NULL == L) {
        T = NULL;
        return OK;
    }
    
    T = (GList)malloc(sizeof(GLNode));
    if (!T) return OVERFLOW;
    T->tag = L->tag;
    
    if (ATOM == L->tag) {
        T->a.atom = L->a.atom;
    } else {
        CopyGList(T->a.ptr.hp, L->a.ptr.hp);
        CopyGList(T->a.ptr.tp, L->a.ptr.tp);
    }
    return OK;
}

// 10. 销毁广义表
Status DestroyGList(GList &L) {
    if (NULL == L) return OK;
    
    if (LIST == L->tag) {
        DestroyGList(L->a.ptr.hp);
        DestroyGList(L->a.ptr.tp);
    }
    free(L);
    L = NULL;
    return OK;
}

// ========== 从字符串创建广义表 ==========

GList CreateGListFromString(const char* &s) {
    if (*s == '\0') return NULL;
    
    if (*s == '(') {
        s++;  // 跳过'('
        GList head = NULL, tail = NULL;
        
        if (*s == ')') {
            s++;  // 跳过')'
            return NULL;
        }
        
        // 创建表头
        if (*s == '(') {
            head = CreateGListFromString(s);
        } else if (*s >= 'a' && *s <= 'z') {
            head = MakeAtom(*s);
            s++;
        }
        
        // 处理表尾
        if (*s == ',') {
            s++;  // 跳过','
            tail = CreateGListFromString(s);
        }
        
        if (*s == ')') s++;  // 跳过')'
        return MakeList(head, tail);
    }
    return NULL;
}

// 简化版：从字符串创建（仅供演示）
GList CreateSimpleGList() {
    // 创建广义表: (a,(b,c),d)
    // 子表 (b,c)
    GList atomB = MakeAtom('b');
    GList atomC = MakeAtom('c');
    GList listBC = MakeList(atomB, MakeList(atomC, NULL));
    
    GList atomA = MakeAtom('a');
    GList atomD = MakeAtom('d');
    
    GList list = MakeList(atomA, MakeList(listBC, MakeList(atomD, NULL)));
    return list;
}

int main() {
    printf("========================================\n");
    printf("    广义表-头尾链表存储表示\n");
    printf("========================================\n\n");
    
    // 创建广义表
    GList L = CreateSimpleGList();
    
    printf("广义表：");
    PrintGList(L);
    printf("\n");
    
    printf("长度：%d\n", GListLength(L));
    printf("深度：%d\n", GListDepth(L));
    
    // 取表头
    GList head = GetHead(L);
    printf("表头：");
    PrintGList(head);
    printf("\n");
    
    // 取表尾
    GList tail = GetTail(L);
    printf("表尾：");
    PrintGList(tail);
    printf("\n");
    
    // 复制广义表
    GList L2;
    CopyGList(L2, L);
    printf("复制后的广义表：");
    PrintGList(L2);
    printf("\n");
    
    // 销毁
    DestroyGList(L);
    DestroyGList(L2);
    
    printf("\n");
    printf("【存储结构说明】\n");
    printf("tag=0：原子结点，atom存储数据\n");
    printf("tag=1：表结点，hp指向表头，tp指向表尾\n");
    printf("图示：每个表结点通过hp和tp形成树形结构\n");
    
    getchar();
    return 0;
}