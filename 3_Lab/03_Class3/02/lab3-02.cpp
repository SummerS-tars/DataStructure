#include <cstdio>
#include <iostream>

class PolyLinkedList;

class TermNode
{
    friend class PolyLinkedList;
    friend TermNode *PolyAdd(TermNode *ah, TermNode *bh);
    // friend TermNode *PolyMul(TermNode *ah, TermNode *bh);

public:
    TermNode() : coef(0), exp(0), link(nullptr) {}
    TermNode(float c, int e) : coef(c), exp(e), link(nullptr) {}

private:
    float coef; // 系数
    int exp;    // 指数
    TermNode *link;
};

class PolyLinkedList
{
public:
    PolyLinkedList() : head(nullptr) {}
    PolyLinkedList(TermNode *head) : head(head) {}

    ~PolyLinkedList()
    {
    }

    // 返回链表头指针
    TermNode *getHead() const
    {
        return head;
    }

    void InitFrom2DArray(float terms[][2], int size)
    {
        head = nullptr;
        TermNode *tail = nullptr;

        // 依次遍历给定的terms数组
        for (int i = 0; i < size; ++i)
        {
            float coef = terms[i][0];
            int exp = terms[i][1];

            TermNode *newTerm = new TermNode(coef, exp);
            if (head == nullptr)
            {
                // 如果链表为空，则新节点为头结点
                head = newTerm;
                tail = newTerm;
            }
            else
            {
                // 否则将新节点连接到链表的末尾
                tail->link = newTerm;
                tail = newTerm;
            }
        }
    }

    // 打印多项式
    void PrintPoly() const
    {
        TermNode *current = head;
        if (current == nullptr)
        {
            std::cout << "0" << std::endl; // 如果链表为空，输出0
            return;
        }

        bool firstTerm = true;
        while (current != nullptr)
        {
            float coef = current->coef;
            int exp = current->exp;
            if (!firstTerm && current->coef > 0)
                std::cout << " + "; // 为正数的项前加上"+"符号
            else if (current->coef < 0)
            {
                std::cout << " - "; // 为负数的项前加上"-"符号
                coef = -coef;       // 系数取绝对值
            }

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

            current = current->link;
            firstTerm = false;
        }
        std::cout << std::endl;
    }

private:
    TermNode *head;
};

TermNode *PolyAdd(TermNode *ah, TermNode *bh)
{
    // TODO: code here
    /**
     * 1. 头部：谁小谁开头，否则以a为准将b的节点吸收
     * 2. 遍历：先移动的那边与未移动的那边的下一个比较
     *     2.1. 先动小，插入到未动的下一个的上一个，两者都后移一个
     *     2.2. 未动下一个小，未动后移一个
     *     2.3. 相等，系数相加，未动后移一个
     * 3. 尾部：a到达尾部，b未到达尾部，将b的剩余部分接到a的尾部
     * 4. 返回头部
     */
    TermNode *result = nullptr;

    TermNode *p1, *p2;
    if(ah->exp < bh->exp)
    {
        result = ah;
        p1 = ah;
        p2 = bh;
    }
    else if(ah->exp > bh->exp)
    {
        result = bh;
        p1 = bh;
        p2 = ah;    
    }
    else
    {
        ah->coef += bh->coef;
        TermNode *del = bh;
        bh = bh->link;
        delete del;
        result = ah;
        p1 = ah;
        p2 = bh;
    }

    while(p1->link != nullptr && p2 != nullptr)
    {
        if(p1->link->exp < p2->exp)
        {
            p1 = p1->link;
        }
        else if(p1->link->exp > p2->exp)
        {
            TermNode *ins = p2;
            p2 = p2->link;
            ins->link = p1->link;
            p1->link = ins;
            p1 = p1->link;
        }
        else
        {
            p1->link->coef += p2->coef;
            TermNode *del = p2;
            p2 = p2->link;
            delete del;
            p1 = p1->link;
        }
    }

    if(p1->link == nullptr && p2 != nullptr)
    {
        p1->link = p2;
    }

    // 考虑可能系数为0的情况
    for(auto cur = result; cur->link != nullptr;)
    {
        if(cur->link->coef == 0)
        {
            TermNode *del = cur->link;
            cur->link = cur->link->link;
            delete del;
        }
        else
        {
            cur = cur->link;
        }
    }
    if(result->coef == 0)
    {
        TermNode *del = result;
        result = result->link;
        delete del;
    }

    return result;
}

