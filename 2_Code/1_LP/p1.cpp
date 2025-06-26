#include <iostream>
#include <cstdlib>

#define N 1000
#define TIMES 1000
#define MIN_VALUE 1
#define MAX_VALUE 1000

// Function to generate random integer within the specified range
int generateRandomInt() {
    return MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
}

// Function to generate random integer within a custom range
int generateRandomIntInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

void initArray(int* arr) {
    for(int i = 0; i < N; i++) {
        arr[i] = generateRandomInt();
    }
}

int findMinMax(int* arr) {
    int count = 0;
    int min, max;
    min = max = arr[0];
    // 按下标奇偶性分组，两两成对，分为两个N/2
    // 然后对内比较，先得出大小后再与整体最大最小比较
    // 因此就是3次稳定的N/2次比较
    int big, small;
    for(int i = 0; i <= (N/2 - 1)*2 ; i += 2) {
        count ++;
        if(arr[i] > arr[i+1]) big = arr[i], small = arr[i+1];
        else big = arr[i+1], small = arr[i];

        count ++;
        max = big > max ? big : max;

        count ++;
        min = small < min ? small : min;
    }

    if(N % 2 == 1) {
        count ++;
        max = max > arr[N-1] ? max : arr[N-1];

        count ++;
        min = min < arr[N-1] ? min : arr[N-1];
    }

    std::cout << "count: " << count << " min: " << min << " max: " << max << std::endl;
    return count;
}

int main() {
    // Initialize random seed
    int countTotal = 0;
    int array[N];
    for(int i = 0; i < TIMES; i++) {
        srand(i);
        initArray(array);
        countTotal += findMinMax(array);
    }

    std::cout << "countTotal: " << countTotal << " average: " << countTotal / TIMES << std::endl;

    return 0;
}