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

/**
 * 二叉搜索树的最近公共祖先（利用BST性质）
 * 
 * 算法思路：
 * 利用二叉搜索树的性质：左子树 < 根节点 < 右子树
 * - 如果 a, b 都小于 root，则最近公共祖先在左子树，递归处理左子树
 * - 如果 a, b 都大于 root，则最近公共祖先在右子树，递归处理右子树
 * - 否则（a <= root <= b 或 b <= root <= a），root 就是最近公共祖先
 * 
 * 时间复杂度：O(h)，其中 h 是树的高度，平衡树下为 O(log n)
 * 空间复杂度：O(h)，递归栈深度
 * 
 * @param root 二叉搜索树的根节点
 * @param a 第一个节点的值
 * @param b 第二个节点的值
 * @return 两个节点的最近公共祖先节点指针
 */
TreeNode *lowestCommonAncestor(TreeNode *root, int a, int b)
{
    // 基本情况：树为空
    if (root == nullptr)
    {
        return nullptr;
    }

    // 确保 a <= b（方便后续处理）
    if (a > b)
    {
        swap(a, b);
    }

    // 利用BST性质：
    // 如果 a, b 都小于 root->val，说明最近公共祖先在左子树
    if (b < root->val)
    {
        return lowestCommonAncestor(root->left, a, b);
    }

    // 如果 a, b 都大于 root->val，说明最近公共祖先在右子树
    if (a > root->val)
    {
        return lowestCommonAncestor(root->right, a, b);
    }

    // 否则 a <= root->val <= b 或 a == root->val 或 b == root->val
    // root 就是最近公共祖先
    return root;
}

/**
 * 迭代版本（可选）- 不使用递归栈空间
 * 时间复杂度：O(h)
 * 空间复杂度：O(1)
 */
TreeNode *lowestCommonAncestorIterative(TreeNode *root, int a, int b)
{
    // 确保 a <= b
    if (a > b)
    {
        swap(a, b);
    }

    // 从根节点开始，不断向下移动
    while (root != nullptr)
    {
        // 如果 a, b 都小于当前节点，向左移动
        if (b < root->val)
        {
            root = root->left;
        }
        // 如果 a, b 都大于当前节点，向右移动
        else if (a > root->val)
        {
            root = root->right;
        }
        // 否则当前节点就是最近公共祖先
        else
        {
            return root;
        }
    }

    return nullptr;
}

int main()
{
    cout << "========== 测试用例 1 ==========" << endl;
    // 测试用例1：[6,2,8,0,4,7,9,null,null,3,5], p=2, q=8
    // 树结构：
    //        6
    //       / \
    //      2   8
    //     / \ / \
    //    0  4 7  9
    //      / \
    //     3   5
    vector<string> nodes1 = {"6", "2", "8", "0", "4", "7", "9", "null", "null", "3", "5"};
    TreeNode *root1 = buildTree(nodes1);
    TreeNode *lca1_a = lowestCommonAncestor(root1, 2, 8);
    TreeNode *lca1_b = lowestCommonAncestor(root1, 2, 4);
    cout << "输入: root = [6,2,8,0,4,7,9,null,null,3,5], p = 2, q = 8" << endl;
    cout << "输出: " << lca1_a->val << " (期望: 6)" << endl;
    cout << "输入: root = [6,2,8,0,4,7,9,null,null,3,5], p = 2, q = 4" << endl;
    cout << "输出: " << lca1_b->val << " (期望: 2)" << endl;

    cout << "\n========== 测试用例 2 ==========" << endl;
    // 测试用例2：节点自己是自己的祖先
    // 树结构：
    //        10
    //       /  \
    //      5   15
    //     / \
    //    3   7
    vector<string> nodes2 = {"10", "5", "15", "3", "7"};
    TreeNode *root2 = buildTree(nodes2);
    TreeNode *lca2_a = lowestCommonAncestor(root2, 5, 7);
    TreeNode *lca2_b = lowestCommonAncestor(root2, 5, 5); // 节点与自己的最近公共祖先是自己
    cout << "输入: root = [10,5,15,3,7], p = 5, q = 7" << endl;
    cout << "输出: " << lca2_a->val << " (期望: 5)" << endl;
    cout << "输入: root = [10,5,15,3,7], p = 5, q = 5" << endl;
    cout << "输出: " << lca2_b->val << " (期望: 5)" << endl;

    cout << "\n========== 测试用例 3 ==========" << endl;
    // 测试用例3：一个节点在另一个的子树中
    // 树结构：
    //        20
    //       /
    //      10
    //     / \
    //    5  15
    //   /
    //  2
    vector<string> nodes3 = {"20", "10", "null", "5", "15", "2"};
    TreeNode *root3 = buildTree(nodes3);
    TreeNode *lca3_a = lowestCommonAncestor(root3, 10, 2);
    TreeNode *lca3_b = lowestCommonAncestor(root3, 5, 15);
    cout << "输入: root = [20,10,null,5,15,2], p = 10, q = 2" << endl;
    cout << "输出: " << lca3_a->val << " (期望: 10)" << endl;
    cout << "输入: root = [20,10,null,5,15,2], p = 5, q = 15" << endl;
    cout << "输出: " << lca3_b->val << " (期望: 10)" << endl;

    cout << "\n========== 测试迭代版本 ==========" << endl;
    TreeNode *lca_iter = lowestCommonAncestorIterative(root1, 2, 8);
    cout << "迭代版本测试: " << lca_iter->val << " (期望: 6)" << endl;

    return 0;
}