int main()
{
    std::cout << "Polynomial Addition using Linked List" << std::endl;

    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Case 1:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    // 创建两个多项式链表
    PolyLinkedList poly1, poly2;

    // 初始化第一个多项式 (1.8 + 2.0x^1000)
    float terms1[][2] = {{1.8, 0}, {2, 1000}};
    int size1 = sizeof(terms1) / sizeof(terms1[0]);
    poly1.InitFrom2DArray(terms1, size1);

    // 初始化第二个多项式 (1.2 + 51.3x^50 + 3.7x^101)
    float terms2[][2] = {{1.2, 0}, {51.3, 50}, {3.7, 101}};
    int size2 = sizeof(terms2) / sizeof(terms2[0]);
    poly2.InitFrom2DArray(terms2, size2);

    // 打印初始多项式
    std::cout << "Polynomial 1: ";
    poly1.PrintPoly();

    std::cout << "Polynomial 2: ";
    poly2.PrintPoly();

    // 相加两个多项式
    TermNode *result = PolyAdd(poly1.getHead(), poly2.getHead());

    std::cout << "Sum of Polynomials: ";
    PolyLinkedList resultPoly(result);
    resultPoly.PrintPoly();

    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Case 2:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    // 创建两个多项式链表
    PolyLinkedList poly3, poly4;
    // 初始化第一个多项式 (1.0 + 2.0x + 3.0x^2 + 4.0x^3)
    float terms3[][2] = {{1.0, 0}, {2.0, 1}, {3.0, 2}, {4.0, 3}};
    int size3 = sizeof(terms3) / sizeof(terms3[0]);
    poly3.InitFrom2DArray(terms3, size3);
    // 初始化第二个多项式 (4.0 + -2.0x + -3.0x^2 + 1.0x^3)
    float terms4[][2] = {{4.0, 0}, {-2.0, 1}, {-3.0, 2}, {1.0, 3}};
    int size4 = sizeof(terms4) / sizeof(terms4[0]);
    poly4.InitFrom2DArray(terms4, size4);
    // 打印初始多项式
    std::cout << "Polynomial 3: ";
    poly3.PrintPoly();
    std::cout << "Polynomial 4: ";
    poly4.PrintPoly();
    // 相加两个多项式
    TermNode *result2 = PolyAdd(poly3.getHead(), poly4.getHead());
    std::cout << "Sum of Polynomials: ";
    PolyLinkedList resultPoly2(result2);
    resultPoly2.PrintPoly();

    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Case 3:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    // 创建两个多项式链表
    PolyLinkedList poly5, poly6;
    // 初始化第一个多项式 (-1.0 + 2.0x^3 + -3.0x^5 + 4.0x^7)
    float terms5[][2] = {{-1.0, 0}, {2.0, 3}, {-3.0, 5}, {4.0, 7}};
    int size5 = sizeof(terms5) / sizeof(terms5[0]);
    poly5.InitFrom2DArray(terms5, size5);
    // 初始化第二个多项式 (1.0 + -2.0x^3 + 3.0x^5 + -4.0x^7)
    float terms6[][2] = {{1.0, 0}, {-2.0, 3}, {3.0, 5}, {-4.0, 7}};
    int size6 = sizeof(terms6) / sizeof(terms6[0]);
    poly6.InitFrom2DArray(terms6, size6);
    // 打印初始多项式
    std::cout << "Polynomial 5: ";
    poly5.PrintPoly();
    std::cout << "Polynomial 6: ";
    poly6.PrintPoly();
    // 相加两个多项式
    TermNode *result3 = PolyAdd(poly5.getHead(), poly6.getHead());
    std::cout << "Sum of Polynomials: ";
    PolyLinkedList resultPoly3(result3);
    resultPoly3.PrintPoly();

    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Case 4:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    // 创建两个多项式链表
    PolyLinkedList poly7, poly8;
    // 初始化第一个多项式 (x + x^2 + x^3)
    float terms7[][2] = {{1, 1}, {1, 2}, {1, 3}};
    int size7 = sizeof(terms7) / sizeof(terms7[0]);
    poly7.InitFrom2DArray(terms7, size7);
    // 初始化第二个多项式 (x^2 + x^4 + x^5 + x^6)
    float terms8[][2] = {{1, 2}, {1, 4}, {1, 5}, {1, 6}};
    int size8 = sizeof(terms8) / sizeof(terms8[0]);
    poly8.InitFrom2DArray(terms8, size8);
    // 打印初始多项式
    std::cout << "Polynomial 7: ";
    poly7.PrintPoly();
    std::cout << "Polynomial 8: ";
    poly8.PrintPoly();
    // 相加两个多项式
    TermNode *result4 = PolyAdd(poly7.getHead(), poly8.getHead());
    std::cout << "Sum of Polynomials: ";
    PolyLinkedList resultPoly4(result4);
    resultPoly4.PrintPoly();

    return 0;
}