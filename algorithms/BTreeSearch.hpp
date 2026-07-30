/*
 *（根节点，关键字，子树）。通过比较关键字与节点内关键字，递归地在对应子树中查找。
 *
 * B树/B+树查找思路：
 * 1. 从根节点开始，在节点内部顺序查找第一个大于等于 key 的位置 i
 * 2. 若 key 等于当前节点第 i 个关键字，查找成功（B树直接返回data）
 * 3. 若 key 小于当前节点第 i 个关键字，进入第 i-1 棵子树继续查找
 * 4. 若 key 大于节点内所有关键字，进入最后一棵子树继续查找
 * 5. 若到达叶子节点仍未找到，则查找失败
 *
 * 查找算法    平均时间复杂度    最差时间复杂度    空间复杂度    数据对象稳定性
 * B树查找     O(logₘ n)        O(logₘ n)         O(logₘ n)    多路平衡搜索树
 * B+树查找    O(logₘ n)        O(logₘ n)         O(logₘ n)    多路平衡搜索树
 * 
 * B树示例（3阶，每个节点最多2个关键字）：
 *              [30:100, 60:200]
 *              /      |        \
 *     [10:10, 20:20] [40:40, 50:50] [70:70, 80:80]
 * 
 * B树查找目标值 40：
 *   第1步：从根节点开始
 *     根节点关键字：[30, 60]
 *     40 > 30 → 继续比较
 *     40 < 60 → 进入第1棵子树（child[1]）
 *   第2步：当前节点 [40:40, 50:50]
 *     40 == 40 → 找到！返回 data=40
 * 
 * B树查找目标值 25（不存在）：
 *   第1步：从根节点开始
 *     根节点关键字：[30, 60]
 *     25 < 30 → 进入第0棵子树（child[0]）
 *   第2步：当前节点 [10:10, 20:20]
 *     25 > 20 → 大于所有关键字
 *     进入最后一棵子树（child[2]），但 child[2] = NULL
 *   第3步：到达空节点，查找失败
 * 
 * B+树示例（3阶，所有数据在叶子节点）：
 *              [30, 60]
 *              /   |   \
 *     [10, 20] [40, 50] [70, 80]
 *     (10:10,  (40:40,  (70:70,
 *      20:20)   50:50)   80:80)
 *         |        |        |
 *         └────────┼────────┘
 *                  ↓
 *              (next指针)
 * 
 * B+树查找目标值 50：
 *   第1步：从根节点开始（内部节点）
 *     根节点关键字：[30, 60]
 *     50 >= 30 → i=2
 *     50 < 60 → 进入第1棵子树（child[1]）
 *   第2步：当前节点 [40, 50]（内部节点）
 *     50 >= 40 → i=2
 *     50 >= 50 → i=3
 *     进入第2棵子树（child[2]）
 *   第3步：到达叶子节点 [70:70, 80:80]
 *     50 < 70 → 不在该叶子节点
 *     查找失败（说明数据不存在）
 * 
 * B+树查找目标值 70：
 *   第1步：从根节点开始
 *     70 >= 30 → i=2
 *     70 >= 60 → i=3
 *     进入第2棵子树（child[2]）
 *   第2步：当前节点 [70, 80]
 *     70 >= 70 → i=2
 *     70 < 80 → 进入第1棵子树（child[1]）
 *   第3步：到达叶子节点 [70:70, 80:80]
 *     70 == 70 → 找到！返回 data=70
 * 
 * B+树范围查找 [35, 75]：
 *   第1步：找到第一个 >= 35 的叶子节点
 *     从根节点开始：35 < 30？否，35 < 60？是 → 进入 child[0]
 *     从节点 [40, 50]：35 < 40？是 → 进入 child[0]
 *     到达叶子节点 [10:10, 20:20]：35 > 20 → 通过next指针继续
 *   第2步：遍历叶子节点链表
 *     [40:40, 50:50] → 40,50在范围内，输出
 *     [70:70, 80:80] → 70在范围内，80 > 75 停止
 *     结果：[40:40] [50:50] [70:70]
 * 
 * B树与B+树区别：
 *   B树：每个节点都存储数据，查找可能在内部节点就结束
 *   B+树：数据只在叶子节点，内部节点仅作索引，查找必须到达叶子节点
 *   B+树叶子节点通过next指针连接，支持高效范围查询
 */

