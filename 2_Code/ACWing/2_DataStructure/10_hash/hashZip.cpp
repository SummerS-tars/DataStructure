#include <iostream>
#include <cstring>

using namespace std;

const int N = 1e5 + 3;
int h[N], e[N], ne[N], idx;

int main(){
    int n;
    scanf("%d", &n);

    memset(h, -1, sizeof h);

    while(n --){
        char op[2];
        int x;
        scanf("%s%d", op, &x);

        int k = (x % N + N) % N;
        if(op[0] == 'I'){
            e[idx] = x;
            ne[idx] = h[k];
            h[k] = idx ++;
        }
        else{
            bool exist = false;
            for(int i = h[k]; i != -1; i = ne[i]){ // 插入的方法保证了拉链的最后一个节点的指针指向空（即-1）
                if(e[i] == x){
                    exist = true;
                    break;
                }
            }
            if(exist) puts("Yes");
            else puts("No");
        }
    }
}