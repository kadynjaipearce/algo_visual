#include <vector>
#include <mutex>
#include <atomic>

namespace ArraySort {
    extern std::mutex dataMutex;
    extern std::atomic<bool> isSorting;

    void bubbleSort(std::vector<float> &arr, float delay);

    void selectionSort(std::vector<float> &arr, float delay);

    void insertionSort(std::vector<float> &arr, float delay);

    void brickSort(std::vector<float> &arr, float delay);

    void mergeSort(std::vector<float> &arr, float delay);

    void quickSort(std::vector<float> &arr, float delay);
}

#ifndef ALGO_VISUAL_ARRAYSORT_H
#define ALGO_VISUAL_ARRAYSORT_H

#endif //ALGO_VISUAL_ARRAYSORT_H