using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1

typedef int Status;
typedef int KeyType;    // 关键字类型
typedef int DataType;   // 数据类型

/*
 * 性质：
 * 1. 每个节点最多有 M 个子树（M 阶）
 * 2. 每个节点最多有 M-1 个关键字
 * 3. 除根节点外，每个节点至少有 ⌈M/2⌉ 个子树
 * 4. 因此，除根节点外，每个节点至少有 ⌈M/2⌉ - 1 个关键字
 * 
 * 当 M 为奇数时：⌈M/2⌉ = (M+1)/2
 *   最少关键字 = (M+1)/2 - 1 = (M-1)/2
 * 
 * 当 M 为偶数时：⌈M/2⌉ = M/2
 *   最少关键字 = M/2 - 1
 * 
 * 代码中统一使用 (M-1)/2，向下取整
 * 证明：⌈M/2⌉ - 1 = (M-1)/2（整数除法）
 * 
 * 情况1：M 为奇数，M = 2k+1
 *   ⌈M/2⌉ - 1 = ⌈(2k+1)/2⌉ - 1 
 *              = (k+1) - 1        // 向上取整
 *              = k
 *   (M-1)/2 = (2k+1-1)/2 = 2k/2 = k
 *   
 * 
 * 情况2：M 为偶数，M = 2k
 *   ⌈M/2⌉ - 1 = ⌈2k/2⌉ - 1 
 *              = k - 1            // 向上取整
 *   (M-1)/2 = (2k-1)/2 = k - 0.5
 *   整数除法 k - 0.5 = k - 1      // 向下取整
 *   
 */

#define M 3             // B树/B+树的阶
#define MIN_KEY (M - 1) / 2   // 节点最少关键字个数


/* ==================== B树 ==================== */

/**
 * @brief B树节点结构体
 * 
 * @param keynum  当前关键字个数
 * @param key     关键字数组（下标从1开始）
 * @param data    与关键字对应的数据数组（下标从1开始）
 * @param child   子树指针数组（下标从0开始）
 * @param parent  双亲节点指针
 * @param isLeaf  是否为叶子节点（1为叶子，0为内部节点）
 */
typedef struct BTNode {
    int keynum;                 
    KeyType key[M + 1];        
    DataType data[M + 1];      
    struct BTNode *child[M + 1];
    struct BTNode *parent;
    int isLeaf;
} BTNode, *BTree;

/**
 * @brief 创建B树节点
 * 
 * @param isLeaf 是否为叶子节点（1:叶子，0:内部节点）
 * @return BTNode* 新创建的节点指针，若失败返回NULL
 */
BTNode* CreateBTNode(int isLeaf) {
    BTNode *p = (BTNode *)malloc(sizeof(BTNode));
    if (NULL == p) return NULL;
    p->keynum = 0;
    p->isLeaf = isLeaf;
    p->parent = NULL;
    for (int i = 0; i <= M; i++) {
        p->child[i] = NULL;
        p->key[i] = 0;
        p->data[i] = 0;
    }
    return p;
}

/**
 * @brief 在B树中查找关键字，并返回对应的数据
 * 
 * @param T     B树的根节点指针
 * @param key   要查找的关键字
 * @param data  查找成功时返回对应的数据
 * @return Status 查找成功返回 OK，失败返回 ERROR
 * 
 * @note 时间复杂度 O(logₘ n)，其中 m 为 B 树的阶
 * @note 空间复杂度 O(logₘ n)（递归调用栈）
 * @note 要求树满足 B 树性质：所有叶子节点在同一层
 * @note B树在每个节点都存储数据，找到 key 即可直接返回 data
 */
