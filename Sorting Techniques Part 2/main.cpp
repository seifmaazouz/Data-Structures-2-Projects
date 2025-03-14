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

// Quick Sort
void quickSort(int arr[], int low, int high);
int partition(int arr[], int low, int high);

// Part 2 - Hybrid Merge Sort (Merge Sort with Insertion Sort) and Quick Select (Quick Sort with Selection)
void hybridMergeSort(int arr[], int left, int right,int THRESHOLD);
int quickSelect(int arr[], int low, int high, int k);

int main() {
    // Test the Quick Select function
    int arr[] = {3, 41, 16, 25, 63, 52, 40};
    int size = sizeof(arr) / sizeof(arr[0]);
    int k = 3; // Find the 3rd smallest element

    int kthSmallest = quickSelect(arr, 0, size - 1, k);
    printf("%dth smallest element is %d\n\n", k, kthSmallest);

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

int quickSelect(int arr[], int low, int high, int k){

    if(low<=high){
        int pivot=partition(arr,low,high);
        //checks if pivot found equals k
        if(pivot==k-1)
            return arr[pivot];
        //checks if k less than pivot takes left subarray
        if(pivot>k-1)
            return quickSelect(arr,low,pivot-1,k);
        //takes the right subarray (bigger than pivot)
        return quickSelect(arr,pivot+1,high,k);
    }
    //error case
    return -1;

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

    // Quick Sort
    memcpy(arrCopy, arr, size * sizeof(int));
    start = clock();
    quickSort(arrCopy, 0, size - 1);
    end = clock();
    cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;
    printf("Running time for Quick Sort is %f ms\n", cpu_time_used);

    // Hybrid Merge Sort
    memcpy(arrCopy, arr, size * sizeof(int));
    start = clock();
    hybridMergeSort(arrCopy, 0, size - 1,32);
    end = clock();
    cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;
    printf("Running time for Hybrid Merge Sort is %f ms\n", cpu_time_used);

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

void quickSort(int arr[], int low,int high) {
    if (low < high) {
        int pivot = partition(arr,low,high);
        quickSort(arr,low,pivot-1);
        quickSort(arr,pivot+1,high);
    }
}

int partition(int arr[], int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(arr[randomIndex], arr[high]);
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i += 1;
            swap(arr[i], arr[j]);
        }
    }
    i += 1;
    swap(arr[i], arr[high]);
    return i;
}

void hybridMergeSort(int arr[], int left, int right,int THRESHOLD) {
    if (right - left + 1 <= THRESHOLD) {
        insertionSort(arr + left, right - left + 1);
        return;
    }
    if (left < right) {
        int mid = left + (right - left) / 2;
        hybridMergeSort(arr, left, mid,THRESHOLD);
        hybridMergeSort(arr, mid + 1, right,THRESHOLD);
        merge(arr, left, mid, right);
    }
}
