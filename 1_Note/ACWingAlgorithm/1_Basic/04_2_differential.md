# 差分

定义：  
对于数组 $a_1, a_2, ..., a_n$  
构造差分数组 $b_1, b_2, ..., b_n$  
使得 $a_i = b_1 + b_2 + ... + b_i$  

差分数组满足：  
$b_i = a_i - a_{i - 1}$  

理解：  
可以认为是前缀和的逆运算  
不难发现，差分数组的前缀和就是原数组  

作用：  
用于对原数组进行大量成段加减操作的时间复杂度简化  
*即对某一段数组加上或减去同一个数*  

## 一维差分

```cpp
int main(){
    int n, m;
    cin >> n >> m;
    
    for(int i = 0; i < n; i ++){
        scanf("%d", arr + i);
        dif[i] = (i == 0) ? arr[i] : arr[i] - arr[i - 1];
    }
    
    while(m --){
        int l, r, c;
        scanf("%d%d%d", &l, &r, &c);
        dif[l - 1] += c; // l到r之间元素增加
        dif[r] -= c; // r之后恢复
    }

    for(int i = 0; i < n; i ++){
        arr[i] = (i == 0) ? dif[i] : (arr[i - 1] + dif[i]);
        printf("%d ", arr[i]) ;
    } // 通过差分数组dif恢复arr原数组
    
    return 0;
}
```

## 二维差分

```cpp
int main(){
    int n, m, q;
    cin >> n >> m >> q;
    
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++){
            scanf("%d", arr[i] + j);
        }
    }
    
    /* 构造差分数组（实际上就是二维前缀和的逆运算） */
    dif[0][0] = arr[0][0];
    for(int i = 1; i < n; i ++)
        dif[i][0] = arr[i][0] - arr[i - 1][0];
    for(int j = 1; j < m; j ++)
        dif[0][j] = arr[0][j] - arr[0][j - 1];
    for(int i = 1; i < n; i ++){
        for(int j = 1; j < m; j ++){
            dif[i][j] = arr[i][j] - arr[i-1][j] - arr[i][j-1] + arr[i-1][j-1];
        }
    }
    
    while(q --){
        int x1, y1, x2, y2, c;
        scanf("%d%d%d%d%d", &x1, &y1, &x2, &y2, &c); // 其实如果太靠近边界可能会出现越界无效，但是没影响（超出范围的dif中的元素，在恢复原数组时没有影响）
        dif[x1 - 1][y1 - 1] += c;
        dif[x1 - 1][y2] -= c;
        dif[x2][y1 - 1] -= c;
        dif[x2][y2] += c;
    }

    /* 从差分数组恢复原数组，实际上就是求差分数组的前缀和 */
    arr[0][0] = dif[0][0];
    for(int i = 1; i < n; i ++)
        arr[i][0] = arr[i - 1][0] + dif[i][0];
    for(int j = 1; j < m; j ++)
        arr[0][j] = arr[0][j - 1] + dif[0][j];
    for(int i = 1; i < n; i ++){
        for(int j = 1; j < m; j ++){
            arr[i][j] = dif[i][j] - arr[i-1][j-1] + arr[i-1][j] + arr[i][j-1] ;
        }
    }

    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++){
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }

    return 0;
}
```
