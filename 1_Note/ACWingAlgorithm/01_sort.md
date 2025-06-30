# 排序算法

1. 快速排序
2. 归并排序

总体思想：分治  

运用技术：递归  

重点：确定分界点  

快速排序非稳定排序  
归并排序是稳定排序  

## 快速排序

选定某个值，将数组分为两部分，一部份小于该值，一部份大于该值  

通过递归，最终实现排序  

难点：可能的边界问题

### 模板

```cpp
void quick_sort(int q[], int l, int r) {
    if(l >= r) return;

    int x = q[l + r + 1 >> 1]; // 选定分界点（实际上可以任意选择，只要保证最后分界不出问题即可）
    int i = l - 1, j = r + 1;
    while(i < j) {  // 双指针算法，i和j分别从两端向中间移动，通过交换实现分界点，直到相遇
        do i++; while(q[i] < x);
        do j--; while(q[j] > x);
        if(i < j) swap(q[i], q[j]);
    }

    quick_sort(q, l, j);
    quick_sort(q, j + 1, r);
}
```

### 例题

[785. 快速排序](https://www.acwing.com/problem/content/787/)

```cpp

```