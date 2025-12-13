#include <iostream>
#include <vector>

void getNextArray(const std::string &pattern, std::vector<int> &next) {
    int m = pattern.size();
    next.resize(m, -1);
    int j = -1;
    for(int i = 1; i < m; ++i) {
        while(j != -1 && pattern[j + 1] != pattern[i]) {
            j = next[j]; // 回退
        }
        if(pattern[j + 1] == pattern[i]) {
            ++j;
        }
        next[i] = j;
    }
}

int kmpSearch(const std::string &A, const std::string &pattern, const std::vector<int> &next) {
    int n = A.size();
    int m = pattern.size();
    int j = -1; // pattern的指针
    for(int i = 0; i < n; ++i) {
        while(j != -1 && pattern[j + 1] != A[i]) {
            j = next[j]; // 回退
        }
        if(pattern[j + 1] == A[i]) {
            ++j;
        }
        if(j == m - 1) {
            return i - m + 1; // 找到匹配，返回起始位置
        }
    }
    return -1; // 未找到匹配
}

int main() {
    std::string A, pattern;
    A = "ababcabcabababd";
    pattern = "ababd";
    std::vector<int> next;
    getNextArray(pattern, next);
    std::cout << "Next array: ";
    for(int val : next) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    int position = kmpSearch(A, pattern, next);
    if(position != -1) {
        std::cout << "Pattern found at index: " << position << std::endl;
    } else {
        std::cout << "Pattern not found." << std::endl;
    }
    return 0;
}