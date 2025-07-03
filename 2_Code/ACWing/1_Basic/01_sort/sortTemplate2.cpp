#include <iostream>

#define MAX_SIZE 10000
int tmp[MAX_SIZE];

void merge_sort(int q[], int l, int r) {
    if(l >= r) return;
    
    /**
     ** 采用双指针算法，先递，将数组切片
     ** 归并，双指针比较，将数组依序先写入临时数组
     ** 将临时数组写回原数组
    */
    int mid = (l + r) >> 1;
    merge_sort(q, l, mid);
    merge_sort(q, mid + 1, r);

    int k = 0, i = l, j = mid + 1;
    while(i <= mid && j <= r) {
        if(q[i] <= q[j]) tmp[k++] = q[i++];
        else tmp[k++] = q[j++];
    }

    // * 将剩余的元素写入临时数组
    while(i <= mid) tmp[k++] = q[i++];
    while(j <= r) tmp[k++] = q[j++];

    for(int i = l, j = 0; i <= r; i++, j++) q[i] = tmp[j];
}

int main() {
    int n;
    std::cin >> n;
    int q[n];
    for(int i = 0; i < n; i++) std::cin >> q[i];

    merge_sort(q, 0, n - 1);

    for(int i = 0; i < n; i++) std::cout << q[i] << " ";
    return 0;
}