# 二分查找

二分查找的重点在于找到某个性质用于检测判断  
使得检测区间可以划分为两个部分  
一侧满足，而另一测不满足  

注意点：  
用于数组，二分查找一般用于有序数组  
及利用单调性，将问题转化为二分查找问题  
但是需注意，二分查找的本质是检测性质，而不是单调性  

## 模板

Template 1

```cpp
int bsearch_1(int l, int r) {
    while(l < r) {
        int mid = (l + r) >> 1;
        if(check(mid)) r = mid; // * 左侧满足性质，右边界收缩
        else l = mid + 1;
    }
    return l;
}
```

Template 2

```cpp
int bsearch_2(int l, int r) {
    while(l < r) {
        int mid = (l + r + 1) >> 1;
        if(check(mid)) l = mid; // * 右侧满足性质，左边界收缩
        else r = mid - 1;
    }
    return l;
}
```

## 理解点

注意二分搜索一定会返回一个结果  
可能不是需要的结果（可能只是最接近的）  
