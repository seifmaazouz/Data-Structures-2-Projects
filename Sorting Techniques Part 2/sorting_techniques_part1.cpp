#include "sorting_techniques_part1.h"
#include <iostream>

using namespace std;

void bubbleSort(int arr[], int size) {
    bool isSorted;
    for (int i = 0; i < size - 1; i++) {
        isSorted = true;
        for (int j = 0; j < size - 1 - i; j++) {
            // if value before is greater than after --> swap
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                isSorted = false;
            }
        }
        if (isSorted) break;
    }
}

void selectionSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int min = i;
        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        swap(arr[i], arr[min]);
    }
}

void insertionSort(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
