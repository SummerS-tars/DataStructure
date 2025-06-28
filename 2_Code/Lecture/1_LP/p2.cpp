#include <iostream>

void deleteValueX(int* arr, int& size, int index) {
    for(; index < size; index++) {
        arr[index] = arr[index + 1];
    }

    size --;
}

int deleteAllValueX(int* arr, int& size, int x) {
    int i = 0;
    int removeCount = 0;
    while(i < size) {
        if(arr[i] == x) {
            deleteValueX(arr, size, i);
            removeCount ++;
        }
        else i ++;
    }
    return removeCount;
}

int main() {
    int arr[10] = {1, 2, 3, 4, 5, 5, 6, 6, 6, 10};
    int size = 10;

    for(int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    int removeCount = deleteAllValueX(arr, size, 6);
    std::cout << "removeCount: " << removeCount << std::endl;

    for(int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}