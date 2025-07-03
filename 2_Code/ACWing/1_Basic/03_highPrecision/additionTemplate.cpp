#include <iostream>
#include <vector>

#define N 1000010

// * 高精度加法
std::vector<int> add(std::vector<int> &A, std::vector<int> &B);

// * 存储，类似于小端法，低位在数组前（下标小），以方便进位
int main() {
    std::string a, b;
    std::vector<int> A, B;

    std::cin >> a >> b;

    for(int i = a.size() - 1; i >= 0; i --) A.push_back(a[i] - '0');
    for(int i = b.size() - 1; i >= 0; i --) B.push_back(b[i] - '0');

    auto C =add(A, B);

    for(int i = C.size() - 1; i >= 0; i --) std::cout << C[i];

    return 0;
}

std::vector<int> add(std::vector<int> &A, std::vector<int> &B) {
    std::vector<int> C;

    int t = 0; // 中间值
    for(int i = 0; i < A.size() || i < B.size(); i ++) {
        if(i < A.size()) t += A[i];
        if(i < B.size()) t += B[i];
        C.push_back(t % 10); // * 取个位
        t /= 10; // * 保留进位
    }

    if(t) C.push_back(t); // * 如果最后还有进位，则直接添加

    return C;
}