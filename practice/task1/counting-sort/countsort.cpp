#include <iostream>
#include <vector>

void countingSort(std::vector<int>& arr, int maxNumber) {
    // в этом векторе будет хранится количество какого-либо числа в векторе arr
    // (это число соответствует индексу numbersCount), мы сразу создаём maxNumber + 1
    // элементов внутри вектора и заполняем их нулями (+1, потому что индексация с нуля)
    std::vector<int> numbersCount(maxNumber + 1, 0);

    // подсчитываем количество каждого числа в arr
    for (int num : arr) {
        numbersCount[num]++;
    }

    // мы проходим по каждому индексу из maxNumber (значение по индексу содержит
    // количество числа под индексом в arr), смотрим значение по индексу и
    // записываем индекс в arr столько раз, сколько записано в значении по индексу
    // (то есть столько, сколько раз число под индексом встречается в arr)
    int arrIndex = 0;
    for (int i = 0; i < maxNumber + 1; i++) {
        for (int j = 0; j < numbersCount[i]; j++) {
            arr[arrIndex++] = i;
        }
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
    countingSort(arr, maxNumber);

    // выводим массив на экран
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
