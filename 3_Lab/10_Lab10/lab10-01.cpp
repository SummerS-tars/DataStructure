#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <algorithm>

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

        // 如果当前节点为空，不需要处理其子节点
        if (current == nullptr)
        {
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
                q.push(nullptr); // null节点也入队，保持层序结构
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
                q.push(nullptr); // null节点也入队，保持层序结构
            }
        }
        i++;
    }

    return root; // 返回构造好的树的根节点
}

/*
 * ========================================
 * AVL树验证 - 总体思路详解
 * ========================================
 * 
 * 一句话总结：
 * 要判断一棵树是否是有效的AVL树，咱们需要同时检验两个条件。如果两个条件都满足，
 * 那这就是一棵合法的AVL树。只要有其中一个不满足，就直接判定为非法。
 * 
 * 核心检验条件：
 * 
 * 条件1：二叉搜索树(BST)性质
 *   - 简单说就是：要保证"左小右大"的秩序
 *   - 对于每个节点，它的左子树中所有的值都要比它小，右子树中所有的值都要比它大
 *   - 比如，如果根节点是5，那左子树里不能有5或比5大的数，右子树里不能有5或比5小的数
 * 
 * 条件2：AVL平衡性质
 *   - 简单说就是：不能让树跑得太偏，左右子树的高度要差不多
 *   - 具体来说，对于任意节点，它的左右子树的高度差不能超过1
 *   - 换个角度想，如果左子树特别高，右子树特别矮，这树就不平衡了
 * 
 * 算法设计思路：
 * 
 * 使用"自下而上"的递归方法（后序遍历DFS）：
 *   - 从最底层的叶子节点开始，一层层往上检查
 *   - 对于每个节点，先递归检查左右子树，确保它们都是合法的AVL树
 *   - 然后再检查当前这个节点是否满足AVL树的两个条件
 *   - 如果当前节点不满足条件，就直接返回false，上面的节点也跟着返回false
 *   - 这样一直冒泡到根节点，最后就知道整棵树是不是AVL树了
 * 
 * 信息收集（递归函数返回什么）：
 * 
 * 为了方便检查，我们在递归过程中收集四个关键信息：
 *   1. isValid - 子树是否是合法的AVL树（布尔值）
 *   2. height - 子树的高度（用来计算左右子树的高度差）
 *   3. minVal - 子树中最小的那个值（用来检查BST的左边界）
 *   4. maxVal - 子树中最大的那个值（用来检查BST的右边界）
 * 
 * 
 * 时间复杂度：O(n)
 *   - 每个节点只需要访问一次，所以是线性时间
 * 
 * 空间复杂度：O(h)，其中h为树的高度
 *   - 因为用了递归，深度就是树的高度h
 *   - 最坏情况下（链式树），h = n，所以空间是O(n)
 *   - 最好情况下（平衡树），h = log(n)，所以空间是O(log n)
 */

struct AVLInfo
{
    bool isValid;      // 是否为有效的AVL树
    int height;        // 子树的高度
    long long minVal;  // 子树中的最小值
    long long maxVal;  // 子树中的最大值
};

// 辅助函数：检验子树是否为有效的AVL树，返回树的信息
// 参数：root - 待检查的子树根节点
// 返回：包含检查结果和树的相关信息的AVLInfo结构体
AVLInfo checkAVL(TreeNode *root)
{
    // ========== 第1步：处理空树的基础情况 ==========
    // 空树是合法的AVL树，高度记为-1，这样便于计算
    // minVal设为最大值，maxVal设为最小值，这样在与实际值比较时不会出问题
    if (root == nullptr)
    {
        return {true, -1, LLONG_MAX, LLONG_MIN};
    }

    // ========== 第2步：递归检查左右子树 ==========
    // 先让左右子树自己检查自己是否合法
    // 通过递归调用，自下而上地逐层检查
    AVLInfo left = checkAVL(root->left);
    AVLInfo right = checkAVL(root->right);

    // ========== 第3步：计算当前节点所在子树的基本信息 ==========
    AVLInfo current;
    
    // 高度 = max(左子树高度, 右子树高度) + 1
    // 比如左子树高度是2，右子树高度是1，那这个节点所在的子树高度就是3
    current.height = max(left.height, right.height) + 1;
    
    // 计算子树中的最小值
    // 逻辑很简单：左子树如果存在(height >= 0)，那最小值就在左子树里
    // 如果左子树不存在，那最小值就是当前节点的值
    if (left.height >= 0)
    {
        current.minVal = left.minVal;
    }
    else
    {
        current.minVal = root->val;
    }
    
    // 计算子树中的最大值
    // 同样的逻辑：右子树如果存在(height >= 0)，那最大值就在右子树里
    // 如果右子树不存在，那最大值就是当前节点的值
    if (right.height >= 0)
    {
        current.maxVal = right.maxVal;
    }
    else
    {
        current.maxVal = root->val;
    }

    // ========== 第4步：检查左右子树是否都是合法的AVL树 ==========
    // 这是个快速失败机制：只要发现左子树或右子树不合法，
    // 整个树就不合法，咱们直接返回false，不用继续检查了
    if (!left.isValid || !right.isValid)
    {
        current.isValid = false;
        return current;
    }

    // ========== 第5步：检查AVL平衡性质 ==========
    // AVL树的第一个重要特性：左右子树的高度差不能超过1
    // 比如左子树高度是5，右子树高度是2，差了3，就不平衡了
    int heightDiff = abs(left.height - right.height);
    if (heightDiff > 1)
    {
        // 不满足AVL平衡性质，判定为非法
        current.isValid = false;
        return current;
    }

    // ========== 第6步：检查二叉搜索树(BST)性质 ==========
    // AVL树的第二个重要特性：必须满足BST的"左小右大"特性
    // 
    // 对于当前节点（值为root->val）：
    //   - 左子树中的所有值都要 < root->val
    //     所以左子树的最大值不能 >= root->val
    //   - 右子树中的所有值都要 > root->val
    //     所以右子树的最小值不能 <= root->val
    //
    // 举个例子：假设当前节点是10
    //   - 如果左子树存在且最大值是8，没问题，8 < 10 ✓
    //   - 如果左子树存在且最大值是12，不行，12 > 10 ✗
    //   - 如果右子树存在且最小值是15，没问题，15 > 10 ✓
    //   - 如果右子树存在且最小值是7，不行，7 < 10 ✗
    if ((left.height >= 0 && left.maxVal >= root->val) ||
        (right.height >= 0 && right.minVal <= root->val))
    {
        // 违反了BST性质，判定为非法
        current.isValid = false;
        return current;
    }

    // ========== 第7步：都通过检查，返回合法结果 ==========
    // 如果执行到这里，说明当前子树满足所有条件：
    // 1. 左右子树都是合法的AVL树 ✓
    // 2. 左右子树高度差不超过1 ✓
    // 3. 满足BST的"左小右大"特性 ✓
    current.isValid = true;
    return current;
}

