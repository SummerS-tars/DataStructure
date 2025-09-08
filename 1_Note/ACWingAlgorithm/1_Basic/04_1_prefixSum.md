# 前缀和

作用：  
快速求出原始数组中某一段的和  
*空间换时间，将O(n)降低为O(1)*  

原理：  
类似于数列和  
通过 $S_r - S_{l - 1}$ 得到 $a_l + a_{l + 1} + ... + a_r$  

实现：  
一般定义 $S_0 = 0$  
然后递推 $S_i = S_{i - 1} + a_i$  
*定义 $S_0 = 0$ 方便统一从头开始的情况*  

## 1. 一维前缀和

```cpp
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
```

## 2. 二维前缀和

用到基础的填补思想  

```cpp
int main() {
    int n, m, q;
    std::cin >> n >> m >> q; // * q 为询问次数

    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++){
            scanf("%d", arr[i] + j);
        }
    }

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
```
