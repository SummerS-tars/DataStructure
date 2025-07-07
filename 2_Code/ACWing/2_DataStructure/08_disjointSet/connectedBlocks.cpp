// * 连通块中点的数量

#include <iostream>

using namespace std;

const int N = 1e5 + 10;
int p[N];
int s[N];

int find(int x){
    if(p[x] != x) p[x] = find(p[x]); // * 路径压缩
    return p[x];
}

int main(){
    int n, m;
    scanf("%d %d", &n, &m);

    for(int i = 1; i <= n; i ++) {
        p[i] = i;
        s[i] = 1;
    }

    while(m --){
        char op[3];
        int a, b;
        scanf("%s", op); // * 读入字符串，会自动忽略空格

        if(op[0] == 'C') { // 连接
            scanf("%d %d", &a, &b);
            if(find(a) != find(b)) {
                s[find(b)] += s[find(a)];
                p[find(a)] = find(b);
            }
        }
        else if(op[1] == '1') { // 判断是否在同一个连通块中
            scanf("%d %d", &a, &b);
            if(find(a) == find(b)) puts("Yes");
            else puts("No");
        }
        else { // 输出连通块中点的数量
            scanf("%d", &a);
            printf("%d\n", s[find(a)]);
        }
    }

    return 0;
}