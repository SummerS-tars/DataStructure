#include <cstdio>
#include <iostream>
#include <string>
#include <stack>
using namespace std;

/*
 * 实现思路：
 * 使用栈存储字符和对应的计数，栈中每个元素是 pair<char, int>
 * 1. 遍历字符串的每个字符
 * 2. 如果栈为空或栈顶字符与当前字符不同，将字符和计数1入栈
 * 3. 如果栈顶字符与当前字符相同，将栈顶计数+1
 * 4. 如果计数达到k，弹出栈顶元素（相当于删除k个相邻相同字符）
 * 5. 最后将栈中剩余字符按照计数重建字符串
 * 
 * 时间复杂度：O(n)，其中 n 是字符串长度，每个字符最多入栈出栈一次
 * 空间复杂度：O(n)，栈的空间最坏情况下存储所有字符
 */
string removeDuplicates(string s, int k)
{
    // 使用栈存储字符和对应的连续出现次数
    stack<pair<char, int>> st;
    
    for (char c : s)
    {
        // 如果栈不为空且栈顶字符与当前字符相同
        if (!st.empty() && st.top().first == c)
        {
            // 将栈顶的计数+1
            st.top().second++;
            
            // 如果计数达到k，则删除（弹出栈顶）
            if (st.top().second == k)
            {
                st.pop();
            }
        }
        else
        {
            // 栈为空或字符不同，将当前字符和计数1入栈
            st.push({c, 1});
        }
    }
    
    // 从栈中重建结果字符串
    string result = "";
    while (!st.empty())
    {
        auto p = st.top();
        st.pop();
        // 将字符重复 count 次添加到结果前面
        for (int i = 0; i < p.second; i++)
        {
            result = p.first + result;
        }
    }
    
    return result;
}

int main()
{
    // 测试用例1：基本情况
    string s1 = "deeedbbcccbdaa";
    int k1 = 3;
    cout << "测试用例1: s = \"" << s1 << "\", k = " << k1 << endl;
    cout << "结果: \"" << removeDuplicates(s1, k1) << "\"" << endl;
    cout << "预期: \"aa\" (删除eee->删除ccc->删除bbb->删除ddd)" << endl << endl;
    
    // 测试用例2：需要多次删除
    string s2 = "abcd";
    int k2 = 2;
    cout << "测试用例2: s = \"" << s2 << "\", k = " << k2 << endl;
    cout << "结果: \"" << removeDuplicates(s2, k2) << "\"" << endl;
    cout << "预期: \"abcd\" (没有连续重复)" << endl << endl;
    
    // 测试用例3：连锁删除
    string s3 = "pbbcggttciiippooaais";
    int k3 = 2;
    cout << "测试用例3: s = \"" << s3 << "\", k = " << k3 << endl;
    cout << "结果: \"" << removeDuplicates(s3, k3) << "\"" << endl;
    cout << "预期: \"ps\" (删除bb->删除gg->删除tt->删除cc->删除ii->删除pp->删除oo->删除aa->删除ii)" << endl << endl;
    
    return 0;
}
