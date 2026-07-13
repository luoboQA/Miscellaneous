#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define LONGTH 5

typedef int Status;
typedef int ElemType;

typedef struct CLNode {
    ElemType data;
    struct CLNode *next;
} CLNode, *CLinkList;

Status InitList_Cir(CLinkList &L) {
    L = (CLinkList)malloc(sizeof(CLNode));
    if (NULL == L) return OVERFLOW;
    L->next = L;
    return OK;
}

Status DestroyList_Cir(CLinkList &L) {
    if (!L) return ERROR;
    CLNode *p = L->next;
    while (p != L) {
        CLNode *q = p;
        p = p->next;
        free(q);
    }
    free(L);
    L = NULL;
    return OK;
}

Status ListEmpty_Cir(CLinkList L) {
    if (L->next == L) return TRUE;
    return FALSE;
}

Status ClearList_Cir(CLinkList &L) {
    CLNode *p = L->next;
    while (p != L) {
        CLNode *q = p;
        p = p->next;
        free(q);
    }
    L->next = L;
    return OK;
}

int ListLength_Cir(CLinkList L) {
    int len = 0;
    CLNode *p = L->next;
    while (p != L) {
        len++;
        p = p->next;
    }
    return len;
}

Status GetElem_Cir(CLinkList L, int pos, ElemType &e) {
    if (pos < 1) return ERROR;
    CLNode *p = L->next;
    int j = 1;
    while (p != L && j < pos) {
        p = p->next;
        j++;
    }
    if (p == L || j > pos) return ERROR;
    e = p->data;
    return e;
}

Status ListInsert_Cir(CLinkList &L, int pos, ElemType e) {
    if (pos < 1) return ERROR;
    CLNode *p = L;
    int j = 0;
    while (p->next != L && j < pos - 1) {
        p = p->next;
        j++;
    }
    if (j > pos - 1) return ERROR;
    CLNode *s = (CLNode *)malloc(sizeof(CLNode));
    if (NULL == s) return OVERFLOW;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

Status ListDelete_Cir(CLinkList &L, int pos, ElemType &e) {
    if (pos < 1 || ListEmpty_Cir(L)) return ERROR;
    CLNode *p = L;
    int j = 0;
    while (p->next != L && j < pos - 1) {
        p = p->next;
        j++;
    }
    if (j > pos - 1 || p->next == L) return ERROR;
    CLNode *q = p->next;
    e = q->data;
    p->next = q->next;
    free(q);
    return e;
}

Status ListTraverse_Cir(CLinkList L) {
    CLNode *p = L->next;
    while (p != L) {
        printf("%d\t", p->data);
        p = p->next;
    }
    printf("\n");
    return OK;
}

Status ListInsertHead_Cir(CLinkList &L, ElemType e) {
    CLNode *s = (CLNode *)malloc(sizeof(CLNode));
    if (NULL == s) return OVERFLOW;
    s->data = e;
    s->next = L->next;
    L->next = s;
    return OK;
}

Status ListInsertTail_Cir(CLinkList &L, ElemType e) {
    CLNode *p = L;
    while (p->next != L) {
        p = p->next;
    }
    CLNode *s = (CLNode *)malloc(sizeof(CLNode));
    if (NULL == s) return OVERFLOW;
    s->data = e;
    s->next = L;
    p->next = s;
    return OK;
}

Status ListMerge_Cir(CLinkList &La, CLinkList &Lb) {
    CLNode *pa = La;
    CLNode *pb = Lb;
    while (pa->next != La) pa = pa->next;
    while (pb->next != Lb) pb = pb->next;
    pa->next = Lb->next;
    pb->next = La;
    free(Lb);
    Lb = NULL;
    return OK;
}

int main() {
    CLinkList L, L2;
    int i, pos;
    ElemType e, eArray[LONGTH] = {1, 2, 3, 4, 5};

    printf("---【循环链表】---\n");

    if (!InitList_Cir(L)) {
        printf("初始化循环链表失败\n");
        exit(0);
    }
    printf("已初始化循环链表\n");

    for (i = 0; i < LONGTH; i++) {
        if (!ListInsertHead_Cir(L, eArray[i])) {
            printf("%d插入失败\n", eArray[i]);
            exit(0);
        }
    }
    printf("已头插元素\n");

    printf("链表元素：");
    ListTraverse_Cir(L);

    if (ListEmpty_Cir(L)) printf("链表为空\n");
    else printf("链表非空\n");

    printf("链表长度：%d\n", ListLength_Cir(L));

    pos = 3;
    GetElem_Cir(L, pos, e);
    printf("第%d个元素：%d\n", pos, e);

    pos = 3;
    ListDelete_Cir(L, pos, e);
    printf("删除第%d个元素：%d\n", pos, e);

    printf("删除后链表：");
    ListTraverse_Cir(L);

    printf("尾插元素6：");
    ListInsertTail_Cir(L, 6);
    ListTraverse_Cir(L);

    printf("\n---【循环链表合并】---\n");
    InitList_Cir(L2);
    for (i = 0; i < 3; i++) {
        ListInsertTail_Cir(L2, (i + 1) * 10);
    }
    printf("链表L2：");
    ListTraverse_Cir(L2);

    ListMerge_Cir(L, L2);
    printf("合并后链表L：");
    ListTraverse_Cir(L);

    ClearList_Cir(L);
    printf("已清空链表\n");

    DestroyList_Cir(L);

    getchar();
    return 0;
}