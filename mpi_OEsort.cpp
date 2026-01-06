#include <mpi.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

// 初始化数组（仅在主进程调用）
void arrayinit(std::vector<int>& a, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // 当前进程编号
    MPI_Comm_size(MPI_COMM_WORLD, &size); // 总进程数

    if (argc < 2) {
        if (rank == 0) printf("用法: mpirun -np <进程数> ./a.out <数据量>\n");
        MPI_Finalize();
        return 0;
    }

    int n = strtol(argv[1], NULL, 10);

    // 主进程初始化数组
    std::vector<int> a;
    if (rank == 0) {
        a.resize(n);
        arrayinit(a, n);
    }

    // 计算每个进程的数据量（尽量均衡）
    std::vector<int> counts(size), displs(size);
    int base = n / size;
    int remainder = n % size;
    for (int i = 0; i < size; i++) {
        counts[i] = base + (i < remainder ? 1 : 0);
    }
    displs[0] = 0;
    for (int i = 1; i < size; i++) {
        displs[i] = displs[i-1] + counts[i-1];
    }

    int local_n = counts[rank];
    std::vector<int> local_a(local_n);

    // 分发数据
    MPI_Scatterv(a.data(), counts.data(), displs.data(), MPI_INT,
                 local_a.data(), local_n, MPI_INT,
                 0, MPI_COMM_WORLD);

    double start = MPI_Wtime();

    // 奇偶交换排序
    for (int phase = 0; phase < n; phase++) {
        // 局部交换
        if (phase % 2 == 0) {
            for (int i = 1; i < local_n; i += 2) {
                if (local_a[i - 1] > local_a[i]) std::swap(local_a[i - 1], local_a[i]);
            }
        } else {
            for (int i = 1; i < local_n - 1; i += 2) {
                if (local_a[i] > local_a[i + 1]) std::swap(local_a[i], local_a[i + 1]);
            }
        }

        // 边界交换（对称）
        if (phase % 2 == 0) {
            // 偶数阶段：偶数 rank 与右邻居交换，奇数 rank 与左邻居交换
            if (rank % 2 == 0 && rank < size - 1) {
                int send_val = local_a[local_n - 1], recv_val;
                MPI_Sendrecv(&send_val, 1, MPI_INT, rank + 1, 0,
                             &recv_val, 1, MPI_INT, rank + 1, 0,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (send_val > recv_val) local_a[local_n - 1] = recv_val;
            } else if (rank % 2 == 1 && rank > 0) {
                int send_val = local_a[0], recv_val;
                MPI_Sendrecv(&send_val, 1, MPI_INT, rank - 1, 0,
                             &recv_val, 1, MPI_INT, rank - 1, 0,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (recv_val > send_val) local_a[0] = recv_val;
            }
        } else {
            // 奇数阶段：偶数 rank 与左邻居交换，奇数 rank 与右邻居交换
            if (rank % 2 == 0 && rank > 0) {
                int send_val = local_a[0], recv_val;
                MPI_Sendrecv(&send_val, 1, MPI_INT, rank - 1, 0,
                             &recv_val, 1, MPI_INT, rank - 1, 0,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (recv_val > send_val) local_a[0] = recv_val;
            } else if (rank % 2 == 1 && rank < size - 1) {
                int send_val = local_a[local_n - 1], recv_val;
                MPI_Sendrecv(&send_val, 1, MPI_INT, rank + 1, 0,
                             &recv_val, 1, MPI_INT, rank + 1, 0,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (send_val > recv_val) local_a[local_n - 1] = recv_val;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD); // 同步
    }

    // 收集结果
    MPI_Gatherv(local_a.data(), local_n, MPI_INT,
                a.data(), counts.data(), displs.data(), MPI_INT,
                0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("MPI并行奇偶交换排序：\n用时：\t\t%lfs\n数据量：\t%d\n进程数量：\t%d\n",
               end - start, n, size);

        // 验证排序结果
        bool sorted = true;
        for (int i = 1; i < n; i++) {
            if (a[i - 1] > a[i]) { sorted = false; break; }
        }
        printf("排序结果：%s\n", sorted ? "正确" : "错误");
    }

    MPI_Finalize();
    return 0;
}
