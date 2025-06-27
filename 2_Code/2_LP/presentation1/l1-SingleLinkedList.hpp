#ifndef SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_H

#include <cstdio>

// Default to TYPE_1 if no type is specified
#if !defined(TYPE_1) && !defined(TYPE_2)
#define TYPE_1
#endif

#if defined(TYPE_1)

template <class T> class List;
template <class T> class LinkNode {
    // * 友元类，可以访问私有成员
    friend class List<T>;
    private:
        T data;
        LinkNode<T> *next;
    public:
        // * C++语法糖，构造函数初始化列表
        LinkNode() : data(0), next(nullptr) {}
        LinkNode(T value, LinkNode<T> *nextNode = nullptr) : data(value), next(nextNode) {}
        ~LinkNode() {}

        T getData() const {return data;}
        LinkNode<T> *getNext() const {return next;}
        void setData(T value) {data = value;}
        void setNext(LinkNode<T> *nextNode) {next = nextNode;}
};

template <class T> class List {
    private:
        LinkNode<T> *head;
    public:
        List(const T &value) { head = new LinkNode<T>(value);}
        List() { head = new LinkNode<T>(0);}
        ~List() {
            LinkNode<T> *currentNode = head;
            while (currentNode != nullptr) {
                LinkNode<T> *nextNode = currentNode->getNext();
                delete currentNode;
                currentNode = nextNode;
            }
        }

        void MakeEmpty() {
            LinkNode<T> *currentNode = head;
            while (currentNode != nullptr) {
                LinkNode<T> *nextNode = currentNode->getNext();
                delete currentNode;
                currentNode = nextNode;
            }
        }

        int Length() const {
            int length = 0;
            LinkNode<T> *currentNode = head;
            while (currentNode != nullptr) {
                length++;
                currentNode = currentNode->getNext();
            }
            return length;
        }
        
        LinkNode<T>* Find(T value) {
            LinkNode<T> *currentNode = head;
            while (currentNode != nullptr) {
                if (currentNode->getData() == value) {
                    return currentNode;
                }
                currentNode = currentNode->getNext();
            }
        }

        int Insert(T value, int position) {
            // * position也从0开始计算
            // ? 插入错误是不是应该返回非0（例如1）
            if (position < 0 || position > Length()) {
                return 0;
            }

            LinkNode<T> *newNode = new LinkNode<T>(value);
            if (position == 0) {
                newNode->setNext(head);
                head = newNode;
            } else {
                LinkNode<T> *currentNode = head;
                for (int i = 0; i < position - 1; i++) {
                    currentNode = currentNode->getNext();
                }
                newNode->setNext(currentNode->getNext());
                currentNode->setNext(newNode);
            }
            return 1;
        }

        int Delete(int position) {
            if (position < 0 || position >= Length()) {
                return 0;
            }

            LinkNode<T> *currentNode = head;
            // * 遍历至要删除的节点位置
            for (int i = 0; i < position - 1; i++) {
                currentNode = currentNode->getNext();
            }
            LinkNode<T> *nextNode = currentNode->getNext();
            currentNode->setNext(nextNode->getNext());
            delete nextNode;
            return 1;
        }

        void PrintList() const {
            LinkNode<T> *currentNode = head;
            while (currentNode != nullptr) {
                printf("%d ", currentNode->getData());
                currentNode = currentNode->getNext();
            }
            printf("\n");
        }
};


#elif defined(TYPE_2)

#include <cstdio>

template <class T> class List;
template <class T> class LinkNode {
    friend class List<T>;
    private:
        T data;
        LinkNode<T> *next;
    public:
        LinkNode() : data(0), next(nullptr) {}
        LinkNode(T value, LinkNode<T> *nextNode = nullptr) : data(value), next(nextNode) {}
        ~LinkNode() {}

        T getData() const {return data;}
        LinkNode<T> *getNext() const {return next;}
        void setData(T value) {data = value;}
        void setNext(LinkNode<T> *nextNode) {next = nextNode;}
};

// * 此处2文件使用带表头的单链表，头节点始终存在，不存数据，利于统一操作
template <class T> class List {
    private:
        LinkNode<T> *head;
    public:
        List() { head = new LinkNode<T>(0);}
        ~List() {
            LinkNode<T> *currentNode = head->getNext();
            while (currentNode != nullptr) {
                LinkNode<T> *nextNode = currentNode->getNext();
                delete currentNode;
                currentNode = nextNode;
            }
        }

        void MakeEmpty() {
            LinkNode<T> *currentNode = head->getNext();
            while (currentNode != nullptr) {
                LinkNode<T> *nextNode = currentNode->getNext();
                delete currentNode;
                currentNode = nextNode;
            }
        }

        int Length() const {
            int length = 0;
            LinkNode<T> *currentNode = head->getNext();
            while (currentNode != nullptr) {
                length++;
                currentNode = currentNode->getNext();
            }
            return length;
        }
        
        LinkNode<T>* Find(T value) {
            LinkNode<T> *currentNode = head->getNext();
            while (currentNode != nullptr) {
                if (currentNode->getData() == value) {
                    return currentNode;
                }
                currentNode = currentNode->getNext();
            }
        }

        int Insert(T value, int position) {
            if (position < 0 || position > Length()) {
                return 0;
            }

            LinkNode<T> *newNode = new LinkNode<T>(value);
            LinkNode<T> *currentNode = head;
            for (int i = 0; i < position; i++) {
                currentNode = currentNode->getNext();
            }
            newNode->setNext(currentNode->getNext());
            currentNode->setNext(newNode);
            return 1;
        }

        int Delete(int position) {
            if (position < 0 || position >= Length()) {
                return 0;
            }

            LinkNode<T> *currentNode = head;
            for (int i = 0; i < position; i++) {
                currentNode = currentNode->getNext();
            }
            LinkNode<T> *nextNode = currentNode->getNext();
            currentNode->setNext(nextNode->getNext());
            delete nextNode;
            return 1;
        }

        void PrintList() const {
            LinkNode<T> *currentNode = head->getNext();
            while (currentNode != nullptr) {
                printf("%d ", currentNode->getData());
                currentNode = currentNode->getNext();
            }
            printf("\n");
        }
};

#endif // TYPE_2

#endif // SINGLE_LINKED_LIST_H