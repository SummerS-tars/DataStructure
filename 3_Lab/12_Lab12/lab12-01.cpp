#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

/*
 * 算法思路：
 * 1. 使用 Dijkstra 算法从起点 0 和终点 n-1 分别计算到所有节点的最短距离
 * 2. 对于每条边 (u, v, w)，如果满足：
 *    dist_from_start[u] + w + dist_from_end[v] == shortest_path 或
 *    dist_from_start[v] + w + dist_from_end[u] == shortest_path
 *    则该边在某条最短路径上
 * 3. shortest_path = dist_from_start[n-1] 表示从 0 到 n-1 的最短路径长度
 *
 * 时间复杂度：O((n + m) * log(n))，其中 n 是节点数，m 是边数
 * 空间复杂度：O(n + m)
 */

// Dijkstra 算法：计算从 start 节点到所有其他节点的最短距离
vector<long long> dijkstra(int n, vector<vector<pair<int, int>>> &graph, int start)
{
    // dist[i] 表示从 start 到节点 i 的最短距离
    vector<long long> dist(n, LLONG_MAX);
    dist[start] = 0;

    // 优先队列：(距离, 节点编号)，按距离从小到大排序
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        // 如果当前距离大于已知最短距离，跳过
        if (d > dist[u])
            continue;

        // 遍历所有相邻节点
        for (auto [v, w] : graph[u])
        {
            // 如果通过 u 到 v 的距离更短，更新距离并加入队列
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

vector<bool> findAnswer(int n, vector<vector<int>> &edges)
{
    int m = edges.size();
    vector<bool> answer(m, false);

    // 步骤1：构建邻接表，存储无向图
    // graph[u] = {(v, w), ...} 表示节点 u 到节点 v 有一条权重为 w 的边
    vector<vector<pair<int, int>>> graph(n);
    for (int i = 0; i < m; i++)
    {
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // 无向图，双向添加
    }

    // 步骤2：使用 Dijkstra 算法计算从起点 0 到所有节点的最短距离
    vector<long long> dist_from_start = dijkstra(n, graph, 0);

    // 步骤3：使用 Dijkstra 算法计算从终点 n-1 到所有节点的最短距离
    vector<long long> dist_from_end = dijkstra(n, graph, n - 1);

    // 如果起点或终点不可达，返回全 false
    if (dist_from_start[n - 1] == LLONG_MAX)
    {
        return answer;
    }

    // 步骤4：获取从 0 到 n-1 的最短路径长度
    long long shortest_path = dist_from_start[n - 1];

    // 步骤5：检查每条边是否在最短路径上
    for (int i = 0; i < m; i++)
    {
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];

        // 检查边 (u, v) 是否在最短路径上
        // 情况1：起点 -> u -> v -> 终点
        bool case1 = (dist_from_start[u] != LLONG_MAX &&
                      dist_from_end[v] != LLONG_MAX &&
                      dist_from_start[u] + w + dist_from_end[v] == shortest_path);

        // 情况2：起点 -> v -> u -> 终点（因为是无向图）
        bool case2 = (dist_from_start[v] != LLONG_MAX &&
                      dist_from_end[u] != LLONG_MAX &&
                      dist_from_start[v] + w + dist_from_end[u] == shortest_path);

        answer[i] = case1 || case2;
    }

    return answer;
}

int main()
{
    // 测试用例1：题目示例1
    // 预期输出：1 1 1 0 1 1 1 0
    cout << "Test 1: ";
    int n = 6;
    vector<vector<int>> edges = {{0, 1, 4}, {0, 2, 1}, {1, 3, 2}, {1, 4, 3}, {1, 5, 1}, {2, 3, 1}, {3, 5, 3}, {4, 5, 2}};
    vector<bool> ans = findAnswer(n, edges);
    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << " ";
    }
    cout << endl;

    // 测试用例2：题目示例2
    // 预期输出：1 0 0 1
    cout << "Test 2: ";
    n = 4;
    edges = {{2, 0, 1}, {0, 1, 1}, {0, 3, 4}, {3, 2, 2}};
    ans = findAnswer(n, edges);
    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << " ";
    }
    cout << endl;

    // 测试用例3：题目示例3 - 无法从节点0到达节点n-1
    // 预期输出：0
    cout << "Test 3: ";
    n = 3;
    edges = {{2, 1, 6}};
    ans = findAnswer(n, edges);
    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << " ";
    }
    cout << endl;

    // 额外测试用例4：简单路径 - 只有一条直接路径
    // 图结构：0 -> 1 -> 2 (权重都为1)
    // 预期输出：1 1 (所有边都在最短路径上)
    cout << "Test 4: ";
    n = 3;
    edges = {{0, 1, 1}, {1, 2, 1}};
    ans = findAnswer(n, edges);
    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << " ";
    }
    cout << endl;

    // 额外测试用例5：多条等长最短路径
    // 图结构：菱形图，0->1->3 和 0->2->3 路径长度相同
    // 0 -> 1 (权重1), 0 -> 2 (权重1), 1 -> 3 (权重1), 2 -> 3 (权重1)
    // 预期输出：1 1 1 1 (所有边都在某条最短路径上)
    cout << "Test 5: ";
    n = 4;
    edges = {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3, 1}};
    ans = findAnswer(n, edges);
    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << " ";
    }
    cout << endl;

    // 额外测试用例6：有绕路的边
    // 图结构：0 -> 1 (权重1), 1 -> 2 (权重1), 0 -> 2 (权重5)
    // 最短路径是 0 -> 1 -> 2，权重为2
    // 预期输出：1 1 0 (第三条边不在最短路径上)
    cout << "Test 6: ";
    n = 3;
    edges = {{0, 1, 1}, {1, 2, 1}, {0, 2, 5}};
    ans = findAnswer(n, edges);
    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << " ";
    }
    cout << endl;

    // 额外测试用例7：复杂图 - 多个节点和多条路径
    // 测试更复杂的情况
    cout << "Test 7: ";
    n = 5;
    edges = {{0, 1, 2}, {0, 2, 4}, {1, 2, 1}, {1, 3, 7}, {2, 4, 3}, {3, 4, 2}};
    ans = findAnswer(n, edges);
    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << " ";
    }
    cout << endl;

    return 0;
}