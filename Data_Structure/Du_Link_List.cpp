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

typedef struct DuLNode {
    ElemType data;
    struct DuLNode *prior;
    struct DuLNode *next;
} DuLNode, *DuLinkList;

Status InitList_Du(DuLinkList &L) {
    L = (DuLinkList)malloc(sizeof(DuLNode));
    if (NULL == L) return OVERFLOW;
    L->prior = NULL;
    L->next = NULL;
    return OK;
}

Status DestroyList_Du(DuLinkList &L) {
    DuLinkList p = L;
    while (p) {
        DuLinkList q = p;
        p = p->next;
        free(q);
    }
    L = NULL;
    return OK;
}

Status ListEmpty_Du(DuLinkList L) {
    if (L->next == NULL) return TRUE;
    return FALSE;
}

Status ClearList_Du(DuLinkList &L) {
    DuLinkList p = L->next;
    while (p) {
        DuLinkList q = p;
        p = p->next;
        free(q);
    }
    L->next = NULL;
    return OK;
}

int ListLength_Du(DuLinkList L) {
    int len = 0;
    DuLinkList p = L->next;
    while (p) {
        len++;
        p = p->next;
    }
    return len;
}

Status GetElem_Du(DuLinkList L, int pos, ElemType &e) {
    if (pos < 1) return ERROR;
    DuLinkList p = L->next;
    int j = 1;
    while (p && j < pos) {
        p = p->next;
        j++;
    }
    if (!p || j > pos) return ERROR;
    e = p->data;
    return e;
}

Status ListInsert_Du(DuLinkList &L, int pos, ElemType e) {
    if (pos < 1) return ERROR;
    DuLinkList p = L;
    int j = 0;
    while (p && j < pos - 1) {
        p = p->next;
        j++;
    }
    if (!p || j > pos - 1) return ERROR;
    DuLinkList s = (DuLinkList)malloc(sizeof(DuLNode));
    if (NULL == s) return OVERFLOW;
    s->data = e;
    s->prior = p;
    s->next = p->next;
    if (p->next) p->next->prior = s;
    p->next = s;
    return OK;
}

Status ListDelete_Du(DuLinkList &L, int pos, ElemType &e) {
    if (pos < 1) return ERROR;
    DuLinkList p = L;
    int j = 0;
    while (p->next && j < pos - 1) {
        p = p->next;
        j++;
    }
    if (!p->next || j > pos - 1) return ERROR;
    DuLinkList q = p->next;
    e = q->data;
    p->next = q->next;
    if (q->next) q->next->prior = p;
    free(q);
    return e;
}

Status PriorElem_Du(DuLinkList L, ElemType cur_e, ElemType &pre_e) {
    DuLinkList p = L->next;
    while (p && p->data != cur_e) {
        p = p->next;
    }
    if (!p || !p->prior || p->prior == L) return ERROR;
    pre_e = p->prior->data;
    return pre_e;
}

Status NextElem_Du(DuLinkList L, ElemType cur_e, ElemType &next_e) {
    DuLinkList p = L->next;
    while (p && p->data != cur_e) {
        p = p->next;
    }
    if (!p || !p->next) return ERROR;
    next_e = p->next->data;
    return next_e;
}

Status ListTraverse_Du(DuLinkList L) {
    DuLinkList p = L->next;
    while (p) {
        printf("%d\t", p->data);
        p = p->next;
    }
    printf("\n");
    return OK;
}

int main() {
    DuLinkList L;
    int i, pos;
    ElemType e, eArray[LONGTH] = {1, 2, 3, 4, 5};

    printf("---【双向链表】---\n");

    if (!InitList_Du(L)) {
        printf("初始化双向链表失败\n");
        exit(0);
    }
    printf("已初始化双向链表\n");

    for (i = 0; i < LONGTH; i++) {
        if (!ListInsert_Du(L, i + 1, eArray[i])) {
            printf("%d插入失败\n", eArray[i]);
            exit(0);
        }
    }
    printf("已插入元素\n");

    printf("链表元素：");
    ListTraverse_Du(L);

    if (ListEmpty_Du(L)) printf("链表为空\n");
    else printf("链表非空\n");

    printf("链表长度：%d\n", ListLength_Du(L));

    pos = 3;
    GetElem_Du(L, pos, e);
    printf("第%d个元素：%d\n", pos, e);

    PriorElem_Du(L, 3, e);
    printf("元素3的前驱：%d\n", e);

    NextElem_Du(L, 3, e);
    printf("元素3的后继：%d\n", e);

    pos = 3;
    ListDelete_Du(L, pos, e);
    printf("删除第%d个元素：%d\n", pos, e);

    printf("删除后链表：");
    ListTraverse_Du(L);

    ClearList_Du(L);
    printf("已清空链表\n");

    DestroyList_Du(L);

    getchar();
    return 0;
}