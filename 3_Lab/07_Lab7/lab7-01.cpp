#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

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
    unordered_map<int, int> inorder_map; // 用于快速查找中序遍历中元素的位置
    
    TreeNode* build(vector<int>& preorder, int pre_start, int pre_end,
                    vector<int>& inorder, int in_start, int in_end)
    {
        // 递归终止条件
        if (pre_start > pre_end || in_start > in_end)
        {
            return nullptr;
        }
        
        // 前序遍历的第一个元素是根节点
        int root_val = preorder[pre_start];
        TreeNode* root = new TreeNode(root_val);
        
        // 在中序遍历中找到根节点的位置
        int root_index = inorder_map[root_val];
        
        // 计算左子树的节点数量
        int left_size = root_index - in_start;
        
        // 递归构建左子树和右子树
        // 左子树：
        //   前序遍历范围：[pre_start + 1, pre_start + left_size]
        //   中序遍历范围：[in_start, root_index - 1]
        root->left = build(preorder, pre_start + 1, pre_start + left_size,
                          inorder, in_start, root_index - 1);
        
        // 右子树：
        //   前序遍历范围：[pre_start + left_size + 1, pre_end]
        //   中序遍历范围：[root_index + 1, in_end]
        root->right = build(preorder, pre_start + left_size + 1, pre_end,
                           inorder, root_index + 1, in_end);
        
        return root;
    }
    
public:
    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
    {
        // 构建中序遍历的值到索引的映射，用于O(1)查找
        for (int i = 0; i < inorder.size(); i++)
        {
            inorder_map[inorder[i]] = i;
        }
        
        // 开始递归构建
        return build(preorder, 0, preorder.size() - 1,
                    inorder, 0, inorder.size() - 1);
    }
};

int main()
{
    Solution s;
    
    // 测试用例1：示例树
    // 树的结构：
    //       3
    //      / \
    //     9  20
    //       /  \
    //      15   7
    cout << "测试用例1: ";
    vector<int> preorder1 = {3, 9, 20, 15, 7};
    vector<int> inorder1 = {9, 3, 15, 20, 7};
    TreeNode *root1 = s.buildTree(preorder1, inorder1);
    levelOrder(root1);
    
    // 测试用例2：只有左子树的树
    // 树的结构：
    //       1
    //      /
    //     2
    //    /
    //   3
    cout << "测试用例2: ";
    vector<int> preorder2 = {1, 2, 3};
    vector<int> inorder2 = {3, 2, 1};
    TreeNode *root2 = s.buildTree(preorder2, inorder2);
    levelOrder(root2);
    
    // 测试用例3：完全二叉树
    // 树的结构：
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    cout << "测试用例3: ";
    vector<int> preorder3 = {1, 2, 4, 5, 3};
    vector<int> inorder3 = {4, 2, 5, 1, 3};
    TreeNode *root3 = s.buildTree(preorder3, inorder3);
    levelOrder(root3);
    
    // 测试用例4：单个节点
    cout << "测试用例4: ";
    vector<int> preorder4 = {1};
    vector<int> inorder4 = {1};
    TreeNode *root4 = s.buildTree(preorder4, inorder4);
    levelOrder(root4);
    
    return 0;
}
