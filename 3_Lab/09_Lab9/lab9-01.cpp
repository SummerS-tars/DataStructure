/*
 * 题目：统计正整数和负整数的个数
 * 
 * 给定一个按非递减顺序排列的整数数组 nums，返回数组中正整数和负整数的个数
 * (0既不是正整数，也不是负整数)
 * 
 * 时间复杂度要求：O(log n)
 * 
 * 算法思路：
 * 由于数组已按非递减顺序排列，我们可以使用二分查找：
 * 1. 使用二分查找找到第一个正数（>0）的位置
 *    - 正整数个数 = n - 第一个正数的位置
 * 2. 使用二分查找找到最后一个负数（<0）的位置
 *    - 负整数个数 = 最后一个负数的位置 + 1
 * 
 * 时间复杂度分析：两次二分查找，每次 O(log n)，总复杂度为 O(log n)
 */

#include <iostream>
#include <vector>

using namespace std;

/**
 * 使用二分查找找到第一个大于 target 的元素位置
 * 
 * @param nums 已排序的整数数组
 * @param target 目标值
 * @return 第一个大于 target 的元素的位置，如果不存在则返回 nums.size()
 */
int binarySearchFirstGreater(vector<int> &nums, int target)
{
    int left = 0, right = nums.size();
    
    // 标准二分查找：找到第一个大于 target 的位置
    while (left < right)
    {
        int mid = left + (right - left) / 2;
        if (nums[mid] <= target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    
    return left;
}

/**
 * 统计正整数和负整数的个数
 * 
 * 算法流程：
 * 1. 使用二分查找找到第一个正数（>0）的位置
 * 2. 使用二分查找找到最后一个负数（<0）的位置
 * 3. 计算个数并返回
 * 
 * @param nums 按非递减顺序排列的整数数组
 * @return {负整数个数, 正整数个数}
 */
vector<int> Count(vector<int> &nums)
{
    int n = nums.size();
    
    // 找到第一个正数的位置（第一个 > 0 的元素）
    int firstPositionIdx = binarySearchFirstGreater(nums, 0);
    
    // 正整数个数 = 总数 - 第一个正数的位置
    int positiveCount = n - firstPositionIdx;
    
    // 负整数个数 = 第一个 >= 0 的位置
    // 即找第一个 >= 0 的位置，其前面的都是负数
    int firstNonNegativeIdx = binarySearchFirstGreater(nums, -1);
    int negativeCount = firstNonNegativeIdx;
    
    return {negativeCount, positiveCount};
}

/**
 * 打印数组函数（用于测试）
 */
void printArray(vector<int> &nums)
{
    cout << "数组: [";
    for (int i = 0; i < nums.size(); i++)
    {
        cout << nums[i];
        if (i < nums.size() - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}

/**
 * 测试函数
 */
void runTest(int testNum, vector<int> &nums, int expectedNeg, int expectedPos)
{
    cout << "\n========== 测试用例 " << testNum << " ==========" << endl;
    printArray(nums);
    
    vector<int> ans = Count(nums);
    int negCount = ans[0];
    int posCount = ans[1];
    
    cout << "输出: " << negCount << " " << posCount << endl;
    cout << "期望: " << expectedNeg << " " << expectedPos << endl;
    
    if (negCount == expectedNeg && posCount == expectedPos)
    {
        cout << "✓ 通过" << endl;
    }
    else
    {
        cout << "✗ 失败" << endl;
    }
}

int main()
{
    cout << "=== 统计正整数和负整数个数 ===" << endl;
    cout << "算法：二分查找，时间复杂度 O(log n)" << endl;
    
    // 测试用例 1：混合负数、零和正数
    vector<int> test1 = {-2, -1, -1, 1, 2, 3};
    runTest(1, test1, 3, 3);
    
    // 测试用例 2：包含多个零
    vector<int> test2 = {-3, -2, -1, 0, 0, 1, 2};
    runTest(2, test2, 3, 2);
    
    // 测试用例 3：全是正数（无零无负数）
    vector<int> test3 = {5, 20, 66, 1314};
    runTest(3, test3, 0, 4);
    
    // 额外测试用例 4：全是负数
    vector<int> test4 = {-100, -50, -3, -1};
    runTest(4, test4, 4, 0);
    
    // 额外测试用例 5：全是零
    vector<int> test5 = {0, 0, 0, 0};
    runTest(5, test5, 0, 0);
    
    // 额外测试用例 6：只有一个正数和一个负数
    vector<int> test6 = {-1, 1};
    runTest(6, test6, 1, 1);
    
    return 0;
}