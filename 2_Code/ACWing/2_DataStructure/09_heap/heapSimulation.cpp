// * 模拟堆

#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 1e5 + 10;
int h[N], sz;
int ph[N], hp[N];
int idx = 1;

void heap_swap(int a, int b){
    swap(ph[hp[a]], ph[hp[b]]);
    swap(hp[a], hp[b]);
    swap(h[a], h[b]);
}

void up(int x){
    while(x / 2 && h[x / 2] > h[x]){
        heap_swap(x, x / 2);
        x /= 2;
    }
}

void down(int x){
    int t = x;
    if(x * 2 <= sz && h[x * 2] < h[t]) t = x * 2;
    if(x * 2 + 1 <= sz && h[x * 2 + 1] < h[t]) t = x * 2 + 1;
    if(t != x){
        heap_swap(t, x);
        down(t);
    }
}

void insert(int x){
    h[++ sz] = x;
    ph[idx] = sz;
    hp[sz] = idx;
    idx ++;
    up(sz);
}

void del(int x){
    heap_swap(x, sz);
    sz --;
    down(x);
    up(x);
}

void change_k(int k, int v){
    int x = ph[k];
    h[x] = v;
    down(x);
    up(x);
}

int main(){
    int n;
    scanf("%d", &n);

    while(n --){
        char op[10];
        int k, x;
        scanf("%s", op);
        if(!strcmp(op, "I")) {
            scanf("%d", &x);
            insert(x);
        }
        else if(!strcmp(op, "PM")) 
            printf("%d\n", h[1]);
        else if(!strcmp(op, "DM"))
            del(1);
        else if(!strcmp(op, "D")) {
            scanf("%d", &k);
            del(ph[k]);
        }
        else if(!strcmp(op, "C")){
            scanf("%d%d", &k, &x);
            change_k(k, x);
        }
    }
    return 0;
}