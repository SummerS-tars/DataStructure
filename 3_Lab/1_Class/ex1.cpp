#include <iostream>

using namespace std;

const int N = 1e5 + 10;
int arr[N];
int n, k;

void quick_sort(int arr[], int l, int r){
    if(l >= r) return;
    
    int x = arr[(l+r) >> 1], i = l - 1, j = r + 1;
    while(i < j){
        do i ++; while(arr[i] < x);
        do j --; while(arr[j] > x);
        if(i < j) swap(arr[i], arr[j]);
    }
    quick_sort(arr, l, j);
    quick_sort(arr, j + 1, r);
}

/**
 ** quick sort
 ** 
 ** time complexity: O(nlogn)
 ** we can suppose that everage depth of recursion is k
 ** than 2^k = n => k = logn
 ** every time we need to traverse all the elements => O(n)
 ** so the time complexity is O(nlogn)
 **
 ** space complexity: O(logn)
 ** according to the depth of recursion
 ** every time we only need l, r, x, i, j => O(1)
 ** so the space complexity is O(logn)
 */

int main() {
    cin >> n;
    for(int i = 0; i < n ; i ++){
        cin >> arr[i];
    }
    quick_sort(arr, 0, n - 1);

    for(int i = 0; i < n; i ++){
        cout << arr[i] << ' ';
    }
    cout << endl;

    k ++;
    for(int i = 1; i < n; i ++){
        if(arr[i] != arr[i - 1]) k ++;
    }
    cout << "number of different elements : " << k << endl;

    /**
     ** number kinds statistics
     ** 
     ** time complexity: O(n)
     ** space complexity: O(1)
     */

    return 0;
}

/**
 * input: input.txt
 * output:
 * 15 20 20 32 40 40 67 89 300 400 
 * The number of different elements is 8
 */