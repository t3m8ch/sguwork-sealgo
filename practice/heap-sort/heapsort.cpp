#include <utility>
#include <vector>
#include <iostream>

void siftDown(std::vector<int>& arr, int nodeIdx, int n) {
    while (2 * nodeIdx + 1 < n) {
        int childIdx = 2 * nodeIdx + 1;
        if (childIdx + 1 < n && arr[childIdx + 1] > arr[childIdx]) {
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

void heapify(std::vector<int>& arr, int n) {
    for (int i = n; i >= 0; i--) {
        siftDown(arr, i, n);
    }
}

void heapsort(std::vector<int>& arr, int n) {
    heapify(arr, n);
    int curr_n = n;
    for (int i = 0; i < n - 1; i++) {
        std::swap(arr[0], arr[--curr_n]);
        siftDown(arr, 0, curr_n);
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
    heapsort(arr, arr.size());

    // выводим массив на экран
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
