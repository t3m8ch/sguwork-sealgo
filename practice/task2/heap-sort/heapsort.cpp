#include <utility>
#include <vector>
#include <iostream>
#include <span>
#include <chrono>

// Эта функция "просеивает" узел дерева до тех пор, пока оно не нарушит свойство кучи.
void siftDown(std::span<int> arr, int nodeIdx) {
    // Выполняем цикл до тех пор, пока у нас есть дочерние узлы
    while (2 * nodeIdx + 1 < arr.size()) {
        // Изначально выбираем левого потомка
        int childIdx = 2 * nodeIdx + 1;

        // Если правый потомок существует и больше левого, выбираем его
        if (childIdx + 1 < arr.size() && arr[childIdx + 1] > arr[childIdx]) {
            childIdx++;
        }

        // Если потомок больше родителя, меняем их местами и продолжаем просеивание
        if (arr[childIdx] > arr[nodeIdx]) {
            std::swap(arr[childIdx], arr[nodeIdx]);
            nodeIdx = childIdx;
        } else {
            // Если потомок меньше или равен родителю, то просеивание завершено
            break;
        }
    }
}

// Преобразовываем обычный массив в кучу
void heapify(std::span<int> arr) {
    for (int i = arr.size(); i >= 0; i--) {
        siftDown(arr, i);
    }
}

// Пирамидальная сортировка
void heapsort(std::vector<int>& arr) {
    std::span<int> arrSpan(arr);

    // Сначала мы преобразуем массив в кучу
    heapify(arrSpan);

    // Потом мы каждый раз будем доставать минимальный элемент кучи и ставить его в конец
    // При этом элемент, который стал первым, мы просеиваем вниз
    for (int i = 0; i < arr.size() - 1; i++) {
        std::swap(arr[0], arr[arrSpan.size() - 1]);
        arrSpan = arrSpan.first(arrSpan.size() - 1);
        siftDown(arrSpan, 0);
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
        heapsort(arr);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << duration.count() << std::endl;
    } else {
        heapsort(arr);

        // выводим массив на экран
        for (int num : arr) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}
