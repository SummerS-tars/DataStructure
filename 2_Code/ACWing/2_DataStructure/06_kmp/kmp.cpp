#include <iostream>

using namespace std;

const int N = 1e5 + 10;
int n, m;
string str, sub;
int ne[N];

int main(){
    cin >> n >> sub >> m >> str;

    // * 预处理子串，生成next数组
    // * next子串存储子串前缀和后缀相同的最大长度
    // * next[i] 表示从子串下标从0~i的段中，相同前后缀的最大长度
    // * next[0] 定义为 0，没有任何字符匹配，从子串开头继续匹配
    // * 例如：
    // * abababab 的 next 数组为 [0, 0, 1, 2, 3, 4, 5, 6]
    // * aabbaa 的 next 数组为 [0, 1, 0, 0, 1, 2]
    for(int i = 1, j = 0; i < n; i ++){
        // * 直接从前一次匹配到的最长相同前后缀的前缀的下一个字符开始判断
        // * 表现在子串中就是当匹配完到第i个字符如果匹配失败（或者已经将子串匹配完），子串应该向后移动至与母串对其的位置下标
        while(j && sub[i] != sub[j]) j = ne[j - 1]; 
        if(sub[i] == sub[j]) j ++;
        ne[i] = j;
    }

    // * 从母串中匹配子串
    int subcnt = 0;
    for(int i = 0, j = 0; i < m; i ++){
        // * 匹配失败，可以理解为将子串向后移动，表现为指向子串的指针j根据next数组回退到最长相同前后缀的前缀的下一个字符
        while(j && str[i] != sub[j]) j = ne[j - 1]; // * 匹配到子串下标j失败，说明匹配成功到了第j-1个字符

        // * 匹配成功，将指向子串的指针j向后移动一位
        if(str[i] == sub[j]) j ++;
        if(j == n){
            subcnt ++;
            printf("%d ", i - n + 1); // * 子串头下标
            j = ne[j - 1];
        }
    }

    cout << endl << subcnt << endl;

    return 0;
}