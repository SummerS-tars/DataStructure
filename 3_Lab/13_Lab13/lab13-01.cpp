#include <iostream>
#include <vector>

using namespace std;

/*
 * 题目：逆序对的个数
 * 定义：数列中下标 i < j 且 a[i] > a[j] 的二元组 (i, j) 称为一个逆序对。
 * 目标：给定整数数组，返回逆序对总数。
 *
 * 核心思路（归并排序统计，O(n log n)）：
 * - 借助归并排序的“分而治之”，在合并两个有序子数组时统计跨越左右子数组的逆序对数量。
 * - 当右侧当前元素小于左侧当前元素时，左侧当前及其后面的所有剩余元素都与该右侧元素构成逆序对。
 * - 这样每个元素参与比较与移动的次数是受控的，总体复杂度 O(n log n)，远优于 O(n^2)。
 *
 * 正确性要点：
 * - 只有当 leftVal > rightVal 时才构成逆序对；等于时不计数（处理重复元素时尤为重要）。
 * - 统计跨区间的逆序对数量时，增量为 (mid - i + 1)。
 *
 * 复杂度：
 * - 时间：O(n log n)
 * - 空间：O(n)（临时辅助数组）
 *
 * 边界与细节：
 * - 空数组或单元素数组逆序对数为 0。
 * - 结果可能很大，返回值使用 long long。
 */

static long long mergeAndCount(vector<int>& a, vector<int>& tmp, int l, int mid, int r) {
    long long inv = 0;
    int i = l, j = mid + 1, k = l;

    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) {
            // 不构成逆序对（包含等于的情况）
            tmp[k++] = a[i++];
        } else {
            // a[i] > a[j] 构成逆序对，且 i..mid 都 > a[j]
            inv += (mid - i + 1);
            tmp[k++] = a[j++];
        }
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r)   tmp[k++] = a[j++];

    for (int t = l; t <= r; ++t) a[t] = tmp[t];
    return inv;
}

static long long sortAndCount(vector<int>& a, vector<int>& tmp, int l, int r) {
    if (l >= r) return 0;
    int mid = l + (r - l) / 2;
    long long left = sortAndCount(a, tmp, l, mid);
    long long right = sortAndCount(a, tmp, mid + 1, r);
    long long cross = mergeAndCount(a, tmp, l, mid, r);
    return left + right + cross;
}

long long countInversions(vector<int> a) {
    if (a.size() < 2) return 0LL;
    vector<int> tmp(a.size());
    return sortAndCount(a, tmp, 0, static_cast<int>(a.size()) - 1);
}

// ========== 测试与展示 ==========
int main() {
    cout << "Lab13-01 逆序对计数" << endl;

    // 示例1：2 3 4 5 6 1 -> 5
    {
        vector<int> a = {2, 3, 4, 5, 6, 1};
        long long ans = countInversions(a);
        cout << "Test 1: expected 5, got " << ans << endl;
    }

    // 示例2：9 7 5 4 6 -> 8
    {
        vector<int> a = {9, 7, 5, 4, 6};
        long long ans = countInversions(a);
        cout << "Test 2: expected 8, got " << ans << endl;
    }

    // 额外测试1：已排序数组 -> 0
    {
        vector<int> a = {1, 2, 3, 4, 5};
        long long ans = countInversions(a);
        cout << "Test 3: expected 0, got " << ans << endl;
    }

    // 额外测试2：完全逆序 -> n*(n-1)/2 = 10
    {
        vector<int> a = {5, 4, 3, 2, 1};
        long long ans = countInversions(a);
        cout << "Test 4: expected 10, got " << ans << endl;
    }

    // 额外测试3：含重复元素，等值不计入逆序
    {
        vector<int> a = {1, 3, 2, 3, 1}; // 手算为 4
        long long ans = countInversions(a);
        cout << "Test 5: expected 4, got " << ans << endl;
    }

    // 额外测试4：全部相等 -> 0
    {
        vector<int> a = {7, 7, 7, 7};
        long long ans = countInversions(a);
        cout << "Test 6: expected 0, got " << ans << endl;
    }

    return 0;
}
