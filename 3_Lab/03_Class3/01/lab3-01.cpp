#include <cstdio>
#include <ctime>
#include <iostream>
#define random(a, b) (rand() % (b - a) + a)

template <class Type>
class List;

template <class Type>
class LinkNode
{
    friend class List<Type>;

public:
    LinkNode() { link = nullptr; }
    LinkNode(const Type &value)
    {
        data = value;
        link = nullptr;
    }

private:
    Type data;
    LinkNode<Type> *link;
};

template <class Type>
class List
{
public:
    List() { first = nullptr; }
    List(const Type &value)
    {
        first = new LinkNode<Type>(value);
        first->link = first;
    }
    ~List();
    LinkNode<Type> *Print();

    void Initialize(const Type arr[], int size);
    void Insert(const Type &value);

private:
    LinkNode<Type> *first;
};

template <class Type>
List<Type>::~List()
{
    if (first == nullptr)
        return;

    LinkNode<Type> *current = first;
    LinkNode<Type> *next;

    do
    {
        next = current->link;
        delete current;
        current = next;
    } while (current != first);
}

template <class Type>
LinkNode<Type> *List<Type>::Print()
{
    if (first == nullptr)
    {
        std::cout << "The list is empty." << std::endl;
        return nullptr;
    }

    LinkNode<Type> *current = first;

    do
    {
        std::cout << current->data << " ";
        current = current->link;
    } while (current != first);

    std::cout << std::endl;

    return first;
}

// 初始化循环单调非递减链表
template <class Type>
void List<Type>::Initialize(const Type arr[], int size)
{
    if (size <= 0)
        return;

    first = new LinkNode<Type>(arr[0]);
    LinkNode<Type> *current = first;

    for (int i = 1; i < size; ++i)
    {
        LinkNode<Type> *newNode = new LinkNode<Type>(arr[i]);
        current->link = newNode;
        current = newNode;
    }

    current->link = first;
}

// 插入节点到循环单调非递减链表中
template <class Type>
void List<Type>::Insert(const Type &value)
{
    LinkNode<Type> *newNode = new LinkNode<Type>(value);

    if(first == nullptr)
    {
        first = newNode;
        newNode->link = newNode;
        return;
    }

    LinkNode<Type> *current = first;
    if(!current->link)
    {
        if(current->data <= value)
        {
            current->link = newNode;
            newNode->link = current;
        }
        else
        {
            newNode->link = current;
            current->link = newNode;
        }
        return;
    }

    do
    {
        /**
        three situations:
        1. 插入值在两个节点之间
        2. 插入值大于所有节点
        3. 插入值小于所有节点
        极特殊情况：所有节点值相等
        */
        if(current->data <= value && current->link->data >= value)
        {
            newNode->link = current->link;
            current->link = newNode;
            return;
        }
        if(current->data > current->link->data)
        {
            if(value >= current->data || value <= current->link->data)
            {
                newNode->link = current->link;
                current->link = newNode;
                return;
            }
        }
        current = current->link;
    }
    while(current != first);

    // 所有节点值相等
    newNode->link = first->link;
    first->link = newNode;
    return;
}

int main()
{
    std::cout << "Test List 1: with [3,4,1] first and insert 2" << std::endl;
    List<int> myList;
    int a[] = {3, 4, 1};
    myList.Initialize(a, 3);
    myList.Print();
    myList.Insert(2);
    myList.Print();

    std::cout << "Test List 2: with empty first and insert 5,3,7" << std::endl;
    List<int> myList2;
    myList2 = List<int>();
    myList2.Print();
    myList2.Insert(5);
    myList2.Print();
    myList2.Insert(3);
    myList2.Print();
    myList2.Insert(7);
    myList2.Print();

    std::cout << "Test List 3: with [5,5,5] first and insert 5,3,7" << std::endl;
    List<int> myList3;
    int b[] = {5, 5, 5};
    myList3.Initialize(b, 3);
    myList3.Print();
    myList3.Insert(5);
    myList3.Print();
    myList3.Insert(3);
    myList3.Print();
    myList3.Insert(7);
    myList3.Print();

    return 0;
}