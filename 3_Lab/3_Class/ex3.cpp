#include <iostream>

using namespace std;

const int N = 1e5 + 10;
int arr[N];
int n, k;

void reverse(int arr[], int l, int r){
    for(int i = l, j = r; i < j; i ++, j --){
        swap(arr[i], arr[j]);
    }
}

/**
 ** reverse analysis
 **
 ** time complexity: O(l - r)
 ** space complexity: O(1)
 */

int main(){
    scanf("%d %d", &n, &k);
    k %= n;  // 防止k大于n

    for(int i = 0; i < n ; i ++){
        scanf("%d", &arr[i]);
    }

    reverse(arr, 0, n - 1);
    reverse(arr, 0, k - 1);
    reverse(arr, k, n - 1);

    /**
     ** time complexity: O(n)
     ** n - 1 + k - 1 + n - k - 1 = 2n - 3
     **
     ** space complexity: O(1)
     */

    for(int i = 0; i < n; i ++){
        printf("%d ", arr[i]);
    }
}