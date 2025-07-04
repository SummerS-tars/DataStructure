#include <iostream>
#include <cstring>

using namespace std;

const int N = 100010;
int stk[N], tt;

void init(){
    tt = -1;
}

void push(int x){
    stk[++tt] = x;
}

void pop(){
    tt --;
}

bool empty(){
    return tt == -1;
}

int top(){
    return stk[tt];
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
        printf("%d\n", top());
    }
}

int main(){
    init();  // 添加初始化调用
    int m;
    scanf("%d", &m);
    
    while(m --){
        char op[10] = {0};
        scanf("%s", op);
        operation(op);
    }

    return 0;
}