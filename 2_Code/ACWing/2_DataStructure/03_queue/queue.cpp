#include <iostream>
#include <cstring>

using namespace std;

const int N = 100010;
int q[N], hh, tt;

void init(){
    hh = 0, tt = -1;
}

void push(int x){
    q[++tt] = x;
}

void pop(){
    hh ++;
}

bool empty(){
    return hh > tt;
}

int query(){
    return q[hh];
}

void operation(char *op){
    if(strcmp(op, "push") == 0){
        int k;
        scanf("%d", &k);
        push(k);
    }
    else if(strcmp(op, "pop") == 0){
        pop();
    }
    else if(strcmp(op, "empty") == 0){
        bool is_empty = empty();
        if(is_empty) printf("YES\n");
        else printf("NO\n");
    }
    else if(strcmp(op, "query") == 0){
        printf("%d\n", query());
    }
}

int main(){
    init();
    int m;
    scanf("%d", &m);

    while(m --){
        char op[10] = {0};
        scanf("%s", op);
        operation(op);
    }

    return 0;
}