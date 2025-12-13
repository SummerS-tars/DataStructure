#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief 并查集类，用于Kruskal算法中判断和合并连通分量
 */
class UnionFind
{
private:
    vector<int> parent;  // parent[i] 表示节点i的父节点
    vector<int> rank;    // rank[i] 表示以i为根的树的秩（高度）

public:
    /**
     * @brief 初始化并查集
     * @param n 节点数量
     */
    UnionFind(int n)
    {
        parent.resize(n);
        rank.resize(n, 0);
        // 初始时每个节点的父节点是自己
        for (int i = 0; i < n; i++)
        {
            parent[i] = i;
        }
    }

    /**
     * @brief 查找节点x所属的集合（查找根节点）
     * 使用路径压缩优化：将路径上的所有节点直接连接到根节点
     * @param x 要查找的节点
     * @return 节点x所属集合的根节点
     */
    int find(int x)
    {
        if (parent[x] != x)
        {
            // 路径压缩：递归查找根节点，并将x直接连接到根节点
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    /**
     * @brief 合并节点x和y所在的集合
     * 使用按秩合并优化：将秩小的树连接到秩大的树上
     * @param x 节点x
     * @param y 节点y
     * @return 如果x和y原本不在同一集合返回true，否则返回false
     */
    bool unite(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);

        // 如果已经在同一集合，无需合并
        if (rootX == rootY)
        {
            return false;
        }

        // 按秩合并：将秩小的树连接到秩大的树上
        if (rank[rootX] < rank[rootY])
        {
            parent[rootX] = rootY;
        }
        else if (rank[rootX] > rank[rootY])
        {
            parent[rootY] = rootX;
        }
        else
        {
            // 秩相同时，任意选择一个作为根，并增加其秩
            parent[rootY] = rootX;
            rank[rootX]++;
        }

        return true;
    }
};

/**
 * @brief 计算连接所有点的最小费用（最小生成树）
 * 
 * 算法思路（Kruskal算法）：
 * 1. 将所有可能的边（点对）及其权重（曼哈顿距离）存储起来
 * 2. 将所有边按权重从小到大排序
 * 3. 使用并查集，按顺序考虑每条边：
 *    - 如果边的两个端点不在同一连通分量中，则选择这条边
 *    - 将两个端点所在的连通分量合并
 * 4. 重复步骤3，直到选择了n-1条边（n个点的树有n-1条边）
 * 5. 返回所有选中边的权重之和
 * 
 * 为什么使用Kruskal算法：
 * - 贪心策略：每次选择权重最小的边，保证局部最优
 * - 并查集保证不形成环：只选择连接不同连通分量的边
 * - 最终得到的是最小生成树（MST），总权重最小
 * 
 * 时间复杂度：O(n^2 * log(n))，其中n是点的数量
 *   - 生成所有边：O(n^2)
 *   - 排序：O(n^2 * log(n^2)) = O(n^2 * log(n))
 *   - Kruskal算法：O(n^2 * α(n))，α(n)是反阿克曼函数，几乎为常数
 * 空间复杂度：O(n^2)，存储所有边
 * 
 * @param points 二维平面上的点坐标
 * @return 连接所有点的最小总费用
 */
int minCostConnectPoints(vector<vector<int>> &points)
{
    int n = points.size();
    
    // 步骤1：生成所有可能的边，并计算每条边的权重（曼哈顿距离）
    // 边的表示：{权重, {点1索引, 点2索引}}
    vector<pair<int, pair<int, int>>> edges;
    
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            // 计算点i和点j之间的曼哈顿距离
            int distance = abs(points[i][0] - points[j][0]) + 
                          abs(points[i][1] - points[j][1]);
            edges.push_back({distance, {i, j}});
        }
    }
    
    // 步骤2：将所有边按权重从小到大排序
    sort(edges.begin(), edges.end());
    
    // 步骤3：使用Kruskal算法和并查集构建最小生成树
    UnionFind uf(n);
    int totalCost = 0;      // 最小生成树的总权重
    int edgesUsed = 0;      // 已使用的边数
    
    // 遍历所有边（从权重最小的开始）
    for (const auto &edge : edges)
    {
        int cost = edge.first;           // 边的权重
        int point1 = edge.second.first;  // 边的第一个端点
        int point2 = edge.second.second; // 边的第二个端点
        
        // 如果两个端点不在同一连通分量中，选择这条边
        if (uf.unite(point1, point2))
        {
            totalCost += cost;
            edgesUsed++;
            
            // 最小生成树有n-1条边，找到后可以提前结束
            if (edgesUsed == n - 1)
            {
                break;
            }
        }
    }
    
    return totalCost;
}

