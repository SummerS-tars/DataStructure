#include <iostream>

using namespace std;

const int N = 100010;

int arr[N], dif[N];

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
        dif[l - 1] += c;
        dif[r] -= c;
    }
    
    for(int i = 0; i < n; i ++){
        arr[i] = (i == 0) ? dif[i] : (arr[i - 1] + dif[i]);
        printf("%d ", arr[i]) ;
    }
    
    return 0;
}