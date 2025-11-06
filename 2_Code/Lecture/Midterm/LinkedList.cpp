#include <iostream>

template <typename T>
struct Node {
    T data;
    Node* next;

    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    LinkedList() {
        head = new Node<T>(T());
        tail = head;
        size = 0;
    }

    ~LinkedList() {}

    //* insert node at tail
    void insertTail(T val) {
        Node<T> *newNode = new Node<T>(val);
        tail->next = newNode;
        tail = newNode;
        size++;
    }

    //* delete node at index(index starts from 0)
    void deleteAt(int index) {
        if(index >= size || index < 0) {
            std::cout << "Index out of bounds" << std::endl;
            return;
        }
        
        Node<T> *node = head;
        int time = index;
        while(index-- > 0) {
            node = node->next;
        }

        Node<T> *toDelete = node->next;
        node->next = toDelete->next;
        if(toDelete == tail) {
            tail = node;
        }
        delete toDelete;
        size--;
    }
};