int main()
{
    cout << "===== 测试用例 1 (题目示例1) =====" << endl;
    cout << "点: [[0,0],[2,2],[3,10],[5,2],[7,0]]" << endl;
    cout << "说明: 5个点的最小生成树" << endl;
    cout << "连接方式示例: 0-4(7), 0-1(4), 1-3(5), 3-2(8+1=9)" << endl;
    vector<vector<int>> points1 = {{0, 0}, {2, 2}, {3, 10}, {5, 2}, {7, 0}};
    int result1 = minCostConnectPoints(points1);
    cout << "输出结果: " << result1 << endl;
    cout << "期望结果: 20" << endl << endl;

    cout << "===== 测试用例 2 (题目示例2) =====" << endl;
    cout << "点: [[3,12],[-2,5],[-4,1]]" << endl;
    cout << "说明: 3个点的最小生成树" << endl;
    cout << "距离: (3,12)到(-2,5) = 5+7=12" << endl;
    cout << "     (3,12)到(-4,1) = 7+11=18（不选）" << endl;
    cout << "     (-2,5)到(-4,1) = 2+4=6" << endl;
    cout << "总和: 12+6=18" << endl;
    vector<vector<int>> points2 = {{3, 12}, {-2, 5}, {-4, 1}};
    int result2 = minCostConnectPoints(points2);
    cout << "输出结果: " << result2 << endl;
    cout << "期望结果: 18" << endl << endl;

    cout << "===== 测试用例 3 (题目示例3) =====" << endl;
    cout << "点: [[0,0],[1,1],[1,0],[-1,1]]" << endl;
    cout << "说明: 正方形的4个角点" << endl;
    vector<vector<int>> points3 = {{0, 0}, {1, 1}, {1, 0}, {-1, 1}};
    int result3 = minCostConnectPoints(points3);
    cout << "输出结果: " << result3 << endl;
    cout << "期望结果: 4" << endl << endl;

    cout << "===== 测试用例 4 (极端情况：两个远距离点) =====" << endl;
    cout << "点: [[-1000000,-1000000],[1000000,1000000]]" << endl;
    cout << "说明: 两个点的曼哈顿距离 = 2000000 + 2000000 = 4000000" << endl;
    vector<vector<int>> points4 = {{-1000000, -1000000}, {1000000, 1000000}};
    int result4 = minCostConnectPoints(points4);
    cout << "输出结果: " << result4 << endl;
    cout << "期望结果: 4000000" << endl << endl;

    cout << "===== 测试用例 5 (单点) =====" << endl;
    cout << "点: [[0,0]]" << endl;
    cout << "说明: 单个点不需要连接，费用为0" << endl;
    vector<vector<int>> points5 = {{0, 0}};
    int result5 = minCostConnectPoints(points5);
    cout << "输出结果: " << result5 << endl;
    cout << "期望结果: 0" << endl << endl;

    cout << "===== 测试用例 6 (共线点) =====" << endl;
    cout << "点: [[0,0],[1,0],[2,0],[3,0]]" << endl;
    cout << "说明: 4个点在同一水平线上，依次连接最优" << endl;
    cout << "连接: 0-1(1), 1-2(1), 2-3(1)，总和=3" << endl;
    vector<vector<int>> points6 = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
    int result6 = minCostConnectPoints(points6);
    cout << "输出结果: " << result6 << endl;
    cout << "期望结果: 3" << endl << endl;

    return 0;
}