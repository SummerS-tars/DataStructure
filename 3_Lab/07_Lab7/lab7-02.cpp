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

TreeNode *buildTree(const vector<string> &nodes)
{
    if (nodes.empty() || nodes[0] == "null")
    {
        return nullptr; // 如果节点列表为空或第一个节点是null，返回空树
    }

    TreeNode *root = new TreeNode(stoi(nodes[0])); // 根节点
    queue<TreeNode *> q;                           // 队列用于层序构造
    q.push(root);
    int i = 1; // 指向下一个节点的位置

    while (!q.empty() && i < nodes.size())
    {
        TreeNode *current = q.front(); // 当前处理的节点
        q.pop();

        // 如果当前节点为空，需要在队列中插入两个null占位符来维持层序结构
        if (current == nullptr)
        {
            q.push(nullptr); // 左子节点的占位符
            q.push(nullptr); // 右子节点的占位符
            i += 2;
            continue;
        }

        // 构造左子节点
        if (i < nodes.size())
        {
            if (nodes[i] != "null")
            {
                current->left = new TreeNode(stoi(nodes[i])); // 非null值构造子节点
                q.push(current->left);                        // 左子节点加入队列
            }
            else
            {
                current->left = nullptr; // null值则将左子节点置为空
                q.push(nullptr);         // 左子节点的占位符加入队列
            }
        }
        i++;

        // 构造右子节点
        if (i < nodes.size())
        {
            if (nodes[i] != "null")
            {
                current->right = new TreeNode(stoi(nodes[i])); // 非null值构造子节点
                q.push(current->right);                        // 右子节点加入队列
            }
            else
            {
                current->right = nullptr; // null值则将右子节点置为空
                q.push(nullptr);          // 右子节点的占位符加入队列
            }
        }
        i++;
    }

    return root; // 返回构造好的树的根节点
}

// 你可以通过修改结构来完成本Lab。
// main函数，头文件，Solution类可以自行添加。

// 判断子树是否包含1
// 如果子树不包含1，返回nullptr（剪枝）
// 如果子树包含1，返回根节点
TreeNode *pruneTree(TreeNode *root)
{
    if (root == nullptr)
    {
        return nullptr;
    }
    
    // 后序遍历：先处理左右子树
    root->left = pruneTree(root->left);
    root->right = pruneTree(root->right);
    
    // 如果当前节点是0，且左右子树都为空（都被剪枝了），则剪掉当前节点
    if (root->val == 0 && root->left == nullptr && root->right == nullptr)
    {
        return nullptr;
    }
    
    // 否则保留当前节点
    return root;
}

int main()
{
    // 测试用例1：Example1
    // 输入：root = [1,null,0,null,null,0,1]
    // 输出：[1,null,0,null,null,null,1]
    cout << "测试用例1：" << endl;
    cout << "输入：";
    vector<string> nodes1 = {"1", "null", "0", "null", "null", "0", "1"};
    TreeNode *root1 = buildTree(nodes1);
    levelOrder(root1);
    cout << "输出：";
    root1 = pruneTree(root1);
    levelOrder(root1);
    cout << endl;
    
    // 测试用例2：Example2
    // 输入：root = [1,1,0,1,1,0,1,0]
    // 输出：[1,1,0,1,1,null,1]
    cout << "测试用例2：" << endl;
    cout << "输入：";
    vector<string> nodes2 = {"1", "1", "0", "1", "1", "0", "1", "0"};
    TreeNode *root2 = buildTree(nodes2);
    levelOrder(root2);
    cout << "输出：";
    root2 = pruneTree(root2);
    levelOrder(root2);
    cout << endl;
    
    // 测试用例3：所有节点都是0的情况
    // 输入：root = [0,0,0,0,0]
    // 输出：[]（整棵树被剪枝）
    cout << "测试用例3：" << endl;
    cout << "输入：";
    vector<string> nodes3 = {"0", "0", "0", "0", "0"};
    TreeNode *root3 = buildTree(nodes3);
    levelOrder(root3);
    cout << "输出：";
    root3 = pruneTree(root3);
    levelOrder(root3);
    cout << endl;
    
    return 0;
}
