// * 最大异或对

#include <iostream>
#include <algorithm>

using namespace std;

const int N = 1e5 + 10;
const int L = 31 * 1e5 + 10; // ? Q1: 1e5的数据，每个占31位，每位只能有两个元素，需要最多多少？
int arr[N];
int son[L][2], idx;

void insert(int x){
    int p = 0;
    for(int i = 30; i >= 0; i --){
        int bit = (x >> i) & 1;
        if(!son[p][bit]) son[p][bit] = ++idx;
        p = son[p][bit];
    } // * 此处不需要计数，知道有元素在即可
}

int find_max_xor(int x){
    int p = 0, res = 0;
    for(int i = 30; i >= 0; i --){
        // * 转换思路，对传入的x，寻找最大异或值，然后对所有数都做一次
        // * 对传入的每个数，判断次数是固定的常数，因此时间复杂度是O(n)
        int bit = (x >> i) & 1;
        if(son[p][!bit]){
            res |= (1 << i);
            p = son[p][!bit];
        }
        else p = son[p][bit];
    }
    return res;
}

int main(){
    int n;
    scanf("%d", &n);

    for(int i = 0; i < n; i ++){
        scanf("%d", arr + i);
        insert(arr[i]);
    }

    int max_xor_num=0;
    for(int i = 0; i < n; i ++){
        max_xor_num = max(max_xor_num, find_max_xor(arr[i]));
    }

    printf("%d\n", max_xor_num);

    return 0;
}