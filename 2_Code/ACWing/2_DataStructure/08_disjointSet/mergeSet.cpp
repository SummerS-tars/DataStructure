// * 合并集合

#include <iostream>

using namespace std;

const int N = 1e5 + 10;
int p[N];

int find(int x){
    if(p[x] != x) p[x] = find(p[x]); // * 路径压缩
    return p[x];
}

int main(){
    int n, m;
    scanf("%d %d", &n, &m);

    for(int i = 1; i <= n; i ++) p[i] = i;

    while(m --){
        char op[2];
        int a, b;
        scanf("%s", op); // * 读入字符串，会自动忽略空格
        scanf("%d %d", &a, &b);

        if(op[0] == 'M') p[find(a)] = find(b);
        else {
            if(find(a) == find(b)) puts("Yes");
            else puts("No");
        }
    }

    return 0;
}