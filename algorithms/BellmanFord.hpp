/*
 *（距离数组，边集）。通过反复松弛所有边，逐步更新源点到各顶点的最短距离。
 *
 * Bellman-Ford算法思路：
 * 1. 初始化：源点到自己的距离为0，到其他顶点为无穷大
 * 2. 松弛操作：对每条边进行 V-1 次松弛
 * 3. 每次松弛：如果 dist[u] + w < dist[v]，则更新 dist[v]
 * 4. 检测负权环：第 V 次松弛如果还能更新，则存在负权环
 *
 * 排序算法	平均时间复杂度	最差时间复杂度	空间复杂度	数据对象稳定性
 * Bellman-Ford	O(VE)	         O(VE)	        O(V)	    适用于负权边
 * 图示例：5个顶点，边集如下
 * 边：0→1(6), 0→3(7), 1→2(5), 1→3(8), 1→4(-4), 2→1(-2), 3→2(-3), 3→4(9), 4→0(2), 4→2(7)
 * 
 * 初始状态（源点=0）：
 *   dist[0]=0, dist[1]=∞, dist[2]=∞, dist[3]=∞, dist[4]=∞
 * 
 * 第1轮松弛（所有边）：
 *   0→1(6)：dist[0]+6=6 < ∞ → dist[1]=6
 *   0→3(7)：dist[0]+7=7 < ∞ → dist[3]=7
 *   1→2(5)：dist[1]+5=11 < ∞ → dist[2]=11
 *   1→3(8)：6+8=14 ≥ 7 → 不变
 *   1→4(-4)：6+(-4)=2 < ∞ → dist[4]=2
 *   2→1(-2)：11+(-2)=9 ≥ 6 → 不变
 *   3→2(-3)：7+(-3)=4 < 11 → dist[2]=4
 *   3→4(9)：7+9=16 ≥ 2 → 不变
 *   4→0(2)：2+2=4 ≥ 0 → 不变
 *   4→2(7)：2+7=9 ≥ 4 → 不变
 *   第1轮结果：dist[1]=6, dist[2]=4, dist[3]=7, dist[4]=2
 * 
 * 第2轮松弛（所有边）：
 *   0→1(6)：0+6=6 ≥ 6 → 不变
 *   0→3(7)：0+7=7 ≥ 7 → 不变
 *   1→2(5)：6+5=11 ≥ 4 → 不变
 *   1→3(8)：6+8=14 ≥ 7 → 不变
 *   1→4(-4)：6+(-4)=2 ≥ 2 → 不变
 *   2→1(-2)：4+(-2)=2 < 6 → dist[1]=2  ✓ 更新
 *   3→2(-3)：7+(-3)=4 ≥ 4 → 不变
 *   3→4(9)：7+9=16 ≥ 2 → 不变
 *   4→0(2)：2+2=4 ≥ 0 → 不变
 *   4→2(7)：2+7=9 ≥ 4 → 不变
 *   第2轮结果：dist[1]=2, dist[2]=4, dist[3]=7, dist[4]=2
 * 
 * 第3轮松弛（所有边）：
 *   2→1(-2)：4+(-2)=2 ≥ 2 → 不变
 *   其他边均无法更新
 *   第3轮结果：无变化，提前退出
 * 
 * 最终结果：dist[0]=0, dist[1]=2, dist[2]=4, dist[3]=7, dist[4]=2
 * 路径：0→3→2→1→4  (0→3:7, 3→2:-3, 2→1:-2, 1→4:-4, 总长=7-3-2-4=-2)
 */

/**
 * @brief Bellman-Ford算法主函数
 * 
 * @param edges 边集（引用传递）
 * @param n     顶点数量
 * @param src   源点
 * @param dist  最短距离数组（输出）
 * @return true  成功找到最短路径
 * @return false 存在负权环
 * 
 * @note 时间复杂度 O(VE)，空间复杂度 O(V)
 * @note 可以处理负权边
 * @note 可以检测负权环
 * @note 如果存在负权环，返回false
 * 
 */
bool BellmanFord(std::vector<Edge>& edges, int n, int src, std::vector<int>& dist) {
    // 1. 初始化距离数组
    dist.resize(n, INF);
    dist[src] = 0;
    std::vector<int> parent(n, -1);  // 记录前驱节点，用于路径回溯
    
    // 2. 松弛 V-1 次
    for (int i = 1; i <= n - 1; i++) {
        bool updated = false;
        
        // 遍历所有边
        for (Edge& e : edges) {
            // 如果起点可达，且通过该边能缩短距离
            if (dist[e.u] != INF && dist[e.u] + e.weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.weight;
                parent[e.v] = e.u;
                updated = true;
            }
        }
        
        // 如果本轮没有更新，说明已经收敛，提前退出
        if (!updated) {
            break;
        }
    }
    
    // 3. 检查负权环
    // 如果第 V 次还能松弛，说明存在负权环
    for (Edge& e : edges) {
        if (dist[e.u] != INF && dist[e.u] + e.weight < dist[e.v]) {
            return false;  // 存在负权环
        }
    }
    
    return true;
}

/**
 * @brief 打印从源点到目标点的最短路径
 * 
 * @param parent 前驱节点数组
 * @param src    源点
 * @param dest   目标点
 * 
 * @note 使用parent数组回溯路径
 * @note 从dest开始，一直追溯到src
 */
void PrintPath(const std::vector<int>& parent, int src, int dest) {
    std::vector<int> path;
    int v = dest;
    
    // 从目标点回溯到源点
    while (v != -1) {
        path.push_back(v);
        v = parent[v];
    }
    
    // 反向输出（从源点到目标点）
    for (int i = path.size() - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0) printf("->");
    }
    printf("\n");
}

/**
 * @brief 打印所有顶点的最短距离
 * 
 * @param dist 最短距离数组
 * @param n    顶点数量
 * @param src  源点
 */
void PrintDistances(const std::vector<int>& dist, int n, int src) {
    printf("\n从顶点%d到各顶点的最短距离：\n", src);
    for (int i = 0; i < n; i++) {
        printf("到%d：", i);
        if (dist[i] == INF) {
            printf("不可达\n");
        } else {
            printf("%d\n", dist[i]);
        }
    }
}