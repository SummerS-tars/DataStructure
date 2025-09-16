#include <cstdio>
#include <iostream>
#include <assert.h>

using namespace std;

#define MAXN 100 // 多项式最大项数

class Term
{
    friend class Polynomial;

private:
    float coef; // 系数
    int exp;    // 指数
};

class Polynomial
{
public:
    Polynomial();                          // 无形参构造函数，返回多项式A(x)=0
    ~Polynomial();                         // 析构函数
    int Poly_Append(float c, int e);       // 插入系数为c、指数为e的项至指针free所指示位置
    Polynomial *Poly_Mul(Polynomial poly); // 返回多项式*this与多项式poly的乘
    void Print() const;                    // 打印多项式

private:
    static Term *Poly_Array; // 存放多项式的数组
    static int free;         // 多项式有效项数的下一位置
    int start, finish;
    // void AddTerm(float c, int e);
    void quick_sort_map(float *coefArr, int *expArr, int l, int r);
    void merge_terms(float *coefArr, int *expArr, int *size);
};

// 在类外初始化 Term, 所有 Ploy_Array 共享同一块内存
Term *Polynomial::Poly_Array = new Term[MAXN + 1];
int Polynomial::free = 0; // 初始化 free 为 0

Polynomial::Polynomial()
{
    start = free;      // 初始化 start 为当前的 free 位置
    finish = free - 1; // 表示没有项，finish 初始化为 free 之前的位置
}

Polynomial::~Polynomial()
{
}

int Polynomial::Poly_Append(float c, int e)
{
    if (free >= MAXN)
    {
        return -1; // 超过多项式最大项数
    }
    Poly_Array[free].coef = c; // 插入系数
    Poly_Array[free].exp = e;  // 插入指数
    free++;                    // 更新 free 指向下一个空闲位置
    finish = free - 1;         // 更新 finish 指向最后一个有效项
    return 0;                  // 成功插入
}

void Polynomial::Print() const
{
    bool isZero = true; // 标志是否是零多项式

    for (int i = start; i <= finish; i++)
    {
        float coef = Poly_Array[i].coef;
        int exp = Poly_Array[i].exp;

        // 忽略系数为 0 的项
        if (coef == 0)
        {
            continue;
        }

        isZero = false; // 存在非零项

        // 输出符号
        if (i > start && coef > 0)
        {
            std::cout << " + ";
        }
        else if (coef < 0)
        {
            std::cout << " - ";
            coef = -coef; // 如果系数为负数，符号已处理，接下来输出绝对值
        }

        // 输出系数（只在系数不是 1 或 -1 时显示）
        if (coef != 1 || exp == 0)
        {
            std::cout << coef;
        }

        // 输出变量部分
        if (exp != 0)
        {
            std::cout << "x";
            if (exp != 1)
            {
                std::cout << "^" << exp; // 只在指数不为 1 时显示幂次
            }
        }
    }

    // 如果所有项的系数都为 0，则输出 "0"
    if (isZero)
    {
        std::cout << "0";
    }

    std::cout << std::endl; // 换行
}

/**
 ** time complexity: O(m^2)*(n^2))
 ** m, n are respectively the number of terms of the two polynomials
 ** space complexity: O(1)
 ** it can be O(mn)
 ** which depends on whether the store of the result polynomial is needed or not
 ** here we assume that it is needed
 ** so we don't need extra space except several variables
 */
// Polynomial *Polynomial::Poly_Mul(Polynomial poly)
// {
//     Polynomial *result = new Polynomial();

//     for(int i = start; i <= finish; i++)
//     {
//         for(int j = poly.start; j <= poly.finish; j++)
//         {
//             float newCoef = Poly_Array[i].coef * poly.Poly_Array[j].coef;
//             int newExp = Poly_Array[i].exp + poly.Poly_Array[j].exp;
//             result->AddTerm(newCoef, newExp); // O(n) time consumption operation
//         }
//     }

//     return result;
// }

/**
 ** time complexity: O(mn)
 ** space complexity: O(1)
 */
// void Polynomial::AddTerm(float c, int e)
// {
//     if(finish < start) // 多项式当前无项，直接插入
//     {
//         Poly_Array[free].coef = c;
//         Poly_Array[free].exp = e;
//         start = free;
//         finish = free;
//         free ++;
//         return;
//     }

//     for(int i = start; i <= finish; i ++)
//     {
//         if(Poly_Array[i].exp == e) // 有同类项，直接合并
//         {
//             Poly_Array[i].coef += c;
//             return;
//         }
        
//         if(Poly_Array[i].exp > e) // 无同类项，根据指数从小到大插入
//         {
//             for(int j = finish + 1; j > i; j --) // ! 受基础实现限制，此处可能会越界，但不在重点测试范围内
//                 Poly_Array[j] = Poly_Array[j - 1]; // ? 能直接对对象这样做吗：可以
//             Poly_Array[i].coef = c;
//             Poly_Array[i].exp = e;
//             finish ++;
//             free ++;
//             return;
//         }
//     }

//     // 指数最大，插入到最后
//     Poly_Array[free].coef = c;
//     Poly_Array[free].exp = e;
//     finish = free;
//     free ++;
// }

