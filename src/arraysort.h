//
// Created by Kadyn-Jai Pearce on 6/3/2026.
//

#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

namespace ArraySort {
    extern std::mutex dataMutex;
    extern std::atomic<bool> isSorting;

    void bubbleSort(std::vector<float> &arr);

    void selectionSort(std::vector<float> &arr);

    void insertionSort(std::vector<float> &arr);

    void mergeSort(std::vector<float> &arr);

    void quickSort(std::vector<float> &arr);
}

#ifndef ALGO_VISUAL_ARRAYSORT_H
#define ALGO_VISUAL_ARRAYSORT_H

#endif //ALGO_VISUAL_ARRAYSORT_H