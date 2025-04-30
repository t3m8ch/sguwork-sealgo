#include <iostream>
#include <span>
#include <vector>
#include <chrono>

// Быстрая сортировка является алгоритмом, использующий операцию сравнения,
// поэтому мы наложим на шаблон ограничение, что он должен перегружать операторы сравнения
template<class T>
requires requires (const T& a, const T& b) {
   { a < b } -> std::convertible_to<bool>;
   { a > b } -> std::convertible_to<bool>;
}
void quicksort(std::span<T> arr) {
    // В быстрой сортировке мы рекурсивно разбиваем массив на две части.
    // Условием выхода из рекурсии будет момент, когда массив станет пустым.
    if (arr.size() <= 1) {
        return;
    }

    // В качестве опорного элемента выбираем средний элемент массива
    T pivot = arr[arr.size() / 2];

    size_t left = 0;
    size_t right = arr.size() - 1;

    while (left <= right) {
        // Мы начинаем идти слева направо до момента, пока элемент не станет меньше опорного.
        while (arr[left] < pivot) {
            ++left;
        }

        // Потом мы идём справа налево до момента, пока элемент не станет больше опорного.
        while (arr[right] > pivot) {
            --right;
        }

        // Если левый индекс меньше правого, то это значит, что необходимо поменять
        // местами левый и правый элементы.
        if (left <= right) {
            std::swap(arr[left], arr[right]);
            ++left;
            --right;
        }

        // Мы это делаем до тех пор, пока левый индекс не станет больше правого.
    }

    // Если мы сдвинулись вправо хотя бы на один элемент,
    // мы запускаем рекурсию для левой части массива.
    if (right > 0) {
        quicksort(arr.subspan(0, right + 1));
    }

    // Аналогично для правой части массива.
    if (left < arr.size()) {
        quicksort(arr.subspan(left));
    }
}

int main(int argc, char* argv[]) {
    bool benchmark_mode = false;
    if (argc > 1 && std::string(argv[1]) == "--benchmark") {
        benchmark_mode = true;
    }

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
    if (benchmark_mode) {
        auto start = std::chrono::high_resolution_clock::now();
        quicksort(std::span<int>(arr));
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << duration.count() << std::endl;
    } else {
        quicksort(std::span<int>(arr));

        // выводим массив на экран
        for (int num : arr) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}
