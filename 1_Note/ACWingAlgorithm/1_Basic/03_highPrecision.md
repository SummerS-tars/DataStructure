# 高精度算法

只有C++需要这个实现  
Java、Python均有内部实现  

需求来源于C++的整数范围限制  
采用数组的方式存储每一位数字  

实现的本质是模拟人工竖式计算  

## 1. 实现细节

输入输出转化模版

```cpp
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
```

重点：  
采用类似小端法的方法存储大数  
将十进制中的高位放在数组低位  
这样方便进位  

补充：  
采用vector实现，其自身可以自动扩容以及记录长度  

### 1.1. 高精度加法

比较简答  
模仿竖式计算  
记得进位

```cpp
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
```

### 1.2. 高精度减法

比加法略微复杂一些  
我们竖式减法时，需要从低位向高位借位  
值得一提的是，竖式减法的另一种理解：
[竖式可以小减大吗？](https://www.zhihu.com/question/434174411)  

以下返回所得的绝对值，最后输出时判断是否需要添加负号  

```cpp
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
```

### 1.3. 高精度乘法

此处只考虑高精度整数乘以一个常规整数  
下面的除法也一样  

```cpp
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
```

### 1.4. 高精度除法

除法和其他的三种运算略微不同  
（考虑竖式计算，加减乘类似，除法有点不一样）  

除法竖式计算时，从高位开始计算  
而商一开始就是高位在前，最后通过reverse与其他结果统一  

```cpp
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
```
