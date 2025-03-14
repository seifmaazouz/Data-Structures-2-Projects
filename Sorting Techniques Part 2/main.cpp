#include <iostream>
#include <vector>
#include "sorting_techniques_part1.h"
#include "heap_sort.h"

using namespace std;

int* generateRandArray(int size);
void measureRuntime(int size);
void printArray(int arr[], int n);

// Merge Sort
void mergeSort(int arr[], int low, int high);
void merge(int arr[], int low, int mid, int high);

int main() {
    // seed random number generator to get different random numbers each time
    srand(time(nullptr));

    int testSizes[] = {1000, 10000, 25000, 50000, 75000, 100000};

    for (int size : testSizes) {
        cout << "Testing size: " << size << endl;
        measureRuntime(size);
        cout << endl;
    }

    return 0;
}

int* generateRandArray(int size) {
    int* arr = new int[size];

    for (int i = 0; i < size; i++)
        arr[i] = rand();

    return arr;
}

void measureRuntime(int size) {
    clock_t start, end;
    double cpu_time_used;

    int* arr = generateRandArray(size);
    int* arrCopy = new int[size];

    // Bubble Sort
    memcpy(arrCopy, arr, size * sizeof(int));
    start = clock();
    bubbleSort(arrCopy, size);
    end = clock();
    cpu_time_used = (((double)(end - start)) / CLOCKS_PER_SEC) * 1000;
    printf("Running time for Bubble Sort is %f ms\n", cpu_time_used);

    // Selection Sort
    memcpy(arrCopy, arr, size * sizeof(int));
    start = clock();
    selectionSort(arrCopy, size);
    end = clock();
    cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;
    printf("Running time for Selection Sort is %f ms\n", cpu_time_used);

    // Insertion Sort
    memcpy(arrCopy, arr, size * sizeof(int));
    start = clock();
    insertionSort(arrCopy, size);
    end = clock();
    cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;
    printf("Running time for Insertion Sort is %f ms\n", cpu_time_used);

    // Heap Sort
    memcpy(arrCopy, arr, size * sizeof(int));
    start = clock();
    heapSort(arrCopy, size);
    end = clock();
    cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;
    printf("Running time for Heap Sort is %f ms\n", cpu_time_used);

    // Merge Sort
    memcpy(arrCopy, arr, size * sizeof(int));
    start = clock();
    mergeSort(arrCopy, 0, size - 1);
    end = clock();
    cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;
    printf("Running time for Merge Sort is %f ms\n", cpu_time_used);

    delete[] arr;
    delete[] arrCopy;
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

void mergeSort(int arr[], int low, int high) {
    if (low >= high) return;
    int mid = low + (high - low) / 2;
    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);
    merge(arr, low, mid, high);
}

void merge(int arr[], int low, int mid, int high) {
    int size = high - low + 1;
    int temp[size];
    int index = 0;
    int left = low;
    int right = mid + 1;
    while (left <= mid && right <= high) {
        if (arr[left] <= arr[right]) {
            temp[index++] = arr[left++];
        }
        else {
            temp[index++] = arr[right++];
        }
    }
    while (left <= mid) {
        temp[index++] = arr[left++];
    }
    while (right <= high) {
        temp[index++] = arr[right++];
    }
    for (int i = 0; i < size; i++) {
        arr[low + i] = temp[i];
    }
}
