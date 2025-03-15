#include "heap_sort.h"
#include <iostream>

using namespace std;

// heapify down (max heap) - O(logn)
void maxHeapify(int arr[], int heap_size, int i) {
    int l = 2*i + 1;
    int r = 2*i + 2;

    int largest = i;

    if (l < heap_size && arr[l] > arr[largest])
        largest = l;
    if (r < heap_size && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        swap(arr[largest], arr[i]);
        maxHeapify(arr, heap_size, largest);
    }
}

// build max heap - O(n), bottom up approach (start from the last non-leaf node)
void buildMaxHeap(int arr[], int n) {
    for (int i = n/2-1; i >= 0; i--)
        maxHeapify(arr, n, i);
}

// heap sort (max heap) - O(nlogn), sorts in ascending order
void heapSort(int arr[], int n) {
    buildMaxHeap(arr, n);

    for (int i = n-1; i > 0; i--) {
        swap(arr[i], arr[0]);
        maxHeapify(arr, i, 0);
    }
}
