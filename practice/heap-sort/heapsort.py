import random


def sift_down(a, v, n):
    while 2*v+1 < n:
        u = 2*v+1
        if 2*v+2 < n and a[2*v+2] < a[2*v+1]:
            u = 2*v+2
        if a[u] < a[v]:
            a[u], a[v] = a[v], a[u]
            v = u
        else:
            break


def heapify(a, n):
    # n..=1
    for i in range(n-1, -1, -1):
        sift_down(a, i, n)


def sort(a, n):
    print(f"Array: {a}")

    heapify(a, n)
    print(f"Heapify: {a}")

    for i in range(1, n+1):
        print(f"Step {i}: {a}")
        # a[1] = a[n]
        a[0], a[n-1] = a[n-1], a[0]
        n -= 1
        sift_down(a, 0, n)


n = 100
a = [random.randint(0, 10000) for _ in range(n)]

sort(a, n)
print(a)
