#include <iostream>

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
    int n;
    std::cin >> n;
    int q[n];
    for(int i = 0; i < n; i++) std::cin >> q[i];

    quick_sort(q, 0, n - 1);

    for(int i = 0; i < n; i++) std::cout << q[i] << " ";
    return 0;
}