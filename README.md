# English Introduction

This repository contains several example programs implementing the Odd-Even Sort algorithm using OpenMP, allowing for performance and implementation comparisons of different parallelization approaches.

**File Descriptions**

- `omp_OEsort0.cpp`: A serial version of the Odd-Even Sort algorithm for baseline comparison. The program initializes an array in descending order, performs n phase exchanges, and measures the execution time.

- `omp_OEsort_for.cpp`: A parallel implementation using `#pragma omp parallel` + `#pragma omp for`. The outer loop is executed in parallel by all threads, while the inner loop uses `omp for` to distribute iterations.

- `omp_OEsort_parallel_for.cpp`: A parallel implementation using `#pragma omp parallel for` to parallelize the inner loop directly for each phase, creating/reusing a thread pool for each phase to execute the iteration distribution.

**Code Features**

- Simple helper functions `swap` and `arrayinit` are used.
- Input is provided via command line arguments: the first argument specifies the number of threads (for parallel versions), and the second argument specifies the data size `n`.
- Timing is done using `omp_get_wtime()`, and output includes execution time, data size, and number of threads (for parallel versions).

**Running Instructions**
- Compilation commands:

    `g++ omp_OEsort0.cpp -fopenmp -o omp_OEsort0`

    `g++ omp_OEsort_for.cpp -fopenmp -o omp_OEsort_for`

    `g++ omp_OEsort_parallel_for.cpp -fopenmp -o omp_OEsort_parallel_for`
- Example execution:
    ./omp_OEsort0 1 100000
    ./omp_OEsort_for 4 100000
    ./omp_OEsort_parallel_for 4 100000

    The first argument specifies the number of threads (can be any value or 1 for the serial program), and the second argument specifies the array size `n`.

**Output Example**
- The program will print information similar to the following:
    Parallel Odd-Even Sort:
    Time taken:     0.012345s
    Data size:      100000
    Number of threads:  4
**Notes**
- The code uses variable-length arrays `int a[n];`, which may not be allowed in some compilers or strict standards; consider using `std::vector<int>` or dynamic allocation if necessary.
- To fairly compare parallel and serial performance, run multiple times under the same hardware and input size and take the average.



# 中文简介

本仓库包含若干用 OpenMP 实现的 "奇偶交换（Odd-Even）排序" 示例程序，便于比较不同并行化方式的性能与实现差异。

**文件说明**
- `omp_OEsort0.cpp`：串行版本的奇偶交换排序，用于基准比较。程序按降序初始化数组，再执行 n 次相位交换并计时。
- `omp_OEsort_for.cpp`：使用 `#pragma omp parallel` + `#pragma omp for` 的并行实现。外层循环由所有线程并行参与，内层使用 `omp for` 划分迭代。
- `omp_OEsort_parallel_for.cpp`：使用 `#pragma omp parallel for` 在每个相位直接并行化内层循环，每个相位创建/重用线程池并执行迭代划分。

**代码特点**
- 使用简单的 `swap` 与 `arrayinit` 辅助函数。
- 输入通过命令行传参：第一个参数为线程数（并行版使用），第二个参数为数据规模 `n`。
- 使用 `omp_get_wtime()` 计时，输出包含耗时、数据量与线程数量（并行版）。

**运行说明**
- 编译命令：

	`g++ omp_OEsort0.cpp -fopenmp -o omp_OEsort0`

	`g++ omp_OEsort_for.cpp -fopenmp -o omp_OEsort_for`

	`g++ omp_OEsort_parallel_for.cpp -fopenmp -o omp_OEsort_parallel_for`


- 运行示例：

	./omp_OEsort0 1 100000
	./omp_OEsort_for 4 100000
	./omp_OEsort_parallel_for 4 100000

	第一个参数：线程数（串行程序可传任意值或 1），第二个参数：数组大小 `n`。

**输出示例**
- 程序会打印类似如下的信息：

	并行奇偶交换排序：
	用时：		0.012345s
	数据量：	100000
	线程数量：	4

**注意事项**
- 代码中使用变长数组 `int a[n];`，在某些编译器或严格标准下可能不被允许，必要时可改为 `std::vector<int>` 或动态分配。
- 为了公平比较并行/串行性能，请在相同硬件与输入规模下多次运行并取平均。