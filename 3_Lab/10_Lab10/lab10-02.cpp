#include <iostream>
#include <vector>

using namespace std;

/*
 * ========================================
 * 找出所有出现两次的整数 - 总体思路详解
 * ========================================
 * 
 * 问题分析：
 * 给定一个数组，长度为n，所有数都在[1, n]范围内，每个数最多出现两次
 * 要找出所有出现两次的数字，并用O(n)时间和O(1)空间搞定（不计输出空间）
 * 
 * 关键约束：
 * 1. 时间复杂度必须是O(n)：不能排序（O(n log n)）也不能用哈希表（虽然快但要额外空间）
 * 2. 空间复杂度必须是O(1)：不能用额外数据结构，只能用常数个变量
 * 3. 数值范围特殊：所有数都在[1, n]范围内，这是关键突破口！
 * 
 * 解题思路 - "数组下标标记法"：
 * 
 * 思考一个问题：如果数都在[1, n]，那我能不能用数组本身来做标记？
 * 
 * 核心思想就是：利用数组的下标！
 * - 把nums[i]看作一个"指令"，告诉我们去标记nums[i] - 1这个下标
 * - 但怎么标记呢？我们把那个位置的值变成负数！
 * - 如果一个值第二次被当成下标时，发现那个位置已经是负数了，那就说明这个数出现了两次！
 * 
 * 举个例子来理解：
 * 输入：nums = [4, 3, 2, 7, 8, 2, 3, 1]
 * 
 * 第1个数是4：
 *   - 去索引3的位置（下标从0开始，所以4对应下标3）
 *   - 把nums[3] = 7改成-7
 *   - 数组变成：[4, 3, 2, -7, 8, 2, 3, 1]
 * 
 * 第2个数是3：
 *   - 去索引2的位置
 *   - 把nums[2] = 2改成-2
 *   - 数组变成：[4, 3, -2, -7, 8, 2, 3, 1]
 * 
 * 第3个数是2：
 *   - 去索引1的位置
 *   - 把nums[1] = 3改成-3
 *   - 数组变成：[4, -3, -2, -7, 8, 2, 3, 1]
 * 
 * ... 继续处理 ...
 * 
 * 第7个数是2（第二次出现）：
 *   - 去索引1的位置
 *   - 发现nums[1]已经是-3（是负数！）
 *   - 说明2出现过两次！加入结果
 * 
 * 第8个数是3（第二次出现）：
 *   - 去索引2的位置
 *   - 发现nums[2]已经是-2（是负数！）
 *   - 说明3出现过两次！加入结果
 * 
 * 最终结果：[2, 3]
 * 
 * 为什么能这样做：
 * 1. 数都在[1, n]范围内，所以任意数num对应的下标num-1都是有效的
 * 2. 每个数最多出现两次，所以只需要两轮判断
 * 3. 用正负号来标记，无需额外空间
 * 
 * 复杂度分析：
 * - 时间复杂度：O(n)，只需要遍历一遍数组
 * - 空间复杂度：O(1)，只用了常数个变量
 */

vector<int> findDuplicates(vector<int> &nums)
{
    vector<int> result;
    
    // 遍历数组中的每个数
    for (int i = 0; i < nums.size(); i++)
    {
        // 获取当前数的绝对值（因为可能已经被标记为负数了）
        // 比如nums[i]可能是-7，绝对值后得到7
        int num = abs(nums[i]);
        
        // 用这个数作为下标，去数组中找对应位置
        // num的范围是[1, n]，所以num-1的范围是[0, n-1]，正好是有效下标
        int index = num - 1;
        
        // 检查nums[index]是否已经被标记成负数
        if (nums[index] < 0)
        {
            // 已经是负数了，说明这个数num出现了第二次！
            // 把num加入结果
            result.push_back(num);
        }
        else
        {
            // 还是正数，说明这是这个数的第一次出现
            // 把nums[index]标记成负数，为下次出现做准备
            nums[index] = -nums[index];
        }
    }
    
    return result;
}

