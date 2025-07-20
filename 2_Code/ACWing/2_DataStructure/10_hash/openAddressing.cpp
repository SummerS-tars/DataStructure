// * 开放寻址法

// * 经验上来说，开放寻址法的使用的哈希表大小一般为题中元素数量的2到3倍

#include <cstring>
#include <iostream>

using namespace std;

const int N = 2e5 + 3, null = 0x3f3f3f3f;

int h[N];

/**
 * 如果存在，返回该元素的索引
 * 如果不存在，返回该元素应该插入的索引
 */
int find(int x){
    int k = (x % N + N) % N;
    while(h[k] != null && h[k] != x){
        k ++;
        if(k == N) k = 0;
    }
    return k;
}

void insert(int x){
    int k = find(x);
    h[k] = x;
}

int main(){
    int n;
    scanf("%d", &n);

    memset(h, 0x3f, sizeof h);

    while(n --){
        char op[2];
        int x;
        scanf("%s%d", op, &x);

        if(op[0] == 'I'){
            insert(x);
        }
        else{
            if(h[find(x)] == null) puts("No");
            else puts("Yes");
        }
    }
}