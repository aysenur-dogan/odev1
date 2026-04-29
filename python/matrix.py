from mpi4py import MPI
import time

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# sadece master okur
if rank == 0:
    def read_matrix(filename):
        with open(filename, "r") as f:
            lines = f.readlines()
            n = int(lines[0])
            matrix = [list(map(int, line.split())) for line in lines[1:]]
        return n, matrix

    n, A = read_matrix("../data/a.txt")
    _, B = read_matrix("../data/b.txt")
else:
    n = None
    A = None
    B = None

# herkese n gönder
n = comm.bcast(n, root=0)

# A'yı parçalara böl
if rank == 0:
    rows_per_proc = n // size
    split_A = [A[i*rows_per_proc:(i+1)*rows_per_proc] for i in range(size)]
else:
    split_A = None

local_A = comm.scatter(split_A, root=0)

# B'yi herkese gönder
B = comm.bcast(B, root=0)
comm.Barrier()
start_time = MPI.Wtime()

# local çarpım
local_C = []
for row in local_A:
    result_row = []
    for j in range(n):
        sum_val = 0
        for k in range(n):
            sum_val += row[k] * B[k][j]
        result_row.append(sum_val)
    local_C.append(result_row)

# sonuçları topla
C = comm.gather(local_C, root=0)
end_time = MPI.Wtime()
# sadece master yazdırır
if rank == 0:
    print("Calisma suresi:", end_time - start_time, "saniye")