#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief 找到可以到达所有点的最小点集
 * 
 * 算法思路：
 * 1. 在有向无环图中，我们需要找到一个最小的起点集合，使得从这些起点可以到达所有节点
 * 2. 关键观察：如果一个节点有入边（入度>0），说明它可以从其他节点到达，因此不需要将它作为起点
 * 3. 只有入度为0的节点才必须作为起点，因为没有其他节点能到达它们
 * 4. 由于图是有向无环图（DAG），从所有入度为0的节点出发，一定能到达所有其他节点
 * 
 * 时间复杂度：O(n + m)，其中n是节点数，m是边数
 * 空间复杂度：O(n)，用于存储入度数组
 * 
 * @param n 节点数量（0到n-1）
 * @param edges 边的列表，edges[i] = [ai, bi] 表示从ai到bi的有向边
 * @return 最小起点集合
 */
vector<int> findSmallestSetOfVertices(int n, vector<vector<int>> &edges)
{
    // 步骤1：创建入度数组，记录每个节点的入度（有多少条边指向它）
    vector<int> inDegree(n, 0);
    
    // 步骤2：遍历所有边，统计每个节点的入度
    // 对于边 [from, to]，to节点的入度加1
    for (const auto &edge : edges)
    {
        // edge[0] 是起点，edge[1] 是终点
        int to = edge[1];    // 终点
        inDegree[to]++;      // 终点的入度加1
    }
    
    // 步骤3：收集所有入度为0的节点
    // 这些节点无法从其他节点到达，必须作为起点
    vector<int> result;
    for (int i = 0; i < n; i++)
    {
        if (inDegree[i] == 0)
        {
            result.push_back(i);
        }
    }
    
    return result;
}

int main()
{
    cout << "===== 测试用例 1 (题目示例1) =====" << endl;
    cout << "图结构: 0->1, 0->2, 2->5, 3->4, 4->2" << endl;
    cout << "说明: 节点0和3没有入边，必须作为起点" << endl;
    vector<vector<int>> edges1 = {{0, 1}, {0, 2}, {2, 5}, {3, 4}, {4, 2}};
    vector<int> ans1 = findSmallestSetOfVertices(6, edges1);
    cout << "输出结果: ";
    for (const auto &x : ans1)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << "期望结果: 0 3" << endl << endl;

    cout << "===== 测试用例 2 (题目示例2) =====" << endl;
    cout << "图结构: 0->1, 2->1, 3->1, 1->4, 2->4" << endl;
    cout << "说明: 节点0、2、3没有入边，必须作为起点" << endl;
    vector<vector<int>> edges2 = {{0, 1}, {2, 1}, {3, 1}, {1, 4}, {2, 4}};
    vector<int> ans2 = findSmallestSetOfVertices(5, edges2);
    cout << "输出结果: ";
    for (const auto &x : ans2)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << "期望结果: 0 2 3" << endl << endl;

    cout << "===== 测试用例 3 (线性链) =====" << endl;
    cout << "图结构: 0->1->2->3->4" << endl;
    cout << "说明: 只有节点0没有入边，它是唯一的起点" << endl;
    vector<vector<int>> edges3 = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    vector<int> ans3 = findSmallestSetOfVertices(5, edges3);
    cout << "输出结果: ";
    for (const auto &x : ans3)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << "期望结果: 0" << endl << endl;

    cout << "===== 测试用例 4 (多个独立子图) =====" << endl;
    cout << "图结构: 0->1, 2->3, 4->5->6" << endl;
    cout << "说明: 三个独立的子图，每个子图的源节点都必须作为起点" << endl;
    vector<vector<int>> edges4 = {{0, 1}, {2, 3}, {4, 5}, {5, 6}};
    vector<int> ans4 = findSmallestSetOfVertices(7, edges4);
    cout << "输出结果: ";
    for (const auto &x : ans4)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << "期望结果: 0 2 4" << endl << endl;

    cout << "===== 测试用例 5 (复杂的汇聚结构) =====" << endl;
    cout << "图结构: 0->2, 1->2, 2->3, 4->3, 5->6, 6->3" << endl;
    cout << "说明: 多个源节点汇聚到公共节点" << endl;
    vector<vector<int>> edges5 = {{0, 2}, {1, 2}, {2, 3}, {4, 3}, {5, 6}, {6, 3}};
    vector<int> ans5 = findSmallestSetOfVertices(7, edges5);
    cout << "输出结果: ";
    for (const auto &x : ans5)
    {
        cout << x << " ";
    }
    cout << endl;
    cout << "期望结果: 0 1 4 5" << endl << endl;

    return 0;
}