Status SearchBTree(BTree T, KeyType key, DataType &data) {
    if (NULL == T) return ERROR;
    
    int i = 1;
    while (i <= T->keynum && key > T->key[i]) {
        i++;
    }
    
    if (i <= T->keynum && key == T->key[i]) {
        data = T->data[i];
        return OK;
    }
    
    if (T->isLeaf) return ERROR;
    return SearchBTree(T->child[i - 1], key, data);
}


/* ==================== B+树 ==================== */

/**
 * @brief B+树节点结构体
 * 
 * @param keynum  当前关键字个数
 * @param key     关键字数组（下标从1开始）
 * @param child   子树指针数组（下标从0开始），叶子节点为 NULL
 * @param parent  双亲节点指针
 * @param isLeaf  是否为叶子节点（1为叶子，0为内部节点）
 * @param next    叶子节点链表指针（仅叶子节点使用，指向下一个叶子节点）
 * @param data    数据数组（仅叶子节点使用，下标从1开始）
 */
typedef struct BPlusNode {
    int keynum;
    KeyType key[M + 1];
    struct BPlusNode *child[M + 1];
    struct BPlusNode *parent;
    int isLeaf;
    struct BPlusNode *next;      // 叶子节点链表指针
    DataType data[M + 1];        // 仅叶子节点有效
} BPlusNode, *BPlusTree;

/**
 * @brief 创建B+树节点
 * 
 * @param isLeaf 是否为叶子节点（1:叶子，0:内部节点）
 * @return BPlusNode* 新创建的节点指针，若失败返回NULL
 */
BPlusNode* CreateBPlusNode(int isLeaf) {
    BPlusNode *p = (BPlusNode *)malloc(sizeof(BPlusNode));
    if (NULL == p) return NULL;
    p->keynum = 0;
    p->isLeaf = isLeaf;
    p->parent = NULL;
    p->next = NULL;
    for (int i = 0; i <= M; i++) {
        p->child[i] = NULL;
        p->key[i] = 0;
        p->data[i] = 0;
    }
    return p;
}

/**
 * @brief 在B+树中查找关键字，并返回对应的数据
 * 
 * @param T     B+树的根节点指针
 * @param key   要查找的关键字
 * @param data  查找成功时返回对应的数据
 * @return Status 查找成功返回 OK，失败返回 ERROR
 * 
 * @note 时间复杂度 O(logₘ n)，其中 m 为 B+树的阶
 * @note 空间复杂度 O(logₘ n)（递归调用栈）
 * @note 要求树满足 B+树性质：所有数据在叶子节点，内部节点仅存索引
 * @note 查找必须到达叶子节点才能获取数据
 */
Status SearchBPlusTree(BPlusTree T, KeyType key, DataType &data) {
    if (NULL == T) return ERROR;
    
    if (T->isLeaf) {
        int i = 1;
        while (i <= T->keynum && key > T->key[i]) {
            i++;
        }
        if (i <= T->keynum && key == T->key[i]) {
            data = T->data[i];
            return OK;
        }
        return ERROR;
    }
    
    int i = 1;
    while (i <= T->keynum && key >= T->key[i]) {
        i++;
    }
    return SearchBPlusTree(T->child[i - 1], key, data);
}

/**
 * @brief 在B+树中范围查找（返回 [low, high] 区间内的所有数据）
 * 
 * @param T     B+树的根节点指针
 * @param low   范围下限
 * @param high  范围上限
 * 
 * @note 时间复杂度 O(logₘ n + k)，其中 k 为结果数量
 * @note 利用叶子节点链表，可高效进行范围查询
 */
void RangeSearchBPlusTree(BPlusTree T, KeyType low, KeyType high) {
    if (NULL == T || low > high) return;
    
    BPlusNode *p = T;
    while (!p->isLeaf) {
        int i = 1;
        while (i <= p->keynum && low >= p->key[i]) {
            i++;
        }
        p = p->child[i - 1];
    }
    
    printf("范围查找 [%d, %d]：", low, high);
    while (p) {
        for (int i = 1; i <= p->keynum; i++) {
            if (p->key[i] >= low && p->key[i] <= high) {
                printf("[%d:%d] ", p->key[i], p->data[i]);
            }
            if (p->key[i] > high) {
                printf("\n");
                return;
            }
        }
        p = p->next;
    }
    printf("\n");
}

