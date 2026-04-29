#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int read_matrix(const char *filename, int **matrix, int *n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya acilamadi: %s\n", filename);
        return 0;
    }

    fscanf(file, "%d", n);

    *matrix = (int *)malloc((*n) * (*n) * sizeof(int));
    if (*matrix == NULL) {
        printf("Bellek ayrilamadi.\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < (*n) * (*n); i++) {
        fscanf(file, "%d", &((*matrix)[i]));
    }

    fclose(file);
    return 1;
}

int main(int argc, char **argv) {
    int rank, size;
    int n;
    int *A = NULL;
    int *B = NULL;
    int *C = NULL;
    int *local_A = NULL;
    int *local_C = NULL;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        if (!read_matrix("../data/a.txt", &A, &n)) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int n_b;
        if (!read_matrix("../data/b.txt", &B, &n_b)) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if (n != n_b) {
            printf("Matris boyutlari ayni degil.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n % size != 0) {
        if (rank == 0) {
            printf("Hata: Matris boyutu (%d), islemci sayisina (%d) tam bolunmeli.\n", n, size);
        }
        MPI_Finalize();
        return 1;
    }

    int rows_per_proc = n / size;

    if (rank != 0) {
        B = (int *)malloc(n * n * sizeof(int));
    }

    local_A = (int *)malloc(rows_per_proc * n * sizeof(int));
    local_C = (int *)malloc(rows_per_proc * n * sizeof(int));

    if (rank == 0) {
        C = (int *)malloc(n * n * sizeof(int));
    }

    MPI_Scatter(
        A,
        rows_per_proc * n,
        MPI_INT,
        local_A,
        rows_per_proc * n,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    MPI_Bcast(B, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += local_A[i * n + k] * B[k * n + j];
            }
            local_C[i * n + j] = sum;
        }
    }

    MPI_Gather(
        local_C,
        rows_per_proc * n,
        MPI_INT,
        C,
        rows_per_proc * n,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Calisma suresi: %f saniye\n", end_time - start_time);

        if (n <= 10) {
            printf("Sonuc matrisi:\n");
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    printf("%d ", C[i * n + j]);
                }
                printf("\n");
            }
        }
    }

    free(A);
    free(B);
    free(C);
    free(local_A);
    free(local_C);

    MPI_Finalize();
    return 0;
}