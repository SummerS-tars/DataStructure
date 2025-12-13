#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/*
 * 算法思路：拓扑排序（Kahn's Algorithm）
 * 
 * 1. 构建有向图和入度数组：
 *    - 对于每个先修课程关系 [a, b]，表示 b -> a（必须先学 b 才能学 a）
 *    - 统计每门课程的入度（有多少门课程依赖于它）
 * 
 * 2. 使用 BFS 进行拓扑排序：
 *    - 将所有入度为 0 的课程加入队列（这些课程没有先修要求）
 *    - 每次从队列中取出一个课程，加入结果数组
 *    - 将该课程的所有后继课程的入度减 1
 *    - 如果后继课程的入度变为 0，加入队列
 * 
 * 3. 检测环：
 *    - 如果最终结果数组的长度等于课程总数，说明所有课程都可以完成（无环）
 *    - 如果小于课程总数，说明存在环（有些课程形成循环依赖），返回空数组
 * 
 * 时间复杂度：O(V + E)，其中 V 是课程数，E 是先修关系数
 * 空间复杂度：O(V + E)
 */

class Solution
{
public:
    vector<int> findOrder(int numCourses, vector<vector<int>> &prerequisites)
    {
        // 步骤1：初始化数据结构
        // graph[i] 存储课程 i 的所有后继课程（学完 i 后可以学哪些课程）
        vector<vector<int>> graph(numCourses);
        
        // inDegree[i] 存储课程 i 的入度（有多少门课程必须在 i 之前学完）
        vector<int> inDegree(numCourses, 0);

        // 步骤2：构建图和入度数组
        for (const auto &pre : prerequisites)
        {
            int course = pre[0];     // 要学的课程
            int prerequisite = pre[1]; // 先修课程
            
            // 添加边：prerequisite -> course（先学 prerequisite，才能学 course）
            graph[prerequisite].push_back(course);
            
            // course 的入度加 1（course 多了一个先修课程）
            inDegree[course]++;
        }

        // 步骤3：将所有入度为 0 的课程加入队列
        // 这些课程没有先修要求，可以直接学习
        queue<int> q;
        for (int i = 0; i < numCourses; i++)
        {
            if (inDegree[i] == 0)
            {
                q.push(i);
            }
        }

        // 步骤4：BFS 进行拓扑排序
        vector<int> result;
        
        while (!q.empty())
        {
            // 取出一个入度为 0 的课程
            int course = q.front();
            q.pop();
            
            // 将该课程加入结果数组（表示可以在这个时间点学习）
            result.push_back(course);

            // 遍历所有依赖于当前课程的后继课程
            for (int nextCourse : graph[course])
            {
                // 将后继课程的入度减 1（相当于完成了一门先修课程）
                inDegree[nextCourse]--;
                
                // 如果后继课程的入度变为 0，说明它的所有先修课程都已完成
                // 可以加入队列等待学习
                if (inDegree[nextCourse] == 0)
                {
                    q.push(nextCourse);
                }
            }
        }

        // 步骤5：检测是否存在环
        // 如果结果数组的长度等于课程总数，说明所有课程都可以完成（无环）
        // 否则说明存在环（有些课程形成循环依赖），返回空数组
        if (result.size() == numCourses)
        {
            return result;
        }
        else
        {
            return {}; // 存在环，返回空数组
        }
    }
};

int main()
{
    // 测试用例1：题目示例1 - 简单的线性依赖
    // 课程1依赖课程0，预期输出：0 1（或任何以0开头的序列）
    cout << "Test 1: ";
    Solution solution;
    int numCourses = 2;
    vector<vector<int>> prerequisites = {{1, 0}};
    vector<int> result = solution.findOrder(numCourses, prerequisites);
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;

    // 测试用例2：题目示例2 - 多个依赖关系
    // 课程1和2都依赖课程0，课程3依赖课程1和2
    // 预期输出：0 1 2 3 或 0 2 1 3（0必须在最前面，3必须在最后面）
    cout << "Test 2: ";
    Solution solution2;
    numCourses = 4;
    prerequisites = {{1, 0}, {2, 0}, {3, 1}, {3, 2}};
    result = solution2.findOrder(numCourses, prerequisites);
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;

    // 测试用例3：题目示例3 - 存在环（循环依赖）
    // 课程0依赖课程2，课程2依赖课程0，形成环
    // 预期输出：空数组
    cout << "Test 3: ";
    Solution solution3;
    numCourses = 3;
    prerequisites = {{1, 0}, {2, 0}, {0, 2}};
    result = solution3.findOrder(numCourses, prerequisites);
    if (result.empty())
    {
        cout << "(empty - cycle detected)";
    }
    else
    {
        for (int i = 0; i < result.size(); i++)
        {
            cout << result[i] << " ";
        }
    }
    cout << endl;

    // 额外测试用例4：没有任何依赖关系
    // 所有课程都可以任意顺序学习
    // 预期输出：0 1 2（或任何顺序）
    cout << "Test 4: ";
    Solution solution4;
    numCourses = 3;
    prerequisites = {};
    result = solution4.findOrder(numCourses, prerequisites);
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;

    // 额外测试用例5：单个课程
    // 只有一门课程，无依赖
    // 预期输出：0
    cout << "Test 5: ";
    Solution solution5;
    numCourses = 1;
    prerequisites = {};
    result = solution5.findOrder(numCourses, prerequisites);
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;

    // 额外测试用例6：链式依赖
    // 课程1依赖0，课程2依赖1，课程3依赖2
    // 预期输出：0 1 2 3
    cout << "Test 6: ";
    Solution solution6;
    numCourses = 4;
    prerequisites = {{1, 0}, {2, 1}, {3, 2}};
    result = solution6.findOrder(numCourses, prerequisites);
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;

    // 额外测试用例7：复杂的依赖关系（菱形结构）
    // 课程1和2依赖课程0，课程3依赖课程1和2，课程4依赖课程3
    // 预期输出：0开头，4结尾，中间1、2、3的顺序有多种可能
    cout << "Test 7: ";
    Solution solution7;
    numCourses = 5;
    prerequisites = {{1, 0}, {2, 0}, {3, 1}, {3, 2}, {4, 3}};
    result = solution7.findOrder(numCourses, prerequisites);
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << " ";
    }
    cout << endl;

    // 额外测试用例8：简单的环（2个节点）
    // 课程0依赖课程1，课程1依赖课程0
    // 预期输出：空数组
    cout << "Test 8: ";
    Solution solution8;
    numCourses = 2;
    prerequisites = {{0, 1}, {1, 0}};
    result = solution8.findOrder(numCourses, prerequisites);
    if (result.empty())
    {
        cout << "(empty - cycle detected)";
    }
    else
    {
        for (int i = 0; i < result.size(); i++)
        {
            cout << result[i] << " ";
        }
    }
    cout << endl;

    return 0;
}