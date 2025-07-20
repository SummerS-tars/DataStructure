#include <iostream>
#include <algorithm>

using namespace std;

const int N = 1e5 + 10;
int h[N];
int sz;

void up(int x){
    while(x / 2 && h[x / 2] > h[x]){
        swap(h[x / 2], h[x]);
        x /= 2;
    }
}

void down(int x){
    int t = x;
    if(x * 2 <= sz && h[x * 2] < h[t]) t = x * 2; // * 左子节点
    if(x * 2 + 1 <= sz && h[x * 2 + 1] < h[t]) t = x * 2 + 1; // * 右子节点
    if(t != x){
        swap(h[t], h[x]);
        down(t);
    }
}

void heapSort(){
    // * 此建堆方法可以实现O(n)的时间复杂度
    for(int i = sz / 2; i; i --) down(i);
}

int main(){
    int n, m;
    scanf("%d%d", &n, &m);

    for(int i = 1; i <= n; i ++) scanf("%d", h + i);
    sz = n;

    heapSort();

    while(m --){
        printf("%d ", h[1]); // * 输出堆顶元素
        h[1] = h[sz];
        sz --;
        down(1);
    }

    return 0;
}