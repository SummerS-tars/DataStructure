#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

/*
 * 实现思路：
 * 使用数组实现循环队列，通过两个指针和取模运算实现循环特性
 * 
 * 数据成员：
 * - data: 存储元素的数组
 * - front: 队首指针，指向队首元素的位置
 * - rear: 队尾指针，指向队尾元素的下一个位置（即下一个要插入的位置）
 * - capacity: 队列容量（需要比k多1个空间来区分空和满）
 * 
 * 关键技巧：
 * 1. 使用取模运算实现循环：(index + 1) % capacity
 * 2. 预留一个空位来区分队列空和满的情况
 *    - 队列空：front == rear
 *    - 队列满：(rear + 1) % capacity == front
 * 3. 队列实际容量是 k+1，但对外表现为 k
 * 
 * 时间复杂度：所有操作均为 O(1)
 * 空间复杂度：O(k)，其中 k 是队列容量
 */
class MyCircularQueue
{
private:
    vector<int> data;  // 存储队列元素
    int front;         // 队首指针
    int rear;          // 队尾指针（指向队尾的下一个位置）
    int capacity;      // 队列容量（实际为k+1）

public:
    // 构造器，设置队列长度为 k
    MyCircularQueue(int k)
    {
        capacity = k + 1;  // 多分配一个空间用于区分空和满
        data.resize(capacity);
        front = 0;
        rear = 0;
    }

    // 向循环队列插入一个元素，成功返回 true
    bool enQueue(int value)
    {
        // 队列已满
        if (isFull())
        {
            return false;
        }
        
        // 在 rear 位置插入元素
        data[rear] = value;
        // rear 指针循环后移
        rear = (rear + 1) % capacity;
        return true;
    }

    // 从循环队列中删除一个元素，成功返回 true
    bool deQueue()
    {
        // 队列为空
        if (isEmpty())
        {
            return false;
        }
        
        // front 指针循环后移（相当于删除队首元素）
        front = (front + 1) % capacity;
        return true;
    }

    // 从队首获取元素，如果队列为空返回 -1
    int Front()
    {
        if (isEmpty())
        {
            return -1;
        }
        return data[front];
    }

    // 获取队尾元素，如果队列为空返回 -1
    int Rear()
    {
        if (isEmpty())
        {
            return -1;
        }
        // rear 指向队尾的下一个位置，所以队尾元素在 (rear - 1 + capacity) % capacity
        return data[(rear - 1 + capacity) % capacity];
    }

    // 检查循环队列是否为空
    bool isEmpty()
    {
        return front == rear;
    }

    // 检查循环队列是否已满
    bool isFull()
    {
        // rear 的下一个位置是 front 时表示队列已满
        return (rear + 1) % capacity == front;
    }
};

int main()
{
    cout << "=== 测试用例1：基本操作 ===" << endl;
    MyCircularQueue* queue1 = new MyCircularQueue(3);
    cout << "创建容量为3的循环队列" << endl;
    
    cout << "enQueue(1): " << queue1->enQueue(1) << " (预期: 1/true)" << endl;
    cout << "enQueue(2): " << queue1->enQueue(2) << " (预期: 1/true)" << endl;
    cout << "enQueue(3): " << queue1->enQueue(3) << " (预期: 1/true)" << endl;
    cout << "enQueue(4): " << queue1->enQueue(4) << " (预期: 0/false, 队列已满)" << endl;
    
    cout << "Rear(): " << queue1->Rear() << " (预期: 3)" << endl;
    cout << "isFull(): " << queue1->isFull() << " (预期: 1/true)" << endl;
    
    cout << "deQueue(): " << queue1->deQueue() << " (预期: 1/true)" << endl;
    cout << "enQueue(4): " << queue1->enQueue(4) << " (预期: 1/true)" << endl;
    cout << "Rear(): " << queue1->Rear() << " (预期: 4)" << endl;
    cout << "Front(): " << queue1->Front() << " (预期: 2)" << endl;
    
    delete queue1;
    cout << endl;

    cout << "=== 测试用例2：空队列和循环特性 ===" << endl;
    MyCircularQueue* queue2 = new MyCircularQueue(5);
    cout << "创建容量为5的循环队列" << endl;
    
    cout << "isEmpty(): " << queue2->isEmpty() << " (预期: 1/true)" << endl;
    cout << "Front(): " << queue2->Front() << " (预期: -1, 空队列)" << endl;
    cout << "Rear(): " << queue2->Rear() << " (预期: -1, 空队列)" << endl;
    cout << "deQueue(): " << queue2->deQueue() << " (预期: 0/false, 空队列)" << endl;
    
    cout << "\n插入 10, 20, 30:" << endl;
    queue2->enQueue(10);
    queue2->enQueue(20);
    queue2->enQueue(30);
    cout << "Front(): " << queue2->Front() << " (预期: 10)" << endl;
    cout << "Rear(): " << queue2->Rear() << " (预期: 30)" << endl;
    
    cout << "\n删除两个元素:" << endl;
    queue2->deQueue();
    queue2->deQueue();
    cout << "Front(): " << queue2->Front() << " (预期: 30)" << endl;
    
    cout << "\n插入 40, 50, 60, 70 (测试循环):" << endl;
    queue2->enQueue(40);
    queue2->enQueue(50);
    queue2->enQueue(60);
    queue2->enQueue(70);
    cout << "isFull(): " << queue2->isFull() << " (预期: 1/true)" << endl;
    cout << "Front(): " << queue2->Front() << " (预期: 30)" << endl;
    cout << "Rear(): " << queue2->Rear() << " (预期: 70)" << endl;
    
    cout << "\n依次弹出所有元素: ";
    while (!queue2->isEmpty())
    {
        cout << queue2->Front() << " ";
        queue2->deQueue();
    }
    cout << "(预期: 30 40 50 60 70)" << endl;
    cout << "isEmpty(): " << queue2->isEmpty() << " (预期: 1/true)" << endl;
    
    delete queue2;
    cout << endl;

    cout << "=== 测试用例3：容量为1的队列 ===" << endl;
    MyCircularQueue* queue3 = new MyCircularQueue(1);
    cout << "创建容量为1的循环队列" << endl;
    
    cout << "enQueue(100): " << queue3->enQueue(100) << " (预期: 1/true)" << endl;
    cout << "isFull(): " << queue3->isFull() << " (预期: 1/true)" << endl;
    cout << "Front(): " << queue3->Front() << " (预期: 100)" << endl;
    cout << "Rear(): " << queue3->Rear() << " (预期: 100)" << endl;
    cout << "enQueue(200): " << queue3->enQueue(200) << " (预期: 0/false)" << endl;
    cout << "deQueue(): " << queue3->deQueue() << " (预期: 1/true)" << endl;
    cout << "isEmpty(): " << queue3->isEmpty() << " (预期: 1/true)" << endl;
    cout << "Front(): " << queue3->Front() << " (预期: -1)" << endl;
    
    delete queue3;

    return 0;
}