// * 滑动窗口

#include <iostream>

using namespace std;

const int N = 1e6 + 10;
int a[N], q[N]; 
// ! 队列中存储的是下标

int main(){
    int n, k;
    scanf("%d%d", &n, &k);
    for(int i = 0; i < n; i++) scanf("%d", &a[i]);

    int hh = 0, tt = -1;
    for(int i = 0; i < n; i++){
        // * 数值解析：
        // ! 请注意前提：队列中存储的是下标
        // * 1. 窗口头部 q[hh] 窗口尾部 i
        // * 2. 窗口范围 i-k+1 ~ i
        // * 3. q[tt]存储的不一定是窗口尾部的下标，而是窗口中最大值的下标

        // ! 注意此处队列默认是双端队列，可以同时从队头和队尾弹出元素
        if(hh <= tt && i - k + 1 > q[hh]) hh++; // * 如果窗口头部超出范围，则弹出
        while(hh <= tt && a[q[tt]] >= a[i]) tt --; // * 如果队尾元素大于或等于当前元素，则弹出，直到队尾元素小于它或队列为空

        q[++tt] = i; // * 将当前元素下标入队
        if(i >= k - 1) printf("%d ", a[q[hh]]); // * 如果窗口长度达到k，则开始输出窗口最小值
    }

    puts("");

    hh = 0, tt = -1;
    for(int i = 0; i < n; i ++){
        if(hh <= tt && i - k + 1 > q[hh]) hh ++;
        while(hh <= tt && a[q[tt]] <= a[i]) tt --;

        q[++tt] = i;
        if(i >= k - 1) printf("%d ", a[q[hh]]);
    }

    return 0;
}