bool isAVL(TreeNode *root)
{
    return checkAVL(root).isValid;
}

int main()
{
    // 测试用例1：简单的平衡树 [2, 1, 3]
    // 预期: true
    vector<string> test1 = {"2", "1", "3"};
    TreeNode *root1 = buildTree(test1);
    cout << "Test 1 [2, 1, 3]: " << (isAVL(root1) ? "true" : "false") << endl;

    // 测试用例2：高度差超过1的树 [1, 2, 2, 3, 3, null, null, 4, 4]
    // 预期: false
    vector<string> test2 = {"1", "2", "2", "3", "3", "null", "null", "4", "4"};
    TreeNode *root2 = buildTree(test2);
    cout << "Test 2 [1, 2, 2, 3, 3, null, null, 4, 4]: " << (isAVL(root2) ? "true" : "false") << endl;

    // 测试用例3：违反BST性质 [5, 4, 6, null, null, 3, 7]
    // 预期: false (右子树中有值3 < 根节点5，违反BST性质)
    vector<string> test3 = {"5", "4", "6", "null", "null", "3", "7"};
    TreeNode *root3 = buildTree(test3);
    cout << "Test 3 [5, 4, 6, null, null, 3, 7]: " << (isAVL(root3) ? "true" : "false") << endl;

    // 测试用例4：平衡的较大树 [5, 4, 7, 2, null, 6, 8, null, null, null, null, null, null, null, 9]
    // 预期: true
    vector<string> test4 = {"5", "4", "7", "2", "null", "6", "8", "null", "null", "null", "null", "null", "null", "null", "9"};
    TreeNode *root4 = buildTree(test4);
    cout << "Test 4 [5, 4, 7, 2, null, 6, 8, ...]: " << (isAVL(root4) ? "true" : "false") << endl;

    // 测试用例5：空树
    // 预期: true (空树是有效的AVL树)
    vector<string> test5 = {};
    TreeNode *root5 = buildTree(test5);
    cout << "Test 5 [empty]: " << (isAVL(root5) ? "true" : "false") << endl;

    // 测试用例6：单节点树
    // 预期: true
    vector<string> test6 = {"1"};
    TreeNode *root6 = buildTree(test6);
    cout << "Test 6 [1]: " << (isAVL(root6) ? "true" : "false") << endl;

    // 测试用例7：完全二叉树且是BST [1, 0, 2]
    // 预期: true
    vector<string> test7 = {"1", "0", "2"};
    TreeNode *root7 = buildTree(test7);
    cout << "Test 7 [1, 0, 2]: " << (isAVL(root7) ? "true" : "false") << endl;

    // 测试用例8：右子树更深 [1, null, 2, null, 3]
    // 预期: false (高度差为2)
    vector<string> test8 = {"1", "null", "2", "null", "3"};
    TreeNode *root8 = buildTree(test8);
    cout << "Test 8 [1, null, 2, null, 3]: " << (isAVL(root8) ? "true" : "false") << endl;

    // 测试用例9：有效的AVL树 [4, 2, 6, 1, 3, 5, 7]
    // 预期: true (完美二叉树)
    vector<string> test9 = {"4", "2", "6", "1", "3", "5", "7"};
    TreeNode *root9 = buildTree(test9);
    cout << "Test 9 [4, 2, 6, 1, 3, 5, 7]: " << (isAVL(root9) ? "true" : "false") << endl;

    return 0;
}