/**
 * @brief 中序遍历B+树（输出 key:data）
 * 
 * @param T B+树的根节点指针
 * 
 * @note 中序遍历会按关键字升序输出所有 key-data 对
 * @note 只有叶子节点才存储数据，内部节点只输出索引信息
 */
void InOrderBPlusTree(BPlusTree T) {
    if (T) {
        if (T->isLeaf) {
            for (int i = 1; i <= T->keynum; i++) {
                printf("[%d:%d] ", T->key[i], T->data[i]);
            }
        } else {
            for (int i = 0; i <= T->keynum; i++) {
                InOrderBPlusTree(T->child[i]);
            }
        }
    }
}

/**
 * @brief 释放B树内存
 * 
 * @param T B树的根节点指针
 */
void FreeBTree(BTree T) {
    if (T) {
        if (!T->isLeaf) {
            for (int i = 0; i <= T->keynum; i++) {
                FreeBTree(T->child[i]);
            }
        }
        free(T);
    }
}

/**
 * @brief 释放B+树内存
 * 
 * @param T B+树的根节点指针
 */
void FreeBPlusTree(BPlusTree T) {
    if (T) {
        if (!T->isLeaf) {
            for (int i = 0; i <= T->keynum; i++) {
                FreeBPlusTree(T->child[i]);
            }
        }
        free(T);
    }
}

/**
 * @brief B树插入核心递归函数
 * 
 * @param T      当前节点指针
 * @param key    要插入的关键字
 * @param data   要插入的数据
 * @param upKey  输出参数，返回需要上升到父节点的关键字
 * @param upData 输出参数，返回需要上升到父节点的数据
 * @param right  输出参数，返回分裂后新创建的右子树指针
 * @return int 返回1表示插入成功且节点未满，返回0表示节点已满需要分裂
 */
int InsertBTreeCore(BTree T, KeyType key, DataType data, 
                     KeyType &upKey, DataType &upData, BTree &right) {
    if (!T) {
        upKey = key;
        upData = data;
        right = NULL;
        return 0;
    }
    
    int i = 1;
    while (i <= T->keynum && key > T->key[i]) i++;
    
    if (i <= T->keynum && key == T->key[i]) {
        T->data[i] = data;
        return 1;
    }
    
    if (!InsertBTreeCore(T->child[i-1], key, data, upKey, upData, right)) {
        if (T->keynum < M-1) {
            int j = T->keynum;
            while (j >= 1 && upKey < T->key[j]) {
                T->key[j+1] = T->key[j];
                T->data[j+1] = T->data[j];
                T->child[j+1] = T->child[j];
                j--;
            }
            T->key[j+1] = upKey;
            T->data[j+1] = upData;
            T->child[j+1] = right;
            if (right) right->parent = T;
            T->keynum++;
            return 1;
        }
        
        KeyType tempKey[M];
        DataType tempData[M];
        BTree tempChild[M+1];
        
        for (int j = 1; j <= M-1; j++) {
            tempKey[j] = T->key[j];
            tempData[j] = T->data[j];
            tempChild[j] = T->child[j];
        }
        tempChild[0] = T->child[0];
        
        int j = M-1;
        while (j >= 1 && upKey < tempKey[j]) {
            tempKey[j+1] = tempKey[j];
            tempData[j+1] = tempData[j];
            tempChild[j+1] = tempChild[j];
            j--;
        }
        tempKey[j+1] = upKey;
        tempData[j+1] = upData;
        tempChild[j+1] = right;
        if (right) right->parent = T;
        
        int mid = (M+1) / 2;
        BTree newNode = CreateBTNode(T->isLeaf);
        newNode->parent = T->parent;
        
        T->keynum = mid-1;
        for (int k = 1; k <= mid-1; k++) {
            T->key[k] = tempKey[k];
            T->data[k] = tempData[k];
            T->child[k] = tempChild[k];
            if (T->child[k]) T->child[k]->parent = T;
        }
        T->child[0] = tempChild[0];
        if (T->child[0]) T->child[0]->parent = T;
        
        int idx = 1;
        for (int k = mid+1; k <= M; k++) {
            newNode->key[idx] = tempKey[k];
            newNode->data[idx] = tempData[k];
            newNode->child[idx-1] = tempChild[k-1];
            if (newNode->child[idx-1]) newNode->child[idx-1]->parent = newNode;
            idx++;
        }
        newNode->child[idx-1] = tempChild[M];
        if (newNode->child[idx-1]) newNode->child[idx-1]->parent = newNode;
        newNode->keynum = M-mid;
        
        upKey = tempKey[mid];
        upData = tempData[mid];
        right = newNode;
        return 0;
    }
    return 1;
}

