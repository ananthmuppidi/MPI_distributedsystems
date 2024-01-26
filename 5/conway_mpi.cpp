#include <bits/stdc++.h>
#include "mpi.h"
using namespace std;

void print_state(vector<vector<int>> &a)
{
    cout << "-----------------" << endl;
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < a[0].size(); j++)
            cout << a[i][j] << " ";
        cout << endl;
    }
    cout << "-----------------" << endl;
}

int get_neighbour_live_cnt(vector<vector<int>> &a, int i, int j)
{
    int n = a.size();
    int m = a[0].size();
    int cnt = 0;
    for (int k = i - 1; k <= i + 1; k++)
    {
        for (int l = j - 1; l <= j + 1; l++)
        {
            if (k == i && l == j)
                continue;
            if (k < 0 || k >= n || l < 0 || l >= m)
                continue;
            if (a[k][l] == 1)
                cnt++;
        }
    }
    return cnt;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int tsk_id, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &tsk_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int n, m, t;
    vector<vector<int>> a, b;

    // Read input and broadcast if process ID is 0
    if (tsk_id == 0)
    {
        cin >> n >> m >> t;
        a.resize(n, vector<int>(m));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cin >> a[i][j];
            }
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&t, 1, MPI_INT, 0, MPI_COMM_WORLD);

    a.resize(n, vector<int>(m));

    for (int i = 0; i < n; i++)
    {
        MPI_Bcast(a[i].data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    b = a;

    for (int iter = 0; iter < t; iter++)
    {

        for (int i = tsk_id; i < n; i += num_procs)
        {
            for (int j = 0; j < m; j++)
            {
                // i  has the row and j has the column
                int cnt = get_neighbour_live_cnt(a, i, j);
                if (a[i][j] == 1)
                {
                    if (cnt < 2)
                        b[i][j] = 0;
                    else if (cnt == 2 || cnt == 3)
                        b[i][j] = 1;
                    else
                        b[i][j] = 0;
                }
                else
                {
                    if (cnt == 3)
                        b[i][j] = 1;
                    else
                        b[i][j] = 0;
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < n; i++)
        {
            MPI_Bcast(b[i].data(), m, MPI_INT, i % num_procs, MPI_COMM_WORLD);
        }
    }
    // after t iterations, print the state from tsk 0
    if (tsk_id == 0)
    {
        print_state(a);
    }

    MPI_Finalize();
    return 0;
}
