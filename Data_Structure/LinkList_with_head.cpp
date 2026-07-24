/**
* 说明：本程序实现了一个具有头结点、并记录头尾指针的单链表
* hp指向第一个数据节点，tp指向最后一个数据节点
* 这样入队操作就是O(1)，不需要遍历
*/

#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"

// 常量定义
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

// 类型定义
typedef int Status;
typedef int ElemType;

// 测试程序长度定义
#define LENGTH 5

// 链表节点类型（保持不变）
typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

// 链表结构体（新增：记录头尾指针）
typedef struct {
    LinkList head;  // 头结点（固定不变）
    LinkList hp;    // 指向第一个数据节点
    LinkList tp;    // 指向最后一个数据节点
} LinkList_with_head;

// 创建包含n个元素的链表L
Status create(LinkList_with_head &L, ElemType *data, int n) {
    if (n < 0) return ERROR;
    
    // 创建头结点
    L.head = (LNode *)malloc(sizeof(LNode));
    if (NULL == L.head) return OVERFLOW;
    L.head->next = NULL;
    
    // 初始化头尾指针
    L.hp = NULL;
    L.tp = NULL;
    
    LinkList p = L.head;
    
    // 依次创建数据节点
    for (int i = 0; i < n; i++) {
        LinkList q = (LNode *)malloc(sizeof(LNode));
        if (NULL == q) return OVERFLOW;
        q->data = data[i];
        q->next = NULL;
        
        p->next = q;
        p = q;
        
        // 更新头尾指针
        if (i == 0) {
            L.hp = q;  // 第一个数据节点
        }
        L.tp = q;      // 最后一个数据节点
    }
    
    return OK;
}

// e从链表末尾入链表（O(1)时间）
Status EnQueue_LQ(LinkList_with_head &L, ElemType e) {
    LinkList q = (LNode *)malloc(sizeof(LNode));
    if (NULL == q) return OVERFLOW;
    q->data = e;
    q->next = NULL;
    
    if (NULL == L.head) {
        // 链表不存在
        return ERROR;
    }
    
    if (NULL == L.hp) {
        // 链表为空，只有头结点
        L.head->next = q;
        L.hp = q;
        L.tp = q;
    } else {
        // 链表非空，直接在尾节点后插入
        L.tp->next = q;
        L.tp = q;  // 更新尾指针
    }
    
    return OK;
}

// 从链表头节点出链表到e
Status DeQueue_LQ(LinkList_with_head &L, ElemType &e) {
    if (NULL == L.head || NULL == L.hp) return ERROR;
    
    LinkList p = L.hp;  // 第一个数据节点
    e = p->data;
    
    L.head->next = p->next;  // 头结点指向第二个数据节点
    
    if (p == L.tp) {
        // 如果只有一个数据节点
        L.hp = NULL;
        L.tp = NULL;
    } else {
        L.hp = p->next;  // 更新头指针
    }
    
    free(p);
    return OK;
}

// 获取链表长度（利用hp和tp可以快速判断是否为空）
int GetLength(LinkList_with_head L) {
    if (NULL == L.hp) return 0;
    
    int count = 0;
    LinkList p = L.hp;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

// 遍历调用
Status visit(ElemType e) {
    printf("%d\t", e);
    return OK;
}

// 遍历单链表
void ListTraverse_L(LinkList_with_head L, Status(*visit)(ElemType e)) {
    if (NULL == L.head || NULL == L.hp) {
        printf("链表为空\n");
        return;
    }
    
    printf("链表元素（共%d个）：", GetLength(L));
    for (LinkList p = L.hp; NULL != p; p = p->next) {
        visit(p->data);
    }
    printf("\n");
}

// 打印链表状态（调试用）
void PrintStatus(LinkList_with_head L) {
    printf("【状态】hp指向：");
    if (NULL == L.hp) {
        printf("NULL");
    } else {
        printf("%d", L.hp->data);
    }
    printf("，tp指向：");
    if (NULL == L.tp) {
        printf("NULL");
    } else {
        printf("%d", L.tp->data);
    }
    printf("\n");
}

int main() {
    int i;
    ElemType e, data[LENGTH] = {1, 2, 3, 4, 5};
    LinkList_with_head L;
    
    // 初始化
    L.head = NULL;
    L.hp = NULL;
    L.tp = NULL;
    
    // 显示测试值
    printf("---【带头结点+头尾指针的单链表】---\n");
    printf("特点：hp指向第一个数据节点，tp指向最后一个数据节点\n");
    printf("入队操作O(1)，无需遍历\n\n");
    
    printf("待测试元素为：\n");
    for (i = 0; i < LENGTH; i++) printf("%d\t", data[i]);
    printf("\n\n");
    
    // 创建链表L
    printf("创建链表L\n");
    if (ERROR == create(L, data, LENGTH)) {
        printf("创建链表L失败\n");
        return -1;
    }
    printf("成功创建包含1个头结点、%d个元素的链表L\n", LENGTH);
    PrintStatus(L);
    
    // 遍历单链表
    ListTraverse_L(L, visit);
    
    // 从链表头出链表
    printf("\n出链表到e\n");
    if (OK == DeQueue_LQ(L, e)) {
        printf("出链表的元素为：%d\n", e);
        PrintStatus(L);
        ListTraverse_L(L, visit);
    }
    
    // e从链表末尾入链表
    printf("\ne入链表\n");
    if (OK == EnQueue_LQ(L, e)) {
        printf("入链表的元素为：%d\n", e);
        PrintStatus(L);
        ListTraverse_L(L, visit);
    }
    
    // 再入一个元素
    printf("\n再入一个元素 6\n");
    e = 6;
    if (OK == EnQueue_LQ(L, e)) {
        PrintStatus(L);
        ListTraverse_L(L, visit);
    }
    
    printf("\n");
    getchar();
    return 0;
}