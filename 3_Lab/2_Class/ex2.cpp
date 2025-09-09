#include <iostream>

using namespace std;

const int N = 1e4 + 10, M = 1e5 + 10;
int arr[N], num[M];
int n;

int main(){
    scanf("%d", &n);
    for(int i = 0; i < n; i ++){
        scanf("%d", arr+i);
    }

    int maxt = 0;
    int maxn = -1;
    for(int i = 0; i < n; i ++){
        num[arr[i]] ++;
        if(num[arr[i]] > maxt){
            maxt = num[arr[i]];
            maxn = arr[i];
        }
    }
    
    /**
     ** Time complexity: O(n)
     ** need to traverse all the elements one time
     **
     ** Space complexity: O(k)
     ** k is about the range of numbers in the list
     */

    printf("The number that appears most : %d\nappears times: %d\n", maxn, maxt);

    return 0;
}