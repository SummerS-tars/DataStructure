// * 最长连续不重复子序列
// n长度整数数列，求最长的不包含重复数字的连续子序列，输出长度

#include <iostream>

const int N = 100010;
int arr[N], rec[N];

int main(){
    int n;
    scanf("%d", &n);

    for(int i = 0 ; i < n ; i ++)
        scanf("%d", arr + i);

    int len = 0;
    for(int i = 0, j = 0; i < n; i ++){
        rec[arr[i]] ++; // rec用于记录arr[i]出现的次数

        while(j < i && rec[arr[i]] > 1) { // 如果arr[i]出现次数大于1，则移动j，直到arr[i]出现次数为1
            rec[arr[j]] --; // 注意这里是arr[j]，而重复的是arr[i]
            j ++;
        }

        len = std::max(len, i - j + 1); // 更新len
    }

    printf("%d", len);

    return 0;
}