/**
 * @brief B树插入入口
 * 
 * @param T    B树的根节点指针（引用），插入后根节点可能改变
 * @param key  要插入的关键字
 * @param data 要插入的数据
 * @return Status 插入成功返回 OK
 */
Status InsertBTree(BTree &T, KeyType key, DataType data) {
    if (!T) {
        T = CreateBTNode(1);
        T->key[++T->keynum] = key;
        T->data[T->keynum] = data;
        return OK;
    }
    
    KeyType upKey, upData;
    BTree right;
    if (!InsertBTreeCore(T, key, data, upKey, upData, right)) {
        BTree newRoot = CreateBTNode(0);
        newRoot->key[1] = upKey;
        newRoot->data[1] = upData;
        newRoot->keynum = 1;
        newRoot->child[0] = T;
        newRoot->child[1] = right;
        T->parent = newRoot;
        if (right) right->parent = newRoot;
        T = newRoot;
    }
    return OK;
}


/* ==================== B+树插入 ==================== */

/**
 * @brief B+树插入内部节点辅助函数
 * 
 * @param node   当前内部节点指针
 * @param key    要插入的关键字
 * @param right  插入key对应的右子树指针
 * @param upKey  输出参数，返回需要上升到父节点的关键字
 * @param upNode 输出参数，返回分裂后新创建的节点指针
 * @return int 返回1表示插入成功且节点未满，返回0表示节点已满需要分裂
 * 
 * @note B+树内部节点只存key（不存data）
 *       分裂时只上升key，data只存在叶子节点
 */
int InsertBPlusInternalCore(BPlusNode *node, KeyType key, BPlusNode *right,
                             KeyType &upKey, BPlusNode *&upNode) {
    if (node->keynum < M-1) {
        int j = node->keynum;
        while (j >= 1 && key < node->key[j]) {
            node->key[j+1] = node->key[j];
            node->child[j+1] = node->child[j];
            j--;
        }
        node->key[j+1] = key;
        node->child[j+1] = right;
        if (right) right->parent = node;
        node->keynum++;
        return 1;
    }
    
    KeyType tempKey[M+2];
    BPlusNode *tempChild[M+3];
    
    for (int i = 1; i <= M-1; i++) {
        tempKey[i] = node->key[i];
        tempChild[i] = node->child[i];
    }
    tempChild[0] = node->child[0];
    
    int j = M-1;
    while (j >= 1 && key < tempKey[j]) {
        tempKey[j+1] = tempKey[j];
        tempChild[j+1] = tempChild[j];
        j--;
    }
    tempKey[j+1] = key;
    tempChild[j+1] = right;
    if (right) right->parent = node;
    
    int mid = (M+1) / 2;
    BPlusNode *newNode = CreateBPlusNode(0);
    newNode->parent = node->parent;
    
    node->keynum = mid-1;
    for (int i = 0; i <= mid-1; i++) {
        node->child[i] = tempChild[i];
        if (i < mid-1) node->key[i+1] = tempKey[i+1];
    }
    
    int idx = 0;
    for (int i = mid; i <= M; i++) {
        newNode->child[idx] = tempChild[i];
        if (i <= M-1) newNode->key[++newNode->keynum] = tempKey[i];
        idx++;
    }
    
    for (int i = 0; i <= newNode->keynum; i++) {
        if (newNode->child[i]) newNode->child[i]->parent = newNode;
    }
    
    upKey = tempKey[mid];
    upNode = newNode;
    return 0;
}

