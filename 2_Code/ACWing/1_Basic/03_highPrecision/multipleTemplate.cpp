#include <iostream>
#include <vector>

// * 高精度整数乘一个常规整数
std::vector<int> mul(std::vector<int> &A, int b);

// * 存储，类似于小端法，低位在数组前（下标小），以方便进位
int main() {
    std::string a;
    int b;
    std::vector<int> A;

    std::cin >> a >> b;

    for(int i = a.size() - 1; i >= 0; i --) A.push_back(a[i] - '0');

    auto C = mul(A, b);

    for(int i = C.size() - 1; i >= 0; i --) std::cout << C[i];

    return 0;
}

std::vector<int> mul(std::vector<int> &A, int b) {
    std::vector<int> C;

    if(b == 0){
        C.push_back(0);
        return C;
    }

    int t = 0;
    for(int i = 0; i < A.size() || t; i ++) {
        if(i < A.size()) t += A[i] * b;
        C.push_back(t % 10);
        t /= 10;
    }

    return C;
}