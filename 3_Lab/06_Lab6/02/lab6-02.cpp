#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_map>

using namespace std;

int find(string start)
{
    // 定义目标状态
    string end = "12345678x";
    
    // 如果初始状态就是目标状态
    if (start == end) return 0;
    
    // BFS搜索
    queue<string> q;
    unordered_map<string, int> visited;
    
    q.push(start);
    visited[start] = 0;
    
    // 四个方向：上、下、左、右
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        
        // 找到 'x' 的位置
        int pos = current.find('x');
        int x = pos / 3;  // 行
        int y = pos % 3;  // 列
        
        // 尝试四个方向
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            // 检查新位置是否合法
            if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
                int newPos = nx * 3 + ny;
                string next = current;
                
                // 交换 'x' 和相邻的数字
                swap(next[pos], next[newPos]);
                
                // 如果这个状态已经访问过，跳过
                if (visited.find(next) != visited.end()) {
                    continue;
                }
                
                // 如果到达目标状态，返回步数
                if (next == end) {
                    return visited[current] + 1;
                }
                
                // 将新状态加入队列
                visited[next] = visited[current] + 1;
                q.push(next);
            }
        }
    }
    
    // 无法达到目标状态
    return -1;
}

int main()
{
    string c, start;

    for (int i = 0; i < 9; i++)
    {
        cin >> c;
        start += c;
    }

    cout << find(start) << endl;

    return 0;
}