/**
 ** time complexity: O(mnlog(mn))
 ** m, n are respectively the number of terms of the two polynomials
 ** first calculate all the mn terms and store them in two arrays
 ** then sort the two arrays according to the exponent using quick sort
 ** after that merge the terms with the same exponent
 ** finally insert the terms into the result polynomial
 **
 ** space complexity: O(mn)
 ** use m*n extra terms' space to store the intermediate results
 */
Polynomial *Polynomial::Poly_Mul(Polynomial poly)
{
    Polynomial *result = new Polynomial();

    float coefArr[(finish - start + 1) * (poly.finish - poly.start + 1)];
    int expArr[(finish - start + 1) * (poly.finish - poly.start + 1)];
    int idx = 0;

    for(int i = start; i <= finish; i ++)
    {
        for(int j = poly.start; j <= poly.finish; j ++)
        {
            coefArr[idx] = Poly_Array[i].coef * poly.Poly_Array[j].coef;
            expArr[idx] = Poly_Array[i].exp + poly.Poly_Array[j].exp;
            idx++;
        }
    }

    quick_sort_map(coefArr, expArr, 0, idx - 1); // O(mnlog(mn))
    merge_terms(coefArr, expArr, &idx); // O(mn)
    for(int i = 0; i < idx; i ++)
    {
        result->Poly_Append(coefArr[i], expArr[i]);
    }

    return result;
}

void Polynomial::quick_sort_map(float *coefArr, int *expArr, int l, int r)
{
    class Polynomial;

    if(l >= r) return;

    int x = expArr[(l+r) >> 1], i = l - 1, j = r + 1;
    while(i < j)
    {
        do i ++; while(expArr[i] < x);
        do j --; while(expArr[j] > x);
        if(i < j) 
        {
            swap(expArr[i], expArr[j]);
            swap(coefArr[i], coefArr[j]);
        }
    }
    quick_sort_map(coefArr, expArr, l, j);
    quick_sort_map(coefArr, expArr, j + 1, r);
}

void Polynomial::merge_terms(float *coefArr, int *expArr, int *size)
{
    int k = 1; // 记录项的个数
    for(int i = 1, j = 0; i < *size; i ++)
    {
        if(expArr[i] != expArr[j])
        {
            j ++;
            coefArr[j] = coefArr[i];
            expArr[j] = expArr[i];
            k ++;
        }
        else 
        {
            coefArr[j] += coefArr[i];
            if(coefArr[j] == 0) // 合并后系数为0，删除该项
            {
                for(int m = j; m < *size - 1; m ++)
                {
                    coefArr[m] = coefArr[m + 1];
                    expArr[m] = expArr[m + 1];
                }
                j --;
                k --;
            }
        }
    }
    *size = k;
}

int main()
{
    // 创建多项式 A = 1.8 + 2.0x^1000
    Polynomial A;
    A.Poly_Append(1.8, 0);    // 1.8
    A.Poly_Append(2.0, 1000); // 2.0x^1000

    // 创建多项式 B = 1.2 + 51.3x^50 + 3.7x^1000
    Polynomial B;
    B.Poly_Append(1.2, 0);    // 1.2
    B.Poly_Append(51.3, 50);  // 51.3x^50
    B.Poly_Append(3.7, 1000); // 3.7x^1000

    // 进行乘法运算
    Polynomial *result = A.Poly_Mul(B);

    // 输出结果，期望结果为 2.16 + 92.34x^50 + 9.06x^1000 + 102.6x^1050 + 7.4x^2000
    std::cout << "Test Case: (1.8 + 2.0x^1000) * (1.2 + 51.3x^50 + 3.7x^1000)" << std::endl;
    result->Print(); // 应输出：2.16 + 92.34x^50 + 9.06x^1000 + 102.6x^1050 + 7.4x^2000

    Polynomial A1;
    A1.Poly_Append(6, 0); // 6
    A1.Poly_Append(-1, 1); // -1x
    A1.Poly_Append(1, 2); // x^2
    Polynomial B1;
    B1.Poly_Append(6, 0); // 61
    B1.Poly_Append(-1, 1); // -1x1
    B1.Poly_Append(1, 2); // x^2

    // 进行乘法运算
    Polynomial *result1 = A1.Poly_Mul(B1);
    // 输出结果，期望结果为 36 - 12x + 13x^2 - 2x^3 + x^4
    std::cout << "Test Case: (6 - 1x + 1x^2) * (6 - 1x + 1x^2)" << std::endl;
    result1->Print(); // 应输出：36 - 12x + 13x^2 - 2x^3 + x^4

    Polynomial A2;
    A2.Poly_Append(0,0); // 0
    Polynomial B2;
    B2.Poly_Append(1, 0);
    B2.Poly_Append(-1, 1);
    B2.Poly_Append(1, 2);
    B2.Poly_Append(-1, 3);

    // 进行乘法运算
    Polynomial *result2 = A2.Poly_Mul(B2);
    // 输出结果，期望结果为 0
    std::cout << "Test Case: (0) * (1 - 1x + 1x^2 - 1x^3)" << std::endl;
    result2->Print(); // 应输出：0

    return 0;
}