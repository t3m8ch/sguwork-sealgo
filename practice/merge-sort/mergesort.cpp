#include <algorithm>
#include <vector>
#include <iostream>

// Сортировка слиянием является алгоритмом, использующий операцию сравнения,
// поэтому мы наложим на шаблон ограничение, что он должен перегружать операторы сравнения
template<class T>
requires requires (const T& a, const T& b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
}
void merge(
    std::vector<T>& inputArr,
    std::vector<T>& outputArr,
    size_t left,
    size_t mid,
    size_t right
) {
    size_t it1 = 0;
    size_t it2 = 0;

    outputArr.clear();
    outputArr.resize(right - left);

    while ((left + it1 < mid) && (mid + it2 < right)) {
        if (inputArr[left + it1] < inputArr[mid + it2]) {
            outputArr[it1 + it2] = inputArr[left + it1];
            it1++;
        } else {
            outputArr[it1 + it2] = inputArr[mid + it2];
            it2++;
        }
    }

    while (left + it1 < mid) {
        std::cout << outputArr.size() << std::endl;
        outputArr[it1 + it2] = inputArr[left + it1];
        it1++;
    }

    while (mid + it2 < right) {
        outputArr[it1 + it2] = inputArr[mid + it2];
        it2++;
    }

    for (size_t i = 0; i <= it1 + it2; i++) {
        inputArr[left + i] = outputArr[i];
    }
}

template<class T>
requires requires (const T& a, const T& b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
}
void mergeSort(std::vector<T>& arr) {
    std::vector<T> mergeResult;
    mergeResult.reserve(arr.size());
    for (size_t i = 0; i <= arr.size(); i *= i) {
        for (size_t j = 0; j <= arr.size() - i; i += 2 * i) {
            merge(arr, mergeResult, j, j + 1, std::min(j + 2 * i, arr.size()));
        }
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
    mergeSort(arr);

    // выводим массив на экран
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
