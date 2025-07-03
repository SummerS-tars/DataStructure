// * 双链表（数组模拟）

#include <iostream>
#include <cstring>

using namespace std;

const int N = 100010;
int e[N], l[N], r[N];
int head, idx;

// ! 注意：双链表的数组实现，考虑设定一个虚拟头节点与尾节点
// * 初始化
void init(){
    r[0] = 1, l[1] = 0;
    l[0] = -1, r[1] = -1;
    idx = 2;
}

// * 在第k个插入的数后插入一个值为x的节点
// * 实际上可以通过调整传入的k来适用于所有插入操作
void insert_to_right(int k, int x){
    e[idx] = x;
    l[idx] = k;
    r[idx] = r[k];
    l[r[k]] = idx;
    r[k] = idx ++;
}

// * 删除第k个插入的数
// ! 注意：第k个插入的数应该下标是 k + 1， 因为有虚拟头节点0与尾节点1
void delete_k(int k){
    r[l[k]] = r[k];
    l[r[k]] = l[k];
}

// * 操作适配函数
void operation(char op[]){
    int k, x;
    if(strcmp(op, "L") == 0){
        scanf("%d", &x);
        insert_to_right(0, x);
    }
    else if(strcmp(op, "R") == 0){
        scanf("%d", &x);
        insert_to_right(l[1], x);
    }
    else if(strcmp(op, "D") == 0){
        scanf("%d", &k);
        delete_k(k + 1);
    }
    else if(strcmp(op, "IL") == 0){ // 插入到左侧需要通过指针定位
        scanf("%d %d", &k, &x);
        insert_to_right(l[k + 1], x);
    }
    else if(strcmp(op, "IR") == 0){ // 插入到右侧不需要通过指针定位
        scanf("%d %d", &k, &x);
        insert_to_right(k + 1, x);
    }
}

int main(){
    init();
    int m;
    scanf("%d", &m);

    while(m --){
        char op[3];
        scanf(" %s", op);
        operation(op);
    }

    for(int i = r[0]; i != 1; i = r[i]){
        printf("%d ", e[i]);
    }
    return 0;
}