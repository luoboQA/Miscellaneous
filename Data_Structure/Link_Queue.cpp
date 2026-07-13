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

typedef struct QNode {
    ElemType data;
    struct QNode *next;
} QNode, *QueuePtr;

typedef struct {
    QueuePtr front;
    QueuePtr rear;
} LinkQueue;

Status InitQueue_L(LinkQueue &Q) {
    Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
    if (NULL == Q.front) return OVERFLOW;
    Q.front->next = NULL;
    return OK;
}

Status DestroyQueue_L(LinkQueue &Q) {
    while (Q.front) {
        Q.rear = Q.front->next;
        free(Q.front);
        Q.front = Q.rear;
    }
    return OK;
}

Status QueueEmpty_L(LinkQueue Q) {
    if (Q.front == Q.rear) return TRUE;
    return FALSE;
}

Status ClearQueue_L(LinkQueue &Q) {
    QueuePtr p, q;
    p = Q.front->next;
    while (p) {
        q = p;
        p = p->next;
        free(q);
    }
    Q.rear = Q.front;
    Q.front->next = NULL;
    return OK;
}

int QueueLength_L(LinkQueue Q) {
    int len = 0;
    QueuePtr p = Q.front->next;
    while (p) {
        len++;
        p = p->next;
    }
    return len;
}

Status EnQueue_L(LinkQueue &Q, ElemType e) {
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if (NULL == p) return OVERFLOW;
    p->data = e;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return OK;
}

Status DeQueue_L(LinkQueue &Q, ElemType &e) {
    if (QueueEmpty_L(Q)) return ERROR;
    QueuePtr p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if (Q.rear == p) Q.rear = Q.front;
    free(p);
    return e;
}

Status GetHead_L(LinkQueue Q, ElemType &e) {
    if (QueueEmpty_L(Q)) return ERROR;
    e = Q.front->next->data;
    return e;
}

int main() {
    LinkQueue Q;
    int i;
    ElemType e, eArray[LONGTH] = {1, 2, 3, 4, 5};

    printf("---【链式队列】---\n");

    if (!InitQueue_L(Q)) {
        printf("初始化链式队列失败\n");
        exit(0);
    }
    printf("已初始化链式队列\n");

    for (i = 0; i < LONGTH; i++) {
        if (!EnQueue_L(Q, eArray[i])) {
            printf("%d入队失败\n", eArray[i]);
            exit(0);
        }
    }
    printf("已入队\n");

    if (QueueEmpty_L(Q)) printf("队列为空\n");
    else printf("队列非空\n");

    printf("队列长度：%d\n", QueueLength_L(Q));

    GetHead_L(Q, e);
    printf("队头元素：%d\n", e);

    printf("出队元素：\n");
    for (i = 0; i < LONGTH; i++) {
        DeQueue_L(Q, e);
        printf("%d\t", e);
    }
    printf("\n");

    ClearQueue_L(Q);
    printf("已清空队列\n");
    printf("队列长度：%d\n", QueueLength_L(Q));

    DestroyQueue_L(Q);

    getchar();
    return 0;
}