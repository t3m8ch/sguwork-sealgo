#include <iostream>
#include <vector>

void merge(std::vector<int>& arr, size_t left, size_t mid, size_t right) {
    std::vector<int> leftArr(mid - left + 1);
    std::vector<int> rightArr(right - mid);

    for (size_t i = 0; i < leftArr.size(); i++) {
        leftArr[i] = arr[left + i];
    }

    for (size_t i = 0; i < rightArr.size(); i++) {
        rightArr[i] = arr[mid + 1 + i];
    }

    size_t i = 0;
    size_t j = 0;
    size_t k = left;

    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < leftArr.size()) {
        arr[k++] = leftArr[i++];
    }

    while (j < rightArr.size()) {
        arr[k++] = rightArr[j++];
    }
}

void mergesort(std::vector<int>& arr, size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    size_t mid = left + (right - left) / 2;

    mergesort(arr, left, mid);
    mergesort(arr, mid + 1, right);

    merge(arr, left, mid, right);
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
    mergesort(arr, 0, arr.size() - 1);

    // выводим массив на экран
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
