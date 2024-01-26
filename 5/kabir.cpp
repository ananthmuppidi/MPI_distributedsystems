#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_NODES 1000
#define INF INT_MAX

void readMatrix(int N, int matrix[MAX_NODES][MAX_NODES], int rank) {
    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                scanf("%d", &matrix[i][j]);
                if (matrix[i][j] == -1) {
                    matrix[i][j] = INF;
                }
            }
        }
    }
}

// void parallelFloydWarshall(int N, int matrix[MAX_NODES][MAX_NODES], int rank, int size) {
//     int row[MAX_NODES], col[MAX_NODES];

//     for (int k = 0; k < N; ++k) {
//         // Correctly identify the broadcasting process
//         int root = k % size;

//         // Copy the k-th row and column if this process is the root
//         if (rank == root) {
//             for (int i = 0; i < N; ++i) {
//                 row[i] = matrix[k][i];
//                 col[i] = matrix[i][k];
//             }
//         }

//         MPI_Bcast(row, N, MPI_INT, root, MPI_COMM_WORLD);
//         MPI_Bcast(col, N, MPI_INT, root, MPI_COMM_WORLD);

//         // Compute the updated distances
//         for (int i = rank; i < N; i += size) {
//             for (int j = 0; j < N; ++j) {
//                 if (matrix[i][k] != INF && matrix[k][j] != INF) {
//                     int newDist = matrix[i][k] + matrix[k][j];
//                     if (newDist < matrix[i][j]) {
//                         matrix[i][j] = newDist;
//                     }
//                 }
//             }
//         }
//         MPI_Barrier(MPI_COMM_WORLD); // Synchronize before next iteration
//     }
// }

void parallelFloydWarshall(int N, int matrix[MAX_NODES][MAX_NODES], int rank, int size) {
    int kthRow[MAX_NODES];

    for (int k = 0; k < N; ++k) {
        // The process that owns the k-th row broadcasts it to everyone else
        int owner = k / (N / size);  // Assuming N is divisible by size
        if (rank == owner) {
            for (int j = 0; j < N; ++j) {
                kthRow[j] = matrix[k][j];
            }
        }
        MPI_Bcast(kthRow, N, MPI_INT, owner, MPI_COMM_WORLD);

        // Each process updates its own set of rows
        int startRow = rank * (N / size);
        int endRow = (rank + 1) * (N / size);
        for (int i = startRow; i < endRow; ++i) {
            for (int j = 0; j < N; ++j) {
                if (matrix[i][k] != INF && kthRow[j] != INF) {
                    int newDist = matrix[i][k] + kthRow[j];
                    if (newDist < matrix[i][j]) {
                        matrix[i][j] = newDist;
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    clock_t init_start = clock(); // Start timing MPI Initialization
    MPI_Init(&argc, &argv);
    clock_t init_end = clock(); // End timing MPI Initialization

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N; // Number of vertices
    int matrix[MAX_NODES][MAX_NODES];

    if (rank == 0) {
        scanf("%d", &N);
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    readMatrix(N, matrix, rank);
    MPI_Bcast(&(matrix[0][0]), N * N, MPI_INT, 0, MPI_COMM_WORLD);
 
    double startTime, endTime;
    startTime = MPI_Wtime();
    parallelFloydWarshall(N, matrix, rank, size);
    endTime = MPI_Wtime();
    if (rank == 0) {
        // Print the final matrix
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (matrix[i][j] == INF) {
                    printf("INF ");
                } else {
                    printf("%d ", matrix[i][j]);
                }
            }
            printf("\n");
        }
    }
    if (rank == 0) {
        printf("Time taken: %lf\n", endTime - startTime);
    }
    clock_t finalize_start = clock(); // Start timing MPI Finalization
    MPI_Finalize();
    clock_t finalize_end = clock(); // End timing MPI Finalization

    double init_elapsed = (double)(init_end - init_start) / CLOCKS_PER_SEC;
    double finalize_elapsed = (double)(finalize_end - finalize_start) / CLOCKS_PER_SEC;
    // printf("MPI Initialization time: %f seconds\n", init_elapsed);
    // printf("MPI Finalization time: %f seconds\n", finalize_elapsed);

    return 0;
}
    