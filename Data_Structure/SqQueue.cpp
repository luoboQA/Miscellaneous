/**
* 说明：本程序实现了一个顺序队列含循环和非循环两种实现方式。
*/
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

// 非循环队列
typedef struct {
    ElemType *elem;
    int front;
    int rear;
    int maxSize;
} SqQueue;

// 循环队列
typedef struct {
    ElemType *elem;
    int front;
    int rear;
    int maxSize;
} CirQueue;

// ========== 非循环队列 ==========

Status InitQueue_Sq(SqQueue &Q, int size) {
    Q.elem = (ElemType *)malloc(size * sizeof(ElemType));
    if (NULL == Q.elem) return OVERFLOW;
    Q.front = 0;
    Q.rear = 0;
    Q.maxSize = size;
    return OK;
}

Status DestroyQueue_Sq(SqQueue &Q) {
    free(Q.elem);
    Q.elem = NULL;
    return OK;
}

Status QueueEmpty_Sq(SqQueue Q) {
    if (Q.front == Q.rear) return TRUE;
    return FALSE;
}

Status QueueFull_Sq(SqQueue Q) {
    if (Q.rear >= Q.maxSize) return TRUE;
    return FALSE;
}

Status ClearQueue_Sq(SqQueue &Q) {
    Q.front = 0;
    Q.rear = 0;
    return OK;
}

int QueueLength_Sq(SqQueue Q) {
    return Q.rear - Q.front;
}

Status EnQueue_Sq(SqQueue &Q, ElemType e) {
    if (QueueFull_Sq(Q)) return ERROR;
    Q.elem[Q.rear++] = e;
    return OK;
}

Status DeQueue_Sq(SqQueue &Q, ElemType &e) {
    if (QueueEmpty_Sq(Q)) return ERROR;
    e = Q.elem[Q.front++];
    return e;
}

Status GetHead_Sq(SqQueue Q, ElemType &e) {
    if (QueueEmpty_Sq(Q)) return ERROR;
    e = Q.elem[Q.front];
    return e;
}

// ========== 循环队列 ==========

Status InitQueue_Cir(CirQueue &Q, int size) {
    Q.elem = (ElemType *)malloc((size + 1) * sizeof(ElemType));
    if (NULL == Q.elem) return OVERFLOW;
    Q.front = 0;
    Q.rear = 0;
    Q.maxSize = size + 1;
    return OK;
}

Status DestroyQueue_Cir(CirQueue &Q) {
    free(Q.elem);
    Q.elem = NULL;
    return OK;
}

Status QueueEmpty_Cir(CirQueue Q) {
    if (Q.front == Q.rear) return TRUE;
    return FALSE;
}

Status QueueFull_Cir(CirQueue Q) {
    if ((Q.rear + 1) % Q.maxSize == Q.front) return TRUE;
    return FALSE;
}

Status ClearQueue_Cir(CirQueue &Q) {
    Q.front = 0;
    Q.rear = 0;
    return OK;
}

int QueueLength_Cir(CirQueue Q) {
    return (Q.rear - Q.front + Q.maxSize) % Q.maxSize;
}

Status EnQueue_Cir(CirQueue &Q, ElemType e) {
    if (QueueFull_Cir(Q)) return ERROR;
    Q.elem[Q.rear] = e;
    Q.rear = (Q.rear + 1) % Q.maxSize;
    return OK;
}

Status DeQueue_Cir(CirQueue &Q, ElemType &e) {
    if (QueueEmpty_Cir(Q)) return ERROR;
    e = Q.elem[Q.front];
    Q.front = (Q.front + 1) % Q.maxSize;
    return e;
}

Status GetHead_Cir(CirQueue Q, ElemType &e) {
    if (QueueEmpty_Cir(Q)) return ERROR;
    e = Q.elem[Q.front];
    return e;
}

int main() {
    SqQueue Q;
    CirQueue CQ;
    int size, i;
    ElemType e, eArray[LONGTH] = {1, 2, 3, 4, 5};

    printf("---【非循环队列】---\n");
    size = LONGTH;

    if (!InitQueue_Sq(Q, size)) {
        printf("初始化非循环队列失败\n");
        exit(0);
    }
    printf("已初始化非循环队列\n");

    for (i = 0; i < size; i++) {
        if (!EnQueue_Sq(Q, eArray[i])) {
            printf("%d入队失败\n", eArray[i]);
            exit(0);
        }
    }
    printf("已入队\n");

    if (QueueEmpty_Sq(Q)) printf("队列为空\n");
    else printf("队列非空\n");

    printf("队列长度：%d\n", QueueLength_Sq(Q));

    GetHead_Sq(Q, e);
    printf("队头元素：%d\n", e);

    printf("出队元素：\n");
    for (i = 0; i < size; i++) {
        DeQueue_Sq(Q, e);
        printf("%d\t", e);
    }
    printf("\n");

    ClearQueue_Sq(Q);
    printf("已清空队列\n");

    // ========== 循环队列测试 ==========
    printf("\n---【循环队列】---\n");

    if (!InitQueue_Cir(CQ, size)) {
        printf("初始化循环队列失败\n");
        exit(0);
    }
    printf("已初始化循环队列\n");

    for (i = 0; i < size; i++) {
        if (!EnQueue_Cir(CQ, eArray[i])) {
            printf("%d入队失败\n", eArray[i]);
            exit(0);
        }
    }
    printf("已入队\n");

    if (QueueEmpty_Cir(CQ)) printf("队列为空\n");
    else printf("队列非空\n");

    printf("队列长度：%d\n", QueueLength_Cir(CQ));

    GetHead_Cir(CQ, e);
    printf("队头元素：%d\n", e);

    printf("出队元素：\n");
    for (i = 0; i < size; i++) {
        DeQueue_Cir(CQ, e);
        printf("%d\t", e);
    }
    printf("\n");

    ClearQueue_Cir(CQ);
    printf("已清空循环队列\n");

    DestroyQueue_Sq(Q);
    DestroyQueue_Cir(CQ);

    getchar();
    return 0;
}