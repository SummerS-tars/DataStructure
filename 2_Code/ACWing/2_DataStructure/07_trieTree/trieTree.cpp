#include <iostream>

using namespace std;

const int L = 1e5 + 10;
int son[L][26], cnt[L], idx;
// 编号为0的节点为root结点，为空

void insert(char str[]){
    int p = 0;
    for(int i = 0; str[i] ; i++){
        int c = str[i] - 'a';
        if(!son[p][c]) son[p][c] = ++idx; // * 子节点不存在，创建子节点
        p = son[p][c]; // * 移动到子节点
    }
    cnt[p] ++; // * 记录该字符串的末尾节点
}

int query(char str[]){
    int p = 0;
    for(int i = 0; str[i]; i ++){
        int c = str[i] - 'a';
        if(!son[p][c]) return 0; // * 子节点不存在，说明该字符串不存在
        p = son[p][c];
    }
    return cnt[p];
}

int main(){
    int n;
    scanf("%d", &n);
    getchar();

    while(n --){
        char op, str[L] = {0};
        scanf("%c %s", &op, str);
        getchar();

        // printf("op: %c\n", op);
        // printf("str: %s\n", str);

        if(op == 'I') insert(str);
        else if(op == 'Q') printf("%d\n", query(str));
    }

    return 0;
}