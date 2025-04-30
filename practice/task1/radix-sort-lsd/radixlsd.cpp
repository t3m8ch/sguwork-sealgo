#include <vector>
#include <array>
#include <iostream>

void radixLSDSort(std::vector<int>& arr, int maxNumber) {
    // индекс массива векторов -- это цифра, которая содержится в разряде числа,
    // этот разряд соответствует множителю multiplier
    std::array<std::vector<int>, 10> buckets;
    int multiplier = 1;

    // цикл работает до тех пор, пока проверяемый разряд не станет старше, чем
    // максимально страший разряд чисел из arr
    while (maxNumber >= multiplier) {
        // добавляем числа из arr в соответствующий вектор из массива векторов
        for (int num : arr) {
            buckets[num / multiplier % 10].push_back(num);
        }

        // переписываем в arr отсортированную по текущему разряду
        // последовательность чисел
        int arrIndex = 0;
        for (auto& bucket : buckets) {
            for (int num : bucket) {
                arr[arrIndex++] = num;
            }

            // очищаем вектор, чтобы его можно было переиспользовать
            // на следующей итерации
            bucket.clear();
        }

        // переходим к следующему разряду
        multiplier *= 10;
    }
}

int main() {
    int n;  // кол-во элементов
    std::cin >> n;

    // указываем capacity = n, тем самым сразу аллоцировав место под n элеметов
    std::vector<int> arr;
    arr.reserve(n);

    // при вводе чисел мы сразу найдём максимальное число, чтобы аллоцировать
    // вектор нужной размерности
    int maxNumber = -1;

    // заполняем массив
    for (int i = 0; i < n; i++) {
        int num;
        std::cin >> num;
        arr.push_back(num);

        if (num > maxNumber) {
            maxNumber = num;
        }
    }

    // сортируем массив
    radixLSDSort(arr, maxNumber);

    // выводим массив на экран
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
