#include <utility>
#include <vector>
#include <iostream>
#include <span>

void siftDown(std::span<int> arr, int nodeIdx) {
    while (2 * nodeIdx + 1 < arr.size()) {
        int childIdx = 2 * nodeIdx + 1;
        if (childIdx + 1 < arr.size() && arr[childIdx + 1] > arr[childIdx]) {
            childIdx++;
        }

        if (arr[childIdx] > arr[nodeIdx]) {
            std::swap(arr[childIdx], arr[nodeIdx]);
            nodeIdx = childIdx;
        } else {
            break;
        }
    }
}

void heapify(std::span<int> arr) {
    for (int i = arr.size(); i >= 0; i--) {
        siftDown(arr, i);
    }
}

void heapsort(std::vector<int>& arr) {
    std::span<int> arrSpan(arr);
    heapify(arrSpan);
    for (int i = 0; i < arr.size() - 1; i++) {
        std::swap(arr[0], arr[arrSpan.size() - 1]);
        arrSpan = arrSpan.first(arrSpan.size() - 1);
        siftDown(arrSpan, 0);
    }
}

int main() {
    int n;  // кол-во элементов
    std::cin >> n;

    // указываем capacity = n, тем самым сразу аллоцировав место под n элеметов
    std::vector<int> arr;
    arr.reserve(n);

    // заполняем массив
    for (int i = 0; i < n; i++) {
        int num;
        std::cin >> num;
        arr.push_back(num);
    }

    // сортируем массив
    heapsort(arr);

    // выводим массив на экран
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
