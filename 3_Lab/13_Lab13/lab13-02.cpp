#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

/*
 * 题目：前 K 个高频元素
 * 给定整数数组 nums 和整数 k，返回出现频率前 k 高的元素，顺序任意。
 * 要求时间复杂度优于 O(n log n)。
 *
 * 思路（桶排序 / 计数分桶，O(n) 期望）：
 * 1) 先用哈希表统计每个元素出现的次数：count[x]
 * 2) 设最多出现次数不超过 n，则建立大小为 n+1 的“桶” buckets，其中 buckets[f] 存放出现频次为 f 的所有元素。
 * 3) 从频次最大（n）向下扫描，将桶内的元素依次加入答案，直到收集到 k 个为止。
 *
 * 复杂度：
 * - 统计 O(n)，分桶 O(U)（U 为不同元素个数，U<=n），扫描桶 O(n)
 * - 总体 O(n)
 * 空间：O(n)
 *
 * 边界：
 * - k 在 [1, 去重元素个数] 范围；若 k==0 可直接返回空（不在题面必要范围）。
 */

vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> cnt;
    cnt.reserve(nums.size() * 2 + 1);
    for (int x : nums) cnt[x]++;

    const size_t n = nums.size();
    vector<vector<int>> buckets(n + 1);
    buckets.shrink_to_fit();
    for (const auto& p : cnt) {
        int val = p.first;
        int f = p.second; // 出现频次
        buckets[f].push_back(val);
    }

    vector<int> ans;
    ans.reserve(static_cast<size_t>(k));
    for (int f = static_cast<int>(n); f >= 1 && static_cast<int>(ans.size()) < k; --f) {
        if (buckets[f].empty()) continue;
        for (int v : buckets[f]) {
            ans.push_back(v);
            if (static_cast<int>(ans.size()) == k) break;
        }
    }
    return ans;
}

// ========== 测试与展示 ==========
int main() {
    cout << "Lab13-02 前K个高频元素" << endl;

    // 示例1：nums = [1,1,1,2,2,3], k = 2 -> [1,2]（顺序任意）
    {
        vector<int> nums = {1,1,1,2,2,3};
        int k = 2;
        auto ans = topKFrequent(nums, k);
        cout << "Test 1: ";
        for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << (i + 1 == ans.size() ? '\n' : ' ');
    }

    // 示例2：nums = [1], k = 1 -> [1]
    {
        vector<int> nums = {1};
        int k = 1;
        auto ans = topKFrequent(nums, k);
        cout << "Test 2: ";
        for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << (i + 1 == ans.size() ? '\n' : ' ');
    }

    // 额外测试1：包含负数
    {
        vector<int> nums = {-1,-1,-2,-2,-2,3}; // 频次：-2:3, -1:2, 3:1
        int k = 2;
        auto ans = topKFrequent(nums, k);
        cout << "Test 3: ";
        for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << (i + 1 == ans.size() ? '\n' : ' ');
    }

    // 额外测试2：k 等于不同元素个数
    {
        vector<int> nums = {4,4,6,6,7}; // 去重 {4,6,7}
        int k = 3;
        auto ans = topKFrequent(nums, k);
        cout << "Test 4: ";
        for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << (i + 1 == ans.size() ? '\n' : ' ');
    }

    // 额外测试3：高频元素唯一
    {
        vector<int> nums = {1,2,2,3,3,3,4,4,4,4}; // 4 的频次最高
        int k = 1;
        auto ans = topKFrequent(nums, k);
        cout << "Test 5: ";
        for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << (i + 1 == ans.size() ? '\n' : ' ');
    }

    // 额外测试4：多个元素频率并列
    {
        vector<int> nums = {5,5,6,6,7,7,8}; // 5/6/7 频次相同
        int k = 2;
        auto ans = topKFrequent(nums, k);
        cout << "Test 6: ";
        for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << (i + 1 == ans.size() ? '\n' : ' ');
    }

    return 0;
}
