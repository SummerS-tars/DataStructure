// * 单链表（数组模拟）
// ! 注意：此处的实现只针对算法题的基础理解，因此不考虑内存释放问题

#include <iostream>

using namespace std;

const int N = 100010;

int e[N], ne[N], head, idx;

// * head指向头节点，idx表示节点数量
// * 表现在数组实现上，head存储头节点的下表，idx指向数组e中启用的最后的下标
// * 模拟中，idx只会增加，不会减少

void init(){
    head = -1; // -1 表示指向空节点
    idx = 0;
}

// * 向链表头插入一个值为x的节点
void insert_to_head(int x){
    e[idx] = x;
    ne[idx] = head;
    head = idx ++;
}

// * 向第k个插入的数后插入一个值为x的节点
void insert_after_k(int k, int x){
    e[idx] = x;
    ne[idx] = ne[k];
    ne[k] = idx ++;
}

// * 将第k个插入的数后的节点删除
void delete_after_k(int k){
    ne[k] = ne[ne[k]];
}

// ! 注意：第k个插入的数，并非指当前链表的第k个节点

void operation(char op){
    int k, x;
    switch(op){
        case 'H':
            scanf("%d", &x);
            insert_to_head(x);
            break;
        case 'I':
            scanf("%d%d", &k, &x);
            insert_after_k(k - 1, x);
            break;
        case 'D':
            scanf("%d", &k);
            if(!k) head = ne[head];
            else delete_after_k(k - 1);
            break;
    }
}

int main(){
    init();  // 添加初始化调用
    int m;
    scanf("%d", &m);  // 改用scanf

    while(m --){
        char op;
        scanf(" %c", &op);  // 在%c前加空格来跳过空白字符
        operation(op);
    }

    for(int i = head; i != -1; i = ne[i]){
        printf("%d ", e[i]);
    }

    return 0;
}

/** 
 * 在你的代码中，你使用了混合的输入方式：cin >> m 然后 scanf("%c ", &op)。这会导致输入缓冲区问题。
 * 当你用cin >> m读取数字后，输入缓冲区中会留下一个换行符，这会影响后续的scanf("%c ", &op)的读取。
 * 
 * 关键改变：
 * 统一使用scanf 而不是混合cin和scanf
 * 在%c前加空格 (" %c") 来自动跳过前面的空白字符（包括换行符）
 * 这个问题很常见：当你混合使用cin和scanf时，cin >> m会留下换行符在输入缓冲区中，导致scanf("%c ", &op)读取到错误的字符。
 * 现在你的代码应该能正确处理示例数据了。让我们来验证一下逻辑：
 * 按照你的示例输入，正确的输出应该是一个链表，而不是只有"6"。修复输入解析问题后，程序应该能正确处理所有的插入和删除操作。
 * 试试修改后的代码吧！
 */