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

void ArraySort::mergeSort(std::vector<float> &arr, float delay) {
    isSorting = false;

}

void ArraySort::quickSort(std::vector<float> &arr, float delay) {
    isSorting = false;
}
