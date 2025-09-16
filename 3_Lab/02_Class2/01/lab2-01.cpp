#include <cstdio>
#include <iostream>
using namespace std;

#define ERROR -1
#define defaultSize 10010

template <class Type>
class SeqList
{
public:
    SeqList(int size = defaultSize); // 构造函数
    ~SeqList() { delete[] list; }    // 析构函数
    void PrintAll();
    int Length() const { return last + 1; }
    int IsEmpty() { return last == -1; }
    int IsFull() { return last == maxSize - 1; }
    Type Get(int i);

    int Find(Type x) const;
    int Insert(Type x, int i);
    int Remove(int i);
    int RemoveBetween(Type x, Type y);

private:
    Type *list;
    int maxSize;
    int last; // 注意，此处last指的是最后一个元素的下标
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

/**
 ** time complexity: O(n)
 **
 ** space complexity: O(1)
 **
 */
template <class Type>
int SeqList<Type>::Find(Type x) const
{
    for (int i = 0; i <= last; i ++)
    {
        if(list[i] == x)
            return i;
    }
    return -1;
}

/**
 ** time complexity: O(n)
 ** the function will average take n/2 steps
 ** space complexity: O(1)
 ** only constant extra space is needed(i, j, e.t.c)
 */
template <class Type>
int SeqList<Type>::Insert(Type x, int i)
{
    if(IsFull())
    {
        cout << "FULL ";
        return -1;
    }
    
    if(i < 0 || i > last + 1)
    {
        cout << "ILLEGAL POSITION ";
        return -1;
    }

    for(int j = last+1; j > i; j --)
        list[j] = list[j - 1];
    list[i] = x;
    last ++;
    return 0;
}

/**
 ** time complexity: O(n)
 ** also will average take n/2 steps
 ** space complexity: O(1)
 */
template <class Type>
int SeqList<Type>::Remove(int i)
{
    if(i < 0 || i > last)
    {
        cout << "POSITION " << i << " EMPTY ";
        return -1;
    }
    
    for(int j = i; j < last; j ++)
        list[j] = list[j + 1];
    last --;
    return 0;
}

/**
 ** Implementation 1
 ** time complexity: O(n^2)
 ** it's about the range between x and y and the distribution of elements
 ** we need to traverse the whole list
 ** we assume that there are a*n elements between x and y
 ** then we will take n + a*n^2/2 steps
 ** the a in average case can be seen as a constant
 ** so the time complexity is O(n^2)
 ** 
 ** space complexity: O(1)
 */
// template <class Type>
// int SeqList<Type>::RemoveBetween(Type x, Type y)
// {
//     int cnt = 0;
//     for(int i = 0; i <= last; i ++)
//     {
//         if(list[i] >= x && list[i] <= y)
//         {
//             cnt ++;
//             Remove(i);
//             i --;
//         }
//     }
//     return cnt;
// }

/**
 ** Implementation 2
 **
 ** time complexity: O(n)
 ** we only traverse the list once
 **
 ** space complexity: O(1)
 **
 */
template <class Type>
int SeqList<Type>::RemoveBetween(Type x, Type y)
{
    int j = 0;
    int cnt = 0;
    for(int i = 0; i <= last; i ++)
    {
        if(list[i] >= x && list[i] <= y)
        {
            cnt ++;
            continue;
        }
        else
        {
            list[j++] = list[i];
        }
    }
    last = j - 1;
    return cnt;
}

int main()
{
    SeqList<int> a(5);
    int n;
    cin >> n; // 输入插入长度
    for (int i = 0; i < n; i++)
    { // 逐个输入顺序表中的值
        int temp;
        cin >> temp;
        if (a.Insert(temp, i) == -1)
            cout << "Insertion Error: " << temp << " is not in." << endl;
    }

    cin >> n; // 输入查找长度
    for (int i = 0; i < n; i++)
    {
        int temp, p;
        cin >> temp;
        p = a.Find(temp);
        if (p == -1)
            cout << "Finding Error: " << temp << " is not in." << endl;
        else
            cout << temp << " is at position " << p << "." << endl;
    }

    cin >> n; // 输入删除长度
    for (int i = 0; i < n; i++)
    {
        int temp, p;
        cin >> temp;
        if (a.Remove(temp) == -1)
            printf("Deletion Error.\n");
    }

    int x, y;
    cin >> x;
    cin >> y;
    int k = a.RemoveBetween(x, y);
    a.PrintAll();
    cout << k << endl;
    return 0;
}