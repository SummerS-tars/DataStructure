#include <iostream>

using namespace std;

const int N = 100010;
int arr[N];

int lowbit(int x){
    return x & (-x);
}

int cnt_one(int x){
    int cnt = 0;

    while(x){
        cnt ++;
        int t = lowbit(x);
        x ^= t;
    }
    
    return cnt;
}

int main(){
    int n ;
    cin >> n;
    
    for(int i = 0; i < n;i ++){
        scanf("%d", arr + i);
    }
    
    for(int i = 0; i < n; i ++){
        printf("%d ", cnt_one(arr[i]));
    }
    
    return 0;
}