int main()
{
    // ==================== 测试用例1 ====================
    // 示例：nums = [4,3,2,7,8,2,3,1]
    // 预期：[2,3]
    // 说明：2和3各出现了2次
    vector<int> nums1 = {4, 3, 2, 7, 8, 2, 3, 1};
    cout << "Test 1 Input: [4,3,2,7,8,2,3,1]" << endl;
    cout << "Test 1 Output: ";
    vector<int> res1 = findDuplicates(nums1);
    for (int num : res1)
    {
        cout << num << " ";
    }
    cout << endl << endl;

    // ==================== 测试用例2 ====================
    // 示例：nums = [1,1,2]
    // 预期：[1]
    // 说明：只有1出现了2次
    vector<int> nums2 = {1, 1, 2};
    cout << "Test 2 Input: [1,1,2]" << endl;
    cout << "Test 2 Output: ";
    vector<int> res2 = findDuplicates(nums2);
    for (int num : res2)
    {
        cout << num << " ";
    }
    cout << endl << endl;

    // ==================== 测试用例3 ====================
    // 示例：nums = [1]
    // 预期：[]
    // 说明：没有数出现两次
    vector<int> nums3 = {1};
    cout << "Test 3 Input: [1]" << endl;
    cout << "Test 3 Output: ";
    vector<int> res3 = findDuplicates(nums3);
    for (int num : res3)
    {
        cout << num << " ";
    }
    if (res3.empty())
        cout << "(empty)";
    cout << endl << endl;

    // ==================== 测试用例4 ====================
    // 自定义：nums = [2,1,1,2]
    // 预期：[1,2]
    // 说明：1和2都各出现了2次
    vector<int> nums4 = {2, 1, 1, 2};
    cout << "Test 4 Input: [2,1,1,2]" << endl;
    cout << "Test 4 Output: ";
    vector<int> res4 = findDuplicates(nums4);
    for (int num : res4)
    {
        cout << num << " ";
    }
    cout << endl << endl;

    // ==================== 测试用例5 ====================
    // 自定义：nums = [5,4,3,2,1]
    // 预期：[]
    // 说明：没有任何数出现两次，所有数都只出现一次
    vector<int> nums5 = {5, 4, 3, 2, 1};
    cout << "Test 5 Input: [5,4,3,2,1]" << endl;
    cout << "Test 5 Output: ";
    vector<int> res5 = findDuplicates(nums5);
    for (int num : res5)
    {
        cout << num << " ";
    }
    if (res5.empty())
        cout << "(empty)";
    cout << endl << endl;

    // ==================== 测试用例6 ====================
    // 自定义：nums = [1,2,3,4,5,5,4,3,2,1]
    // 预期：[1,2,3,4,5]
    // 说明：所有数都各出现了2次
    vector<int> nums6 = {1, 2, 3, 4, 5, 5, 4, 3, 2, 1};
    cout << "Test 6 Input: [1,2,3,4,5,5,4,3,2,1]" << endl;
    cout << "Test 6 Output: ";
    vector<int> res6 = findDuplicates(nums6);
    for (int num : res6)
    {
        cout << num << " ";
    }
    cout << endl << endl;

    // ==================== 测试用例7 ====================
    // 自定义：nums = [3,3]
    // 预期：[3]
    // 说明：最简单的重复情况
    vector<int> nums7 = {3, 3};
    cout << "Test 7 Input: [3,3]" << endl;
    cout << "Test 7 Output: ";
    vector<int> res7 = findDuplicates(nums7);
    for (int num : res7)
    {
        cout << num << " ";
    }
    cout << endl << endl;

    // ==================== 测试用例8 ====================
    // 自定义：nums = [2,2,1]
    // 预期：[2]
    // 说明：2出现两次，1只出现一次
    vector<int> nums8 = {2, 2, 1};
    cout << "Test 8 Input: [2,2,1]" << endl;
    cout << "Test 8 Output: ";
    vector<int> res8 = findDuplicates(nums8);
    for (int num : res8)
    {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}