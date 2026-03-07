//
// Created by kadyn on 7/03/2026.
//

#include "arraysort.h"
#include <mutex>
#include <atomic>
#include <thread>

std::mutex ArraySort::dataMutex;
std::atomic<bool> ArraySort::isSorting{false};

void ArraySort::bubbleSort(std::vector<float> &arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = i + 1; j < arr.size(); j++) {

            {
                std::lock_guard<std::mutex> lock(dataMutex);
                if (arr[i] > arr[j]) {
                    std::swap(arr[i], arr[j]);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    isSorting = false;
}

void ArraySort::insertionSort(std::vector<float> &arr) {

}

void ArraySort::selectionSort(std::vector<float> &arr) {

}

void ArraySort::mergeSort(std::vector<float> &arr) {

}

void ArraySort::quickSort(std::vector<float> &arr) {

}
