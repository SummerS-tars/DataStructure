#include "l1-SingleLinkedList.hpp"

int main() {
    List<int> *list = new List<int>();
    list->Insert(1, 0);
    list->Insert(2, 1);
    list->Insert(3, 2);
    list->PrintList();

    delete list;
    return 0;
}