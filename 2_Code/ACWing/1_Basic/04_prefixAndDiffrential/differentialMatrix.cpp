#include <iostream>

using namespace std;

const int N = 1010;

int arr[N][N], dif[N][N];

int main(){
    int n, m, q;
    cin >> n >> m >> q;
    
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++){
            scanf("%d", arr[i] + j);
        }
    }
    
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
        scanf("%d%d%d%d%d", &x1, &y1, &x2, &y2, &c); // 其实如果太靠近边界可能会出现越界无效，但是没影响
        dif[x1 - 1][y1 - 1] += c;
        dif[x1 - 1][y2] -= c;
        dif[x2][y1 - 1] -= c;
        dif[x2][y2] += c;
    }
    
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