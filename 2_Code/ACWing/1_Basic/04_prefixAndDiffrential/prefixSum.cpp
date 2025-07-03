#include <iostream>

const int N = 100010;

int arr[N], sum[N]; 

// * sum[i] 存储前i个数个数的和
// * 因此，如果要求第 l ~ r 个数的和，使用 sum[r] - sum[l - 1] 即可
// * 此处 l 从 1 开始（数组依然是从0开始存储的）

int main() {
    int n, m;
    std::cin >> n >> m;

    for(int i = 1; i <= n; i ++) {
        scanf("%d", arr + i - 1);
        sum[i] = sum[i - 1] + arr[i - 1];
    }

    while(m --) {
        int l, r;
        std::cin >> l >> r;
        std::cout << sum[r] - sum[l - 1] << std::endl;
    }
}