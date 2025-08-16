# Memory Limit Exceeded Example

```cpp
#include <iostream>

#define N 100010
int a[N];
int n, k;

void quick_sort(int arr[], int l , int r) {
    if(l >= r) return;
    
    int x = arr[(l+r) >> 1];
    int i = l - 1, j = r + 1;
    while(i <= j){ // ！ 这里不能取等号！！！
        do i ++; while(arr[i] < x);
        do j --; while(arr[j] > x);
        if(i < j) std::swap(arr[i], arr[j]);
    }
    
    quick_sort(arr, l, j);
    quick_sort(arr, j + 1, r);
}

int main(){
    scanf("%d%d", &n, &k);
    
    for(int i = 0 ; i < n ; i ++)
        scanf("%d" , a + i);
    
    quick_sort(a, 0, n - 1);
    
    printf("%d" , a[k - 1]);

    return 0;
}
```

correct

```cpp
#include <iostream>

#define N 100010
int n, k;
int q[N];

void quick_sort(int q[], int l, int r) {
    if(l >= r) return;

    int x = q[(l + r) >> 1]; // 选定分界点（实际上可以任意选择，只要保证最后分界不出问题即可）
    int i = l - 1, j = r + 1;
    while(i < j) {  // 双指针算法，i和j分别从两端向中间移动，通过交换实现分界点，直到相遇
        do i++; while(q[i] < x);
        do j--; while(q[j] > x);
        if(i < j) std::swap(q[i], q[j]); // 标准库函数中的swap传入的是引用，因此不是传地址
    }

    quick_sort(q, l, j);
    quick_sort(q, j + 1, r);
}

int main() {
    scanf("%d%d", &n, &k);
    for(int i = 0; i < n; i++) scanf("%d", q + i);

    quick_sort(q, 0, n - 1);

    printf("%d" , q[k -1]);
    return 0;
}
```

why the first code is wrong?
