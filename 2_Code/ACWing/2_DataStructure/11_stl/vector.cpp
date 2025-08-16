#include <iostream>
#include <vector>

using namespace std;

int main(){
    // * 初始化方法
    vector<int> a;
    vector<int> b(10);
    vector<int> c(10, 3);
    vector<int> d(b);

    // * 常用函数
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    for(auto x : a){
        printf("%d ", x);
    }
    printf("\n");

    a.pop_back();
    a.insert(a.begin(), 4);
    a.erase(a.begin()); // * 删除单个元素
    a.erase(a.begin(), a.begin() + 2); // * 删除区间元素
    printf("a.size() = %d\n", (int)a.size());
    printf("a.empty() = %d\n", a.empty());
    a.clear(); // * 清空，并非所有容器都有clear函数
    printf("a.size() = %d\n", (int)a.size());
    printf("a.empty() = %d\n", a.empty());

    // * 遍历
    for(int i = 0; i < a.size(); i ++){
        printf("%d ", a[i]);
    }
    for(auto x : a){}

    // ! 注意：系统为程序分配空间耗时，所需时间基本只与申请次数有关，与其大小无关
}