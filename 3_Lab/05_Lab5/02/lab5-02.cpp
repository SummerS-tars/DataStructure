#include <cstdio>
#include <iostream>
#include <stack>
using namespace std;

/*
 * 实现思路：
 * 使用两个栈来模拟队列的先进先出特性
 * s1: 输入栈，用于接收push操作的元素
 * s2: 输出栈，用于处理pop和peek操作
 * 
 * 关键思想：利用两次栈反转实现FIFO
 * - 第一次反转：元素从s1弹出时顺序反转
 * - 第二次反转：元素压入s2后，s2的栈顶就是最早进入的元素
 * 
 * push操作：直接将元素压入s1
 * pop/peek操作：
 *   1. 如果s2不为空，直接从s2操作（s2栈顶是队首元素）
 *   2. 如果s2为空，将s1所有元素转移到s2（此时顺序反转），再从s2操作
 * 
 * 时间复杂度：
 *   - push: O(1)
 *   - pop/peek: 均摊O(1)，最坏O(n)
 *     每个元素最多被转移一次，所以均摊是O(1)
 * 空间复杂度：O(n)，其中n是队列中的元素个数
 */
class MyQueue
{
private:
    stack<int> s1; // 输入栈：接收新元素
    stack<int> s2; // 输出栈：提供队首元素

public:
    MyQueue()
    {
    }

    // 将元素推到队列末尾
    void push(int x)
    {
        s1.push(x);
    }

    // 从队列开头移除并返回元素
    int pop()
    {
        // 确保s2中有元素（即队首元素在s2栈顶）
        if (s2.empty())
        {
            // 将s1所有元素转移到s2
            while (!s1.empty())
            {
                s2.push(s1.top());
                s1.pop();
            }
        }
        
        // 从s2弹出栈顶元素（队首）
        int front = s2.top();
        s2.pop();
        return front;
    }

    // 返回队列开头的元素
    int peek()
    {
        // 确保s2中有元素
        if (s2.empty())
        {
            // 将s1所有元素转移到s2
            while (!s1.empty())
            {
                s2.push(s1.top());
                s1.pop();
            }
        }
        
        // 返回s2栈顶元素（队首），但不删除
        return s2.top();
    }

    // 判断队列是否为空
    bool empty()
    {
        // 两个栈都为空时，队列才为空
        return s1.empty() && s2.empty();
    }
};

int main()
{
    cout << "=== 测试用例1：基本操作 ===" << endl;
    MyQueue q1;
    q1.push(1);
    q1.push(2);
    cout << "push(1), push(2)" << endl;
    cout << "peek(): " << q1.peek() << " (预期: 1)" << endl;
    cout << "pop(): " << q1.pop() << " (预期: 1)" << endl;
    q1.push(3);
    cout << "push(3)" << endl;
    cout << "pop(): " << q1.pop() << " (预期: 2)" << endl;
    cout << "pop(): " << q1.pop() << " (预期: 3)" << endl;
    cout << "empty(): " << q1.empty() << " (预期: 1/true)" << endl;
    cout << endl;

    cout << "=== 测试用例2：交替操作 ===" << endl;
    MyQueue q2;
    q2.push(10);
    q2.push(20);
    q2.push(30);
    cout << "push(10), push(20), push(30)" << endl;
    cout << "peek(): " << q2.peek() << " (预期: 10)" << endl;
    cout << "pop(): " << q2.pop() << " (预期: 10)" << endl;
    q2.push(40);
    q2.push(50);
    cout << "push(40), push(50)" << endl;
    cout << "pop(): " << q2.pop() << " (预期: 20)" << endl;
    cout << "peek(): " << q2.peek() << " (预期: 30)" << endl;
    cout << "pop(): " << q2.pop() << " (预期: 30)" << endl;
    cout << "empty(): " << q2.empty() << " (预期: 0/false)" << endl;
    cout << "pop(): " << q2.pop() << " (预期: 40)" << endl;
    cout << "pop(): " << q2.pop() << " (预期: 50)" << endl;
    cout << "empty(): " << q2.empty() << " (预期: 1/true)" << endl;
    cout << endl;

    cout << "=== 测试用例3：大量元素 ===" << endl;
    MyQueue q3;
    for (int i = 1; i <= 5; i++)
    {
        q3.push(i * 100);
    }
    cout << "push(100), push(200), push(300), push(400), push(500)" << endl;
    cout << "连续pop: ";
    while (!q3.empty())
    {
        cout << q3.pop() << " ";
    }
    cout << "(预期: 100 200 300 400 500)" << endl;
    cout << "empty(): " << q3.empty() << " (预期: 1/true)" << endl;

    return 0;
}