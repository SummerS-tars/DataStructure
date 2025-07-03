#include <iostream>
#include <vector>

// * 高精度减法
std::vector<int> sub(std::vector<int> &A, std::vector<int> &B);
bool cmp(std::vector<int> &A, std::vector<int> &B);

// * 存储，类似于小端法，低位在数组前（下标小），以方便进位
int main() {
    std::string a, b;
    std::vector<int> A, B;

    std::cin >> a >> b;

    for(int i = a.size() - 1; i >= 0; i --) A.push_back(a[i] - '0');
    for(int i = b.size() - 1; i >= 0; i --) B.push_back(b[i] - '0');

    auto C = sub(A, B);
    if(!cmp(A, B)) std::cout << "-";

    for(int i = C.size() - 1; i >= 0; i --) std::cout << C[i];
    std::cout << std::endl;

    return 0;
}

std::vector<int> sub(std::vector<int> &A, std::vector<int> &B) {
    if(!cmp(A, B)) return sub(B, A);

    std::vector<int> C;
    int t = 0;
    for(int i = 0; i < A.size(); i ++) {
        if(i < B.size()) t = A[i] - t - B[i];
        else t = A[i] - t;
        if(t < 0) {
            C.push_back(t + 10);
            t = -1;
        }
        else {
            C.push_back(t);
            t = 0;
        }
    }

    while(C.size() > 1 && C.back() == 0) C.pop_back(); // * 去除前导0（两个相减数如果类似可能出现）

    return C;
}

/**
 * 比较两个高精度数的大小
 * 
 * @param A 高精度数A
 * @param B 高精度数B
 * @return 如果A大于等于B，返回true，否则返回false
 */
bool cmp(std::vector<int> &A, std::vector<int> &B) {
    if(A.size() != B.size()) return A.size() > B.size();

    for(int i = A.size() - 1; i >= 0; i --){
        if(A[i] != B[i]) return A[i] > B[i];
    }

    return true;
}