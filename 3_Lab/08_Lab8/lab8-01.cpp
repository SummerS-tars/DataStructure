#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

// Definition for a binary tree node.
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

void levelOrder(TreeNode *root)
{
    if (!root)
    {
        cout << "[]" << endl;
        return;
    }

    queue<TreeNode *> q;
    q.push(root);
    int remaining = 1;
    vector<string> result;

    while (!q.empty() && remaining > 0)
    {
        TreeNode *current = q.front();
        q.pop();

        if (current == nullptr)
        {
            result.push_back("null");
            q.push(nullptr);
            q.push(nullptr);
        }
        else
        {
            result.push_back(to_string(current->val));
            remaining--;
            q.push(current->left);
            if (current->left != nullptr)
                remaining++;
            q.push(current->right);
            if (current->right != nullptr)
                remaining++;
        }
    }

    while (!result.empty() && result.back() == "null")
    {
        result.pop_back();
    }

    cout << "[";
    for (size_t i = 0; i < result.size(); ++i)
    {
        cout << result[i];
        if (i != result.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

// 你可以通过修改结构来完成本Lab。
// main函数，头文件，Solution类可以自行添加。
class Solution
{
private:
    // 使用 map 作为记忆化缓存，避免重复计算
    map<pair<int, int>, vector<TreeNode*>> memo;
    
    // 递归生成从 start 到 end 的所有可能的二叉搜索树
    vector<TreeNode*> generate(int start, int end)
    {
        vector<TreeNode*> result;
        
        // 基础情况：如果 start > end，返回包含一个 nullptr 的列表
        if (start > end)
        {
            result.push_back(nullptr);
            return result;
        }
        
        // 检查缓存
        pair<int, int> key = {start, end};
        if (memo.find(key) != memo.end())
        {
            return memo[key];
        }
        
        // 依次选择 start 到 end 之间的每个数字作为根节点
        for (int i = start; i <= end; i++)
        {
            // 递归生成所有可能的左子树（值在 [start, i-1] 范围内）
            vector<TreeNode*> leftTrees = generate(start, i - 1);
            
            // 递归生成所有可能的右子树（值在 [i+1, end] 范围内）
            vector<TreeNode*> rightTrees = generate(i + 1, end);
            
            // 组合所有可能的左右子树
            for (TreeNode* left : leftTrees)
            {
                for (TreeNode* right : rightTrees)
                {
                    // 创建新的根节点
                    TreeNode* root = new TreeNode(i);
                    root->left = left;
                    root->right = right;
                    result.push_back(root);
                }
            }
        }
        
        // 存入缓存
        memo[key] = result;
        return result;
    }
    
public:
    vector<TreeNode *> generateTrees(int n)
    {
        if (n == 0)
        {
            return {};
        }
        
        // 清空缓存（如果要多次调用）
        memo.clear();
        
        // 生成值从 1 到 n 的所有可能的二叉搜索树
        return generate(1, n);
    }
};

int main()
{
    Solution s;
    
    // 测试用例1：n = 3
    // 应该生成 5 个不同的二叉搜索树（Catalan数：C_3 = 5）
    cout << "测试用例1：n = 3" << endl;
    cout << "输出所有可能的二叉搜索树：" << endl;
    vector<TreeNode *> res1 = s.generateTrees(3);
    cout << "[";
    for (size_t i = 0; i < res1.size(); ++i)
    {
        levelOrder(res1[i]);
        if (i != res1.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "共生成 " << res1.size() << " 棵不同的二叉搜索树" << endl;
    cout << endl;
    
    // 测试用例2：n = 1
    // 应该生成 1 个二叉搜索树（只有根节点）
    cout << "测试用例2：n = 1" << endl;
    cout << "输出所有可能的二叉搜索树：" << endl;
    vector<TreeNode *> res2 = s.generateTrees(1);
    cout << "[";
    for (size_t i = 0; i < res2.size(); ++i)
    {
        levelOrder(res2[i]);
        if (i != res2.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "共生成 " << res2.size() << " 棵不同的二叉搜索树" << endl;
    cout << endl;
    
    // 测试用例3：n = 4
    // 应该生成 14 个不同的二叉搜索树（Catalan数：C_4 = 14）
    cout << "测试用例3：n = 4" << endl;
    cout << "输出所有可能的二叉搜索树：" << endl;
    vector<TreeNode *> res3 = s.generateTrees(4);
    cout << "[";
    for (size_t i = 0; i < res3.size(); ++i)
    {
        levelOrder(res3[i]);
        if (i != res3.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "共生成 " << res3.size() << " 棵不同的二叉搜索树" << endl;
    cout << endl;
    
    return 0;
}
