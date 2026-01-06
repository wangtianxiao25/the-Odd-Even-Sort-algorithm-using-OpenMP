#include <omp.h>
#include <cstdio>
#include <stdlib.h>

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
    return;
}

void arrayinit(int* a, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
}

int main(int argc, char* argv[]) {
    int thread_num = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);
    int a[n];
    arrayinit(a, n);
    int i, phase;

    // 计时
    double start, end;
    start = omp_get_wtime();


    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
#           pragma omp parallel for num_threads(thread_num) \
                default(none) shared(a, n) private(i)
            for (i = 1; i < n; i += 2) {
                if (a[i - 1] > a[i]) swap(a[i - 1], a[i]);
            }
        } else {
#           pragma omp parallel for num_threads(thread_num) \
                default(none) shared(a, n) private(i)
            for (i = 1; i < n - 1; i += 2) {
                if (a[i + 1] > a[i]) swap(a[i + 1], a[i]);
            }
        }
    }

    end = omp_get_wtime();
    printf("并行奇偶交换排序：\n用时：\t\t%lfs\n数据量：\t%d\n线程数量：\t%d\n", end - start, n, thread_num);

    return 0;
}
