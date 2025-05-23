#include <iostream>
#include <vector>
#include <chrono>

// В основе сортировки слиянием находится, как ни странно, слияние.
// Это операция, которая объединяет два отсортированных массива в один
// отсортированный массив.
template<class T>
requires requires (const T& a, const T& b) {
   { a < b } -> std::convertible_to<bool>;
   { a > b } -> std::convertible_to<bool>;
}
void merge(
    // Массив, слева содержащий левый подмассив, справа содержащий правый.
    std::vector<T>& arr,

    // Границы левого и правого подмассивов определяются тремя аргументами ниже:
    // [left; mid) и [mid; right)
    size_t left,
    size_t mid,
    size_t right,

    // Слияние мы будем проводить в вектор arr, принимаемый в качестве первого аргумента.
    // Однако во время записи в arr мы можем потерять исходное положение элементов в подмассивах,
    // поэтому нам необходимо их сохранить в векторах leftArr и rightArr.
    //
    // Эти вектора можно создать и внутри функции merge, однако в таком случае
    // они будут реаллоцироваться на каждый вызов функции merge.
    // Поскольку merge ниже вызывается в рекурсивной функции, каждый раз разбивающий массив
    // на две половины до тех пор, пока полученные массивы не окажутся пустыми, то
    // количество вызовов merge составит O(log(n)), а это значит мы будет обращаться к ОС
    // для аллокации памяти в куче O(log(n)) раза. Очевидно, что системный вызов для аллокации
    // памяти в куче -- это дорого, надо это минимизировать.
    //
    // Вместо этого мы можем один раз аллоцировать два вектора, зарезервировав перед их
    // использованием место под [n / 2] и [n / 2] + 1 элементов соответственно (capacity) при
    // помощи метода reserve, а их size мы будем менять при помощи метода resize на каждый вызов.
    // Таким образом мы будем как-будто работать с векторами, имеющими нужный нам размер, но на
    // самом деле один раз мы аллоцируем место для самых больших подмассивов и это место
    // переиспользуем на каждый вызов merge, тем самым у нас будет лишь O(1) обращения к ОС
    // для аллокации памяти.
    std::vector<T>& leftArr,
    std::vector<T>& rightArr
) {
    leftArr.resize(mid - left + 1);
    rightArr.resize(right - mid);

    // Копируем элементы в левый и правый подмассивы из arr.
    for (size_t i = 0; i < leftArr.size(); i++) {
        leftArr[i] = arr[left + i];
    }

    for (size_t i = 0; i < rightArr.size(); i++) {
        rightArr[i] = arr[mid + 1 + i];
    }

    size_t i = 0;     // Текущий элемент левого подмассива
    size_t j = 0;     // Текущий элемент правого подмассива
    size_t k = left;  // Текущий элемент массива arr

    // Производим операцию слияния, пока один из указателей в левом
    // или правом подмассиве не дошёл до последнего элемента.
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) {
            // Если текущий элемент левого подмассива не больше текущего элемента
            // правого подмассива, то мы кладём в arr текущий элемент левого подмассива
            // и переходим к следующему элементу левого подмассива.
            arr[k++] = leftArr[i++];
        } else {
            // В противном случае мы кладём в arr текущий элемент правого подмассива
            // и переходим к следующему элементу правого подмассива.
            arr[k++] = rightArr[j++];
        }
    }

    // Если в левом подмассиве мы не дошли до конца, значит в нём остались
    // элементы, которые мы не переложили. Перекладываем.
    while (i < leftArr.size()) {
        arr[k++] = leftArr[i++];
    }

    // Аналогично для правого подмассива.
    while (j < rightArr.size()) {
        arr[k++] = rightArr[j++];
    }
}

// Это перегрузка функции mergesort, которая будет вызываться рекурсивно.
// Простым работягам-программистам следует применять перегрузку, описанную ниже)
template<class T>
requires requires (const T& a, const T& b) {
   { a < b } -> std::convertible_to<bool>;
   { a > b } -> std::convertible_to<bool>;
}
void mergesort(
    // Массив, который мы сортируем.
    std::vector<T>& arr,

    // Левая и правая граница подмассива.
    size_t left,
    size_t right,

    // Смотреть описание аналогичных аргументов в функции merge.
    std::vector<T>& leftArr,
    std::vector<T>& rightArr
) {
    if (left >= right) {
        return;
    }

    // Разделяем два подмассива на две равные половинки
    // (или почти равные, если количество элементов arr нечётное).
    size_t mid = left + (right - left) / 2;

    // Рекурсивно сортируем левую и правую половинки.
    mergesort(arr, left, mid, leftArr, rightArr);
    mergesort(arr, mid + 1, right, leftArr, rightArr);

    // Сливаем воедино две отсортированные половинки.
    merge(arr, left, mid, right, leftArr, rightArr);
}

// Это перегрузка функции -- простой чилловый парень: он просто принимает
// на вход вектор и сортирует его, без всяких заморочек, все заморочки с
// аллоцированием дополнительных векторов для двух подмассивов или с
// передачей дополнительных аргументов для того, чтобы рекурсия работала,
// эта перегрузка функции инкапсулирует за собой. Настоящий герой!
template<class T>
requires requires (const T& a, const T& b) {
   { a < b } -> std::convertible_to<bool>;
   { a > b } -> std::convertible_to<bool>;
}
void mergesort(std::vector<T>& arr) {
    // Смотреть описание последних двух аргументов функции merge.
    std::vector<T> leftArr;
    leftArr.reserve(arr.size() / 2);

    std::vector<T> rightArr;
    rightArr.reserve(arr.size() / 2 + 1);

    // сортируем массив
    mergesort(arr, 0, arr.size() - 1, leftArr, rightArr);
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
        mergesort(arr);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << duration.count() << std::endl;
    } else {
        mergesort(arr);

        // выводим массив на экран
        for (int num : arr) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}
