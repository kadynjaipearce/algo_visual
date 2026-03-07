#include "arraysort.h"
#include <mutex>
#include <atomic>
#include <thread>

std::mutex ArraySort::dataMutex;
std::atomic<bool> ArraySort::isSorting{false};

void ArraySort::bubbleSort(std::vector<float> &arr, float delay) {
    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = i + 1; j < arr.size(); j++) {

            {
                std::lock_guard<std::mutex> lock(dataMutex);
                if (arr[i] > arr[j]) {
                    std::swap(arr[i], arr[j]);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 100)));
        }
    }

    isSorting = false;
}

void ArraySort::insertionSort(std::vector<float> &arr, float delay) {
    for (size_t i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key && isSorting) {
            {
                std::lock_guard<std::mutex> lock(dataMutex);
                arr[j + 1] = arr[j];
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 100)));
            j--;
        }

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            arr[j + 1] = key;
        }
    }

    isSorting = false;
}

void ArraySort::selectionSort(std::vector<float> &arr, float delay) {
    for (int i = 0; i < arr.size(); i++) {
        int minIndex = i;

        for (int j = i + 1; j < arr.size(); j++) {
            {
                std::lock_guard<std::mutex> lock(dataMutex);
                if (arr[j] < arr[minIndex]) {
                    minIndex = j;
                }
            }
        }


        {
            std::lock_guard<std::mutex> lock(dataMutex);
            std::swap(arr[i], arr[minIndex]);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 100)));
    }
    isSorting = false;
}

void Merge(std::vector<float>& data, int left, int mid, int right, float delay) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = data[left + i];
    for (int j = 0; j < n2; j++) R[j] = data[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Add isSorting check to every loop
    while (i < n1 && j < n2 && ArraySort::isSorting) {
        {
            std::lock_guard<std::mutex> lock(ArraySort::dataMutex);
            if (L[i] <= R[j]) {
                data[k] = L[i];
                i++; // Only increment i
            } else {
                data[k] = R[j];
                j++; // Only increment j
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 10)));
        k++;
    }

    // ALWAYS lock when touching 'data'
    while (i < n1 && ArraySort::isSorting) {
        {
            std::lock_guard<std::mutex> lock(ArraySort::dataMutex);
            data[k] = L[i];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 10)));
        i++; k++;
    }

    while (j < n2 && ArraySort::isSorting) {
        {
            std::lock_guard<std::mutex> lock(ArraySort::dataMutex);
            data[k] = R[j];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay * 10)));
        j++; k++;
    }
}

void MergeSortInternal(std::vector<float>& data, int left, int right, float delay) {
    if (left < right && ArraySort::isSorting) {
        int mid = left + (right - left) / 2;
        MergeSortInternal(data, left, mid, delay);
        MergeSortInternal(data, mid + 1, right, delay);
        Merge(data, left, mid, right, delay);
    }
}

void ArraySort::mergeSort(std::vector<float> &arr, float delay) {
    MergeSortInternal(arr, 0, (int)arr.size() - 1, delay);

    ArraySort::isSorting = false;
}

void ArraySort::quickSort(std::vector<float> &arr, float delay) {
    isSorting = false;
}
