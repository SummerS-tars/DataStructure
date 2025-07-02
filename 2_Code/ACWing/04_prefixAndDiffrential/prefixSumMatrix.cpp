#include <iostream>

const int N = 1010;

int arr[N][N], sum[N][N];

int main() {
    int n, m, q;
    std::cin >> n >> m >> q; // * q 为询问次数

    // printf("n: %d, m: %d, q: %d\n", n, m, q);
    
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++){
            scanf("%d", arr[i] + j);
        }
    }

    // for(int i = 0; i < n; i ++){
    //     for(int j = 0; j < m; j ++){
    //         printf("%d ", arr[i][j]);
    //     }
    //     printf("\n");
    // }

    // * 此处 i，j 也是从1开始（数组从0）开始
    for(int i = 1; i <= n; i ++){
        for(int j = 1; j <= m; j ++){
            sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] + arr[i - 1][j - 1];
        }
    }

    while(q --){
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        printf("%d\n", sum[x2][y2] - sum[x1 - 1][y2] - sum[x2][y1 - 1] + sum[x1 - 1][y1 - 1]);
    }

    return 0;
}