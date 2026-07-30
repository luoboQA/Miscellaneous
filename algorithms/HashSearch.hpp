/*
 *（哈希函数，哈希表，冲突探测）。通过哈希函数将关键字映射到表中位置，冲突时使用线性探测解决。
 *
 * 哈希查找思路：
 * 1. 通过哈希函数计算关键字的哈希地址
 * 2. 若该地址为空，则查找失败
 * 3. 若该地址的关键字等于目标值，则查找成功
 * 4. 若发生冲突，则通过探测序列继续查找
 *
 * 查找算法	平均时间复杂度	最坏时间复杂度	空间复杂度	查找条件
 * 哈希查找	O(1)	         O(n)	        O(n)	    无序或有序均可
 * 
 * 哈希表示例（size=11，哈希函数：key % 11）
 * 插入序列：[12, 25, 37, 18, 29, 11, 33]
 * 
 * 第1步：插入12
 *   12 % 11 = 1 → 下标1空 → 存入下标1
 *   [_, 12, _, _, _, _, _, _, _, _, _]
 * 
 * 第2步：插入25
 *   25 % 11 = 3 → 下标3空 → 存入下标3
 *   [_, 12, _, 25, _, _, _, _, _, _, _]
 * 
 * 第3步：插入37
 *   37 % 11 = 4 → 下标4空 → 存入下标4
 *   [_, 12, _, 25, 37, _, _, _, _, _, _]
 * 
 * 第4步：插入18
 *   18 % 11 = 7 → 下标7空 → 存入下标7
 *   [_, 12, _, 25, 37, _, _, 18, _, _, _]
 * 
 * 第5步：插入29
 *   29 % 11 = 7 → 下标7已占（18）→ 线性探测：下标8空 → 存入下标8
 *   [_, 12, _, 25, 37, _, _, 18, 29, _, _]
 * 
 * 第6步：插入11
 *   11 % 11 = 0 → 下标0空 → 存入下标0
 *   [11, 12, _, 25, 37, _, _, 18, 29, _, _]
 * 
 * 第7步：插入33
 *   33 % 11 = 0 → 下标0已占（11）→ 线性探测：下标1已占（12）→ 下标2空 → 存入下标2
 *   [11, 12, 33, 25, 37, _, _, 18, 29, _, _]
 * 
 * 最终哈希表：[11, 12, 33, 25, 37, _, _, 18, 29, _, _]
 * 
 * 查找过程：
 *   查找29：29%11=7 → 下标7=18≠29 → 下标8=29 → 找到，返回下标8
 *   查找5：5%11=5 → 下标5空 → 查找失败，返回-1
 *   查找33：33%11=0 → 下标0=11≠33 → 下标1=12≠33 → 下标2=33 → 找到，返回下标2
 * 
 * 冲突解决方式：
 *   线性探测：遇到冲突依次向后查找，简单但容易产生聚集
 *   平方探测：减少聚集，但可能跳过空位
 *   链地址法：每个桶使用链表，查找需遍历链表
 */


#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int KeyType;
typedef int ElemType;

/**
 * @brief 哈希表结构
 * 
 * @param elem   动态数组，存储哈希表的元素
 * @param count  当前哈希表中存储的元素个数
 * @param size   哈希表的大小（容量）
 */
typedef struct {
    ElemType* elem;
    int count;
    int size;
} HashTable;

/**
 * @brief 初始化哈希表
 * 
 * @param H     哈希表引用
 * @param size  哈希表大小
 * @return Status 初始化成功返回 OK，失败返回 OVERFLOW
 * 
 * @note 所有元素初始化为 -1（表示空位）
 * @note 时间复杂度 O(n)，空间复杂度 O(n)
 */
Status InitHash(HashTable& H, int size) {
    H.elem = (ElemType*)malloc(size * sizeof(ElemType));
    if (NULL == H.elem) {
        return OVERFLOW;
    }
    H.size = size;
    H.count = 0;
    for (int i = 0; i < size; i++) {
        H.elem[i] = -1;
    }
    return OK;
}

/**
 * @brief 销毁哈希表，释放内存
 * 
 * @param H 哈希表引用
 * @return Status 销毁成功返回 OK
 */
Status DestroyHash(HashTable& H) {
    if (H.elem != NULL) {
        free(H.elem);
        H.elem = NULL;
    }
    H.size = 0;
    H.count = 0;
    return OK;
}

/**
 * @brief 哈希函数（除留余数法）
 * 
 * @param key   关键字
 * @param size  哈希表大小
 * @return int  哈希地址
 * 
 * @note 除留余数法是最常用的哈希函数构造方法
 * @note 建议 size 为质数，可以减少冲突
 */
int Hash(KeyType key, int size) {
    return key % size;
}

/**
 * @brief 向哈希表中插入元素
 * 
 * @param H   哈希表引用
 * @param key 要插入的关键字
 * @return Status 插入成功返回 OK，表满返回 ERROR
 * 
 * @note 使用线性探测法解决哈希冲突
 * @note 时间复杂度 O(1) 平均，O(n) 最坏
 */
Status InsertHash(HashTable& H, KeyType key) {
    if (H.count >= H.size) {
        return ERROR;
    }
    
    int addr = Hash(key, H.size);
    
    while (H.elem[addr] != -1) {
        addr = (addr + 1) % H.size;
    }
    
    H.elem[addr] = key;
    H.count++;
    
    return OK;
}

/**
 * @brief 在哈希表中查找关键字
 * 
 * @param H   哈希表（只读）
 * @param key 要查找的关键字
 * @return int 查找成功返回元素的位置（下标），查找失败返回 -1
 * 
 * @note 线性探测查找：从哈希地址开始，若不等则向后探测
 * @note 遇到空位置（-1）表示查找失败
 * @note 平均时间复杂度 O(1)，最坏 O(n)
 */
int SearchHash(const HashTable& H, KeyType key) {
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