#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Solution
{
private:
    // 快速选择算法 - 方法1：使用分区思想（平均 O(n)）
    // 交换两个元素
    void swap(vector<int>& nums, int i, int j)
    {
        int temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }
    
    // 分区函数：将数组分为两部分，左边都大于等于pivot，右边都小于pivot
    // 返回pivot的最终位置
    int partition(vector<int>& nums, int left, int right)
    {
        // 随机选择一个pivot以优化性能
        int randomIndex = left + rand() % (right - left + 1);
        swap(nums, randomIndex, right);
        
        int pivot = nums[right];
        int i = left;
        
        // 将大于等于pivot的元素放到左边
        for (int j = left; j < right; j++)
        {
            if (nums[j] >= pivot)
            {
                swap(nums, i, j);
                i++;
            }
        }
        
        // 将pivot放到正确的位置
        swap(nums, i, right);
        return i;
    }
    
    // 快速选择算法的核心递归函数
    int quickSelect(vector<int>& nums, int left, int right, int k)
    {
        if (left == right)
        {
            return nums[left];
        }
        
        // 进行分区
        int pivotIndex = partition(nums, left, right);
        
        // pivotIndex 表示当前元素是第几大的（从0开始计数）
        if (pivotIndex == k - 1)
        {
            // 找到了第k大的元素
            return nums[pivotIndex];
        }
        else if (pivotIndex > k - 1)
        {
            // 第k大的元素在左半部分
            return quickSelect(nums, left, pivotIndex - 1, k);
        }
        else
        {
            // 第k大的元素在右半部分
            return quickSelect(nums, pivotIndex + 1, right, k);
        }
    }
    
public:
    int findKthLargest(vector<int> &nums, int k)
    {
        // 初始化随机数种子
        srand(time(0));
        
        // 使用快速选择算法
        return quickSelect(nums, 0, nums.size() - 1, k);
    }
};

// 方法2：使用小顶堆实现（O(nlogk)）- 备选方案
class SolutionWithHeap
{
private:
    // 小顶堆的实现
    vector<int> heap;
    int heapSize;
    
    // 获取父节点索引
    int parent(int i) { return (i - 1) / 2; }
    
    // 获取左子节点索引
    int leftChild(int i) { return 2 * i + 1; }
    
    // 获取右子节点索引
    int rightChild(int i) { return 2 * i + 2; }
    
    // 交换两个元素
    void swap(int i, int j)
    {
        int temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }
    
    // 向上调整（用于插入）
    void heapifyUp(int i)
    {
        while (i > 0 && heap[parent(i)] > heap[i])
        {
            swap(i, parent(i));
            i = parent(i);
        }
    }
    
    // 向下调整（用于删除）
    void heapifyDown(int i)
    {
        int minIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);
        
        if (left < heapSize && heap[left] < heap[minIndex])
        {
            minIndex = left;
        }
        
        if (right < heapSize && heap[right] < heap[minIndex])
        {
            minIndex = right;
        }
        
        if (i != minIndex)
        {
            swap(i, minIndex);
            heapifyDown(minIndex);
        }
    }
    
    // 插入元素
    void insert(int val)
    {
        heap.push_back(val);
        heapSize++;
        heapifyUp(heapSize - 1);
    }
    
    // 删除堆顶元素
    void removeMin()
    {
        if (heapSize == 0) return;
        
        heap[0] = heap[heapSize - 1];
        heap.pop_back();
        heapSize--;
        
        if (heapSize > 0)
        {
            heapifyDown(0);
        }
    }
    
    // 获取堆顶元素
    int getMin()
    {
        return heap[0];
    }
    
public:
    int findKthLargest(vector<int> &nums, int k)
    {
        heap.clear();
        heapSize = 0;
        
        // 维护一个大小为k的小顶堆
        for (int i = 0; i < nums.size(); i++)
        {
            if (heapSize < k)
            {
                // 堆未满，直接插入
                insert(nums[i]);
            }
            else
            {
                // 堆已满，如果当前元素大于堆顶，则替换堆顶
                if (nums[i] > getMin())
                {
                    removeMin();
                    insert(nums[i]);
                }
            }
        }
        
        // 堆顶元素就是第k大的元素
        return getMin();
    }
};

int main()
{
    cout << "========== 使用快速选择算法（平均 O(n)）==========" << endl << endl;
    Solution s;
    
    // 测试用例1：Example1
    cout << "测试用例1：" << endl;
    vector<int> nums1 = {3, 2, 1, 5, 6, 4};
    int k1 = 2;
    cout << "输入：[3, 2, 1, 5, 6, 4], k = " << k1 << endl;
    cout << "输出：" << s.findKthLargest(nums1, k1) << endl;
    cout << "解释：排序后为 [6, 5, 4, 3, 2, 1]，第2大的元素是 5" << endl;
    cout << endl;
    
    // 测试用例2：Example2
    cout << "测试用例2：" << endl;
    vector<int> nums2 = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    int k2 = 4;
    cout << "输入：[3, 2, 3, 1, 2, 4, 5, 5, 6], k = " << k2 << endl;
    cout << "输出：" << s.findKthLargest(nums2, k2) << endl;
    cout << "解释：排序后为 [6, 5, 5, 4, 3, 3, 2, 2, 1]，第4大的元素是 4" << endl;
    cout << endl;
    
    // 测试用例3：找最大的元素
    cout << "测试用例3：" << endl;
    vector<int> nums3 = {7, 6, 5, 5, 4};
    int k3 = 1;
    cout << "输入：[7, 6, 5, 5, 4], k = " << k3 << endl;
    cout << "输出：" << s.findKthLargest(nums3, k3) << endl;
    cout << "解释：排序后为 [7, 6, 5, 5, 4]，第1大的元素是 7" << endl;
    cout << endl;
    
    cout << "========== 使用小顶堆实现（O(nlogk)）==========" << endl << endl;
    SolutionWithHeap sh;
    
    // 测试用例1
    cout << "测试用例1（堆实现）：" << endl;
    vector<int> nums4 = {3, 2, 1, 5, 6, 4};
    cout << "输入：[3, 2, 1, 5, 6, 4], k = 2" << endl;
    cout << "输出：" << sh.findKthLargest(nums4, 2) << endl;
    cout << endl;
    
    // 测试用例2
    cout << "测试用例2（堆实现）：" << endl;
    vector<int> nums5 = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    cout << "输入：[3, 2, 3, 1, 2, 4, 5, 5, 6], k = 4" << endl;
    cout << "输出：" << sh.findKthLargest(nums5, 4) << endl;
    cout << endl;
    
    // 测试用例3
    cout << "测试用例3（堆实现）：" << endl;
    vector<int> nums6 = {7, 6, 5, 5, 4};
    cout << "输入：[7, 6, 5, 5, 4], k = 1" << endl;
    cout << "输出：" << sh.findKthLargest(nums6, 1) << endl;
    cout << endl;

    return 0;
}
