import random
def sift_down(a, v, n):
    while 2*v <= n:
        u = 2*v
        if 2*v+1 <= n and a[2*v+1] < a[2*v]:
            u = 2*v+1
        if a[u] < a[v]:
            a[u], a[v] = a[v], a[u]
            v = u
        else:
            break


def heapify(a, n):
    # n..=1
    for i in range(n, 0, -1):
        sift_down(a, i, n)


def sort(a, n):
    print(f"Array: {a}")

    heapify(a, n)
    print(f"Heapify: {a}")

    for i in range(1, n+1):
        print(f"Step {i}: {a}")
        # a[1] = a[n]
        a[1], a[n] = a[n], a[1]
        n -= 1
        sift_down(a, 1, n)


n = 100
a = [0] + [random.randint(0, 10000) for _ in range(n)]

sort(a, n)
print(a)
