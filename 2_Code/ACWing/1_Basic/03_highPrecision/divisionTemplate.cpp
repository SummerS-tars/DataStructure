#include <iostream>
#include <vector>
#include <algorithm>

// * 高精度整数除以一个常规整数
std::vector<int> del(std::vector<int> &A, int b);

// * 存储，类似于小端法，低位在数组前（下标小），以方便进位
int main() {
    std::string a;
    int b;
    std::vector<int> A;

    std::cin >> a >> b;

    for(int i = a.size() - 1; i >= 0; i --) A.push_back(a[i] - '0');

    auto C = del(A, b);

    for(int i = C.size() - 1; i >= 0; i --) std::cout << C[i];

    return 0;
}

std::vector<int> del(std::vector<int> &A, int b) {
    std::vector<int> C;

    int r = 0; // 存储余数
    for(int i = A.size() - 1; i >= 0; i --) {
        r = r * 10 + A[i];
        C.push_back(r / b);
        r %= b;
    }

    reverse(C.begin(), C.end());

    while(C.size() > 1 && C.back() == 0) C.pop_back();

    return C;
}