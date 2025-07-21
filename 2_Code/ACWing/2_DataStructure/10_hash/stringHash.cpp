// * 字符串哈希

// * 用于快速比较两个字符串是否完全相同

#include <iostream>

using namespace std;

typedef unsigned long long ULL;

const int N = 1e5 + 10;
const int P = 131;

int n, m;
char str[N];
ULL h[N], p[N]; // 注意此处即使P的高次太大，超过ULL，但是相当于自动模了2^64，符合我们取模的初衷

ULL get_sub_hash(int l, int r){
    return h[r] - h[l - 1] * p[r - l + 1];
}

int main(){
    scanf("%d%d", &n, &m);

    scanf("%s", str + 1);
    

    p[0] = 1;
    for(int i = 1; i <= n; i ++){
        p[i] = p[i - 1] * P;
        h[i] = h[i - 1] * P + str[i];
    }

    while(m --){
        int l1, r1, l2, r2;
        scanf("%d%d%d%d", &l1, &r1, &l2, &r2);
        if(get_sub_hash(l1, r1) == get_sub_hash(l2, r2)) puts("Yes");
        else puts("No");
    }

    return 0;
}