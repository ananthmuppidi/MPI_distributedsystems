#include <bits/stdc++.h>
#include "mpi.h"
using namespace std;

int dfs(int n, int row, unordered_set<int>& posDiag, unordered_set<int>& negDiag, unordered_set<int>& col) {
    int ans = 0;
    if (row == n)
        return 1;
    for (int c = 0; c < n; c++) {
        if (posDiag.find(row + c) != posDiag.end() || negDiag.find(row - c) != negDiag.end() || col.find(c) != col.end())
            continue;
        posDiag.insert(row + c);
        negDiag.insert(row - c);
        col.insert(c);
        ans += dfs(n, row + 1, posDiag, negDiag, col);
        posDiag.erase(row + c);
        negDiag.erase(row - c);
        col.erase(c);
    }
    return ans;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int tsk_id, num_procs;
    double start_time, end_time;
    MPI_Comm_rank(MPI_COMM_WORLD, &tsk_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int n;
    unordered_set<int> posDiag, negDiag, col;
    int ans = 0;

    if (tsk_id == 0) {
        cin >> n;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int local_ans = 0;

    start_time = MPI_Wtime(); // Start measuring time

    for (int i = tsk_id; i < n; i += num_procs) {
        col.clear();
        posDiag.clear();
        negDiag.clear();
        posDiag.insert(i);
        negDiag.insert(0 - i);
        col.insert(i);
        local_ans += dfs(n, 1, posDiag, negDiag, col);
    }

    end_time = MPI_Wtime(); // Stop measuring time

    MPI_Reduce(&local_ans, &ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (tsk_id == 0) {
        cout <<  (end_time - start_time) << endl;
        // cout << ans << endl;
    }

    MPI_Finalize();
    return 0;
}
