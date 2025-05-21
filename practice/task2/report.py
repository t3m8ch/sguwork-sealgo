import os
import subprocess
import random
import datetime
import shutil
import matplotlib.pyplot as plt

# Запрос данных пользователя
name = input("Введите ФИО: ")
report_title = input("Введите название отчёта: ")
group = input("Введите номер группы: ")
current_year = datetime.datetime.now().year

# Создание директории для отчётов
report_dir = "./report"
os.makedirs(report_dir, exist_ok=True)

# Компиляция исходных файлов
print("\nКомпиляция алгоритмов сортировки...")
algorithms = [
    ('quick-sort', 'quicksort.cpp'),
    ('merge-sort', 'mergesort.cpp'),
    ('heap-sort', 'heapsort.cpp')
]

for algo_dir, src_file in algorithms:
    src_path = os.path.join(algo_dir, src_file)
    exe_path = os.path.join(algo_dir, "a.out")
    compile_cmd = [
        "g++", "-O3", "-std=c++20", src_path, "-o", exe_path
    ]
    print(f"Компиляция {src_path}...")
    result = subprocess.run(compile_cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Ошибка компиляции {src_path}:\n{result.stderr}")
        exit(1)

# Тестирование алгоритмов
print("\nЗапуск тестирования...")
n_values = [50000, 100000, 500000, 1000000]
results = {
    'Quick sort': [],
    'Merge sort': [],
    'Heap sort': []
}

for algo_dir, algo_name in zip(['quick-sort', 'merge-sort', 'heap-sort'], results.keys()):
    exe_path = os.path.join(algo_dir, "a.out")
    print(f"\nТестирование {algo_name}...")

    for n in n_values:
        times = []
        print(f"  n = {n}: ", end="", flush=True)

        for i in range(5):
            # Генерация случайных данных
            elements = [random.randint(-1000000, 1000000) for _ in range(n)]
            input_data = f"{n} " + " ".join(map(str, elements))

            # Запуск процесса
            proc = subprocess.Popen(
                [exe_path, '--benchmark'],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
            stdout, stderr = proc.communicate(input=input_data.encode())

            if proc.returncode != 0:
                print(f"\nОшибка выполнения: {stderr.decode()}")
                exit(1)

            # Получение времени выполнения
            try:
                time_us = int(stdout.decode().strip())
                times.append(time_us)
                print(f"{i+1} ", end="", flush=True)
            except ValueError:
                print(f"\nНеверный вывод программы: {stdout.decode()}")
                exit(1)

        avg_time = sum(times) / len(times)
        results[algo_name].append(avg_time)
        print(f"\n  Среднее время: {avg_time:.2f} мкс")

print("\nКопирование conf.typ...")
src_conf = "../conf.typ"  # Путь к исходному файлу
dst_conf = os.path.join(report_dir, "conf.typ")  # Целевой путь

try:
    shutil.copy(src_conf, dst_conf)
    print(f"Файл {src_conf} успешно скопирован в {dst_conf}")
except FileNotFoundError:
    print(f"Ошибка: файл {src_conf} не найден в корне проекта!")
    exit(1)
except Exception as e:
    print(f"Ошибка копирования: {str(e)}")
    exit(1)

# Генерация Typst отчёта
print("\nГенерация отчёта...")
typst_content = f"""#import "./conf.typ": conf

#show: conf.with(
  meta: (
    title: "{report_title}",
    author: "{name}",
    group: {group},
    city: "Саратов",
    year: {current_year},
  )
)

= Таблица

#table(
  columns: 5,
  table.cell(rowspan: 2, [Алгоритм сортировки]),
  table.cell(colspan: 4, [Количество элементов в массиве]),
  [50 000], [100 000], [500 000], [1 000 000],
  [Quick sort], [{[round(t) for t in results['Quick sort']][0]} $mu s$], [{[round(t) for t in results['Quick sort']][1]} $mu s$], [{[round(t) for t in results['Quick sort']][2]} $mu s$], [{[round(t) for t in results['Quick sort']][3]} $mu s$],
  [Merge sort], [{[round(t) for t in results['Merge sort']][0]} $mu s$], [{[round(t) for t in results['Merge sort']][1]} $mu s$], [{[round(t) for t in results['Merge sort']][2]} $mu s$], [{[round(t) for t in results['Merge sort']][3]} $mu s$],
  [Heap sort], [{[round(t) for t in results['Heap sort']][0]} $mu s$], [{[round(t) for t in results['Heap sort']][1]} $mu s$], [{[round(t) for t in results['Heap sort']][2]} $mu s$], [{[round(t) for t in results['Heap sort']][3]} $mu s$]
)

= Сравнительный график времени работы алгоритмов

#image("sorting_comparison.png")
"""

with open(os.path.join(report_dir, "main.typ"), "w") as f:
    f.write(typst_content)

# Построение графиков
print("\nПостроение графиков...")
plt.style.use('ggplot')

# Создаем одну фигуру для всех алгоритмов
plt.figure(figsize=(10, 6))

# Отображаем данные каждого алгоритма на одном графике
markers = ['o', 's', '^']  # Разные маркеры для каждого алгоритма
colors = ['#1f77b4', '#ff7f0e', '#2ca02c']  # Разные цвета для каждого алгоритма

for i, algo_name in enumerate(results.keys()):
    plt.plot(n_values, results[algo_name], marker=markers[i], color=colors[i],
             linewidth=2, markersize=8, label=algo_name)

plt.title('Сравнение производительности алгоритмов сортировки', fontsize=14)
plt.xlabel('Количество элементов', fontsize=12)
plt.ylabel('Время выполнения (мкс)', fontsize=12)
plt.xscale('log')
plt.yscale('log')
plt.xticks(n_values, labels=[f"{n//1000}K" if n < 1e6 else "1M" for n in n_values], fontsize=10)
plt.yticks(fontsize=10)
plt.grid(True, which='both', linestyle='--', alpha=0.7)
plt.legend(fontsize=12)
plt.tight_layout()
plt.savefig(os.path.join(report_dir, "sorting_comparison.png"), dpi=300)
plt.close()

# Компиляция Typst в PDF
print("\nКомпиляция PDF...")
try:
    subprocess.run(["typst", "compile", "./report/main.typ"], check=True)
    print("\nОтчёт успешно сгенерирован!")
except subprocess.CalledProcessError as e:
    print(f"\nОшибка компиляции Typst: {e}")
except FileNotFoundError:
    print("\nОшибка: typst не найден. Убедитесь, что Typst установлен и доступен в PATH.")
