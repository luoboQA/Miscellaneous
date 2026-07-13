// 查找算法	平均时间复杂度	空间复杂度	查找条件
// 哈希查找	    O(1)	    O(n)	无序或有序
#include <iostream>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int ElemType;

typedef struct {
    int *elem;
    int count;
    int size;
} HashTable;

// 初始化哈希表
Status InitHash(HashTable &H, int size) {
    H.elem = (int *)malloc(size * sizeof(int));
    if (NULL == H.elem) return OVERFLOW;
    H.size = size;
    H.count = 0;
    for (int i = 0; i < size; i++) {
        H.elem[i] = -1;
    }
    return OK;
}

// 销毁哈希表
Status DestroyHash(HashTable &H) {
    free(H.elem);
    H.elem = NULL;
    return OK;
}

// 哈希函数
int Hash(int key, int size) {
    return key % size;
}

// 插入哈希表（线性探测解决冲突）
Status InsertHash(HashTable &H, int key) {
    if (H.count >= H.size) return ERROR;
    int addr = Hash(key, H.size);
    while (H.elem[addr] != -1) {
        addr = (addr + 1) % H.size;
    }
    H.elem[addr] = key;
    H.count++;
    return OK;
}

// 哈希查找
int SearchHash(HashTable H, int key) {
    int addr = Hash(key, H.size);
    int start = addr;
    while (H.elem[addr] != key) {
        addr = (addr + 1) % H.size;
        if (addr == start || H.elem[addr] == -1) {
            return -1;
        }
    }
    return addr;
}

int main() {
    HashTable H;
    int i;

    printf("---【哈希查找】---\n");
    printf("平均时间复杂度：O(1)\n");
    printf("空间复杂度：O(n)\n");
    printf("查找条件：无序或有序\n\n");

    InitHash(H, 11);
    int hashArray[] = {12, 25, 38, 49, 50, 61, 72, 83};
    for (i = 0; i < 8; i++) {
        InsertHash(H, hashArray[i]);
    }
    printf("插入元素：");
    for (i = 0; i < 8; i++) {
        printf("%d\t", hashArray[i]);
    }
    printf("\n");

    int target = 50;
    int pos = SearchHash(H, target);
    printf("查找%d的位置：%d\n", target, pos);

    target = 99;
    pos = SearchHash(H, target);
    printf("查找%d的位置：%d\n", target, pos);

    DestroyHash(H);

    getchar();
    return 0;
}