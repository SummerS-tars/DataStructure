#include <cstdio>
#include <iostream>
#include <assert.h>

using namespace std;

const int defaultSize = 1010;

template <class Type>
class SeqList
{
public:
    SeqList(int size = defaultSize);
    ~SeqList() { delete[] list; }
    void PrintAll();
    int Length() const { return last + 1; }
    int Find(Type x) const;
    int Insert(Type x, int i);
    int Remove(int i);
    int removeDuplicates();
    int IsEmpty() { return last == -1; }
    int IsFull() { return last == maxSize - 1; }
    Type Get(int i);

private:
    Type *list;
    int maxSize;
    int last;
};

template <class Type>
SeqList<Type>::SeqList(int size)
{
    if (size > 0)
    {
        maxSize = size;
        last = -1;
        list = new Type[maxSize];
        if (list == NULL)
        {
            cerr << " Memory Allocation Error " << endl;
            return;
        }
    }
}

template <class Type>
Type SeqList<Type>::Get(int i)
{
    if (i < 0 || i > last)
    {
        throw std::out_of_range("Index out of bounds");
    }
    return list[i];
}

template <class Type>
void SeqList<Type>::PrintAll()
{
    for (int i = 0; i < last; i++)
        cout << list[i] << ' ';
    cout << list[last] << endl;
}

template <class Type>
int SeqList<Type>::Find(Type x) const
{
    for (int i = 0; i <= last; i++)
        if (list[i] == x)
            return i;
    return -1;
}

template <class Type>
int SeqList<Type>::Insert(Type x, int i)
{
    if (i < 0 || i > last + 1)
    {
        cout << "ILLEGAL POSITION ";
        return -1;
    }
    if (last == maxSize - 1)
    {
        cout << "Full ";
        return -1;
    }
    last++;
    for (int j = last; j > i; j--)
        list[j] = list[j - 1];
    list[i] = x;
    return 0;
}

template <class Type>
int SeqList<Type>::Remove(int i)
{
    if (i < 0 || i > last)
        return 1;
    for (int j = i; j < last; j++)
        list[j] = list[j + 1];
    last--;
    return 0;
}

/**
 ** time complexity: O(n)
 **
 ** space complexity: O(1)
 **
 */
template <class Type>
int SeqList<Type>::removeDuplicates()
{
    int k = 0; // 记录不重复元素的个数
    if(last > -1) // 非空表
    {
        k = 1;
        int tmp;
        for(int i = 1, j = 1; i <= last; i ++)
        {
            if(list[i] != list[i-1])
            {
                list[j] = list[i]; // 保留不重复元素
                j ++;
                k ++;
            }
        }
    }
    last = k - 1; // 更新表长
    return k;
}

int main()
{
    int arr[] = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};

    SeqList<int> a(sizeof(arr) / sizeof(arr[0]));
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        a.Insert(arr[i], i);

    int expectedNums[] = {0, 1, 2, 3, 4}; // 长度正确的期望答案

    int k = a.removeDuplicates(); // 调用
    cout << k << endl;

    assert(k == sizeof(expectedNums) / sizeof(expectedNums[0]));

    for (int i = 0; i < k; i++)
    {
        assert(a.Get(i) == expectedNums[i]);
    }

    a.PrintAll();

    int arr2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    SeqList<int> b(sizeof(arr2) / sizeof(arr2[0]));
    for (int i = 0; i < sizeof(arr2) / sizeof(arr2[0]); i++)
        b.Insert(arr2[i], i);
    int expectedNums2[] = {1}; // 长度正确的期望答案
    k = b.removeDuplicates(); // 调用
    cout << k << endl;
    assert(k == sizeof(expectedNums2) / sizeof(expectedNums2[0]));
    for (int i = 0; i < k; i++)
    {
        assert(b.Get(i) == expectedNums2[i]);
    }
    b.PrintAll();

    int arr3[] = {1, 3, 5, 7, 7, 7, 7, 7, 7, 9, 11, 11, 13, 15, 15, 15, 17};
    SeqList<int> c(sizeof(arr3) / sizeof(arr3[0]));
    for (int i = 0; i < sizeof(arr3) / sizeof(arr3[0]); i++)
        c.Insert(arr3[i], i);
    int expectedNums3[] = {1, 3, 5, 7, 9, 11, 13, 15, 17}; // 长度正确的期望答案
    k = c.removeDuplicates(); // 调用
    cout << k << endl;
    assert(k == sizeof(expectedNums3) / sizeof(expectedNums3[0]));
    for (int i = 0; i < k; i++)
    {
        assert(c.Get(i) == expectedNums3[i]);
    }
    c.PrintAll();

    return 0;
}