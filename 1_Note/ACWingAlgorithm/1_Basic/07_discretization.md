# 离散化

将分布范围广，但数量少的一个线性表的每个元素  
映射到从0开始的连续的整数  
*原数组此处要求是单调递增的（且无重复元素）*  

需求：  

1. 原线性表中可能出现重复元素，需要去重  
2. 如何算出原线性表的a\[i\]的离散化后的值  

## 1. 模版

常用排序及去重写法：  

```cpp
vector<int> alls;
sort(alls.begin(), alls.end());
alls.erase(unique(alls.begin(), alls.end()), alls.end());
```

以上写法是STL中sort和unique的结合，可以实现对vector的排序和去重  

查找：  
使用二分查找  

```cpp
int find(int x){
    int l = 0, r = alls.size() - 1;
    while(l < r){
        int mid = (l + r) >> 1;
        if(alls[mid] >= x) r = mid;
        else l = mid + 1;
    }
    return l + 1;
}
```

以上函数会返回在alls中第一个大于等于x的元素的下标  

## 2. 用法

区间和  

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<int, int> PII;

const int N = 300010;
int arr[N], sum[N]; // arr[i]存储的是离散化数组中vector[i]所对应的原数组位置x的最后积累的值，sum则是arr的前缀和数组
vector<int> alls; // alls存储所有的位置（包含插入值的位置，也包含查询的位置），不含重复元素
vector<PII> add, query; // add存储原始插入位置以及值的对，query存储查询的左右端点

int find(int x){
    int l = 0, r = alls.size() - 1;
    while(l < r){
        int mid = (l + r) >> 1;
        if(alls[mid] >= x) r = mid;
        else l = mid + 1;
    }
    return l + 1; // 因为需要用到前缀和，所以是映射到从1开始的连续整数  
}

int main(){
    int n, m;
    cin >> n >> m;

    for(int i = 0;i < n;i ++){
        int x, c;
        scanf("%d%d", &x, &c);
        add.push_back({x, c}); // 原始数组
        alls.push_back(x); // alls记录下标
    }

    for(int i = 0;i < m;i ++){
        int l, r;
        scanf("%d%d", &l, &r);
        query.push_back({l, r});
        alls.push_back(l);
        alls.push_back(r);  // alls也记录需要查询的下标
    }

    sort(alls.begin(), alls.end());
    alls.erase(unique(alls.begin(), alls.end()), alls.end());

    // * 处理插入
    for(auto item : add){
        int x = find(item.first); // 将原始数组下标对应的值累加到离散化后对应的位置上
        arr[x] += item.second;
    }

    // * 生成前缀和
    for(int i = 1; i <= alls.size(); i ++){
        sum[i] = sum[i -1] + arr[i];
    }

    for(auto item : query){
        int l = find(item.first), r = find(item.second);
        printf("%d\n", sum[r] - sum[l - 1]);
    }

    return 0;
}
```