/**
 * @brief B+树插入核心递归函数
 * 
 * @param T      B+树的根节点指针（引用），用于根节点分裂时更新
 * @param node   当前节点指针
 * @param key    要插入的关键字
 * @param data   要插入的数据
 * @param upKey  输出参数，返回需要上升到父节点的关键字
 * @param upNode 输出参数，返回分裂后新创建的节点指针
 * @return int 返回1表示插入成功且节点未满，返回0表示节点已满需要分裂
 * 
 * @note B+树数据只存在叶子节点
 *       内部节点分裂只上升key，不上升data
 */
int InsertBPlusTreeCore(BPlusTree &T, BPlusNode *node, KeyType key, DataType data,
                         KeyType &upKey, BPlusNode *&upNode) {
    if (node->isLeaf) {
        if (node->keynum < M-1) {
            int i = node->keynum;
            while (i >= 1 && key < node->key[i]) {
                node->key[i+1] = node->key[i];
                node->data[i+1] = node->data[i];
                i--;
            }
            node->key[i+1] = key;
            node->data[i+1] = data;
            node->keynum++;
            return 1;
        }
        
        KeyType tempKey[M+2];
        DataType tempData[M+2];
        
        for (int i = 1; i <= M-1; i++) {
            tempKey[i] = node->key[i];
            tempData[i] = node->data[i];
        }
        
        int i = M-1;
        while (i >= 1 && key < tempKey[i]) {
            tempKey[i+1] = tempKey[i];
            tempData[i+1] = tempData[i];
            i--;
        }
        tempKey[i+1] = key;
        tempData[i+1] = data;
        
        int mid = (M+1) / 2;
        BPlusNode *newLeaf = CreateBPlusNode(1);
        newLeaf->parent = node->parent;
        
        node->keynum = mid-1;
        for (int j = 1; j <= mid-1; j++) {
            node->key[j] = tempKey[j];
            node->data[j] = tempData[j];
        }
        
        for (int j = mid; j <= M; j++) {
            newLeaf->key[++newLeaf->keynum] = tempKey[j];
            newLeaf->data[newLeaf->keynum] = tempData[j];
        }
        
        newLeaf->next = node->next;
        node->next = newLeaf;
        
        upKey = newLeaf->key[1];
        upNode = newLeaf;
        return 0;
    }
    
    int i = 1;
    while (i <= node->keynum && key >= node->key[i]) i++;
    
    if (!InsertBPlusTreeCore(T, node->child[i-1], key, data, upKey, upNode)) {
        return InsertBPlusInternalCore(node, upKey, upNode, upKey, upNode);
    }
    return 1;
}

/**
 * @brief B+树插入入口
 * 
 * @param T    B+树的根节点指针（引用），插入后根节点可能改变
 * @param key  要插入的关键字
 * @param data 要插入的数据
 * @return Status 插入成功返回 OK
 */
Status InsertBPlusTree(BPlusTree &T, KeyType key, DataType data) {
    if (!T) {
        T = CreateBPlusNode(1);
        T->key[++T->keynum] = key;
        T->data[T->keynum] = data;
        return OK;
    }
    
    KeyType upKey;
    BPlusNode *upNode = NULL;
    if (!InsertBPlusTreeCore(T, T, key, data, upKey, upNode)) {
        BPlusNode *newRoot = CreateBPlusNode(0);
        newRoot->key[1] = upKey;
        newRoot->keynum = 1;
        newRoot->child[0] = T;
        newRoot->child[1] = upNode;
        T->parent = newRoot;
        upNode->parent = newRoot;
        T = newRoot;
    }
    return OK;
}