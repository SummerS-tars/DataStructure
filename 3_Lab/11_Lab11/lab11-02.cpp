#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief DFS遍历连通分量，收集该分量中的所有节点
 * 
 * @param node 当前访问的节点
 * @param graph 邻接表表示的图
 * @param visited 访问标记数组
 * @param component 当前连通分量中的节点集合
 */
void dfs(int node, vector<vector<int>> &graph, vector<bool> &visited, vector<int> &component)
{
    // 标记当前节点已访问
    visited[node] = true;
    // 将当前节点加入连通分量
    component.push_back(node);
    
    // 遍历当前节点的所有邻居
    for (int neighbor : graph[node])
    {
        // 如果邻居未访问，递归访问
        if (!visited[neighbor])
        {
            dfs(neighbor, graph, visited, component);
        }
    }
}

/**
 * @brief 统计图中完全连通分量的数量
 * 
 * 算法思路：
 * 1. 首先使用邻接表构建无向图的表示
 * 2. 使用DFS遍历图，找出所有的连通分量
 * 3. 对于每个连通分量，检查它是否是完全图：
 *    - 完全图的定义：任意两个节点之间都有边
 *    - 对于有m个节点的完全图，边数应该是 m*(m-1)/2
 *    - 由于是无向图，每条边在邻接表中会被存储两次，所以总度数是 m*(m-1)
 * 4. 统计满足完全图条件的连通分量数量
 * 
 * 时间复杂度：O(n + e)，其中n是节点数，e是边数
 * 空间复杂度：O(n + e)，用于存储图和访问标记
 * 
 * @param n 节点数量
 * @param edges 边的列表
 * @return 完全连通分量的数量
 */
int countCompleteComponents(int n, vector<vector<int>> &edges)
{
    // 步骤1：构建邻接表表示的无向图
    vector<vector<int>> graph(n);
    for (const auto &edge : edges)
    {
        int u = edge[0];
        int v = edge[1];
        // 无向图：添加双向边
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    
    // 步骤2：使用DFS找出所有连通分量
    vector<bool> visited(n, false);
    int completeCount = 0;
    
    for (int i = 0; i < n; i++)
    {
        // 如果节点未访问，说明发现了新的连通分量
        if (!visited[i])
        {
            vector<int> component;  // 存储当前连通分量的所有节点
            dfs(i, graph, visited, component);
            
            // 步骤3：检查该连通分量是否为完全图
            int m = component.size();  // 连通分量中的节点数
            
            // 计算该连通分量中所有节点的度数之和
            int totalDegree = 0;
            for (int node : component)
            {
                totalDegree += graph[node].size();
            }
            
            // 完全图判定：
            // - 在完全图中，每个节点都与其他所有节点相连
            // - m个节点的完全图，每个节点的度数是 (m-1)
            // - 所有节点度数之和 = m * (m-1)
            if (totalDegree == m * (m - 1))
            {
                completeCount++;
            }
        }
    }
    
    return completeCount;
}

int main()
{
    cout << "===== 测试用例 1 (题目示例1) =====" << endl;
    cout << "n=6, 边: (0,1), (0,2), (1,2), (3,4)" << endl;
    cout << "说明: " << endl;
    cout << "  - 分量1: {0,1,2} 是完全图 (3个节点，3条边)" << endl;
    cout << "  - 分量2: {3,4} 是完全图 (2个节点，1条边)" << endl;
    cout << "  - 分量3: {5} 是完全图 (1个节点，0条边)" << endl;
    vector<vector<int>> edges1 = {{0, 1}, {0, 2}, {1, 2}, {3, 4}};
    int result1 = countCompleteComponents(6, edges1);
    cout << "输出结果: " << result1 << endl;
    cout << "期望结果: 3" << endl << endl;

    cout << "===== 测试用例 2 (题目示例2) =====" << endl;
    cout << "n=6, 边: (0,1), (0,2), (1,2), (3,4), (3,5)" << endl;
    cout << "说明: " << endl;
    cout << "  - 分量1: {0,1,2} 是完全图 (3个节点，3条边)" << endl;
    cout << "  - 分量2: {3,4,5} 不是完全图 (3个节点但只有2条边，缺少4-5边)" << endl;
    vector<vector<int>> edges2 = {{0, 1}, {0, 2}, {1, 2}, {3, 4}, {3, 5}};
    int result2 = countCompleteComponents(6, edges2);
    cout << "输出结果: " << result2 << endl;
    cout << "期望结果: 1" << endl << endl;

    cout << "===== 测试用例 3 (所有节点孤立) =====" << endl;
    cout << "n=4, 无边" << endl;
    cout << "说明: 4个孤立节点，每个都是完全图（大小为1的完全图）" << endl;
    vector<vector<int>> edges3 = {};
    int result3 = countCompleteComponents(4, edges3);
    cout << "输出结果: " << result3 << endl;
    cout << "期望结果: 4" << endl << endl;

    cout << "===== 测试用例 4 (一个大完全图) =====" << endl;
    cout << "n=4, 边: (0,1), (0,2), (0,3), (1,2), (1,3), (2,3)" << endl;
    cout << "说明: K4完全图，4个节点，6条边" << endl;
    vector<vector<int>> edges4 = {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};
    int result4 = countCompleteComponents(4, edges4);
    cout << "输出结果: " << result4 << endl;
    cout << "期望结果: 1" << endl << endl;

    cout << "===== 测试用例 5 (混合情况) =====" << endl;
    cout << "n=8, 边: (0,1), (2,3), (2,4), (3,4), (5,6), (5,7)" << endl;
    cout << "说明: " << endl;
    cout << "  - 分量1: {0,1} 是完全图" << endl;
    cout << "  - 分量2: {2,3,4} 是完全图 (K3)" << endl;
    cout << "  - 分量3: {5,6,7} 不是完全图 (缺少6-7边)" << endl;
    vector<vector<int>> edges5 = {{0, 1}, {2, 3}, {2, 4}, {3, 4}, {5, 6}, {5, 7}};
    int result5 = countCompleteComponents(8, edges5);
    cout << "输出结果: " << result5 << endl;
    cout << "期望结果: 2" << endl << endl;

    cout << "===== 测试用例 6 (线性链不是完全图) =====" << endl;
    cout << "n=5, 边: (0,1), (1,2), (2,3), (3,4)" << endl;
    cout << "说明: 5个节点形成一条链，不是完全图（完全图需要10条边）" << endl;
    vector<vector<int>> edges6 = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    int result6 = countCompleteComponents(5, edges6);
    cout << "输出结果: " << result6 << endl;
    cout << "期望结果: 0" << endl << endl;

    return 0;
}