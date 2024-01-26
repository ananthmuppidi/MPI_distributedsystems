#include <bits/stdc++.h>
#include "mpi.h"
using namespace std;
#define INF INT_MAX

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int tsk_id, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &tsk_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int n;
    vector<vector<int>> graph;

    if (tsk_id == 0)
    {
        cin >> n;
        graph.resize(n, vector<int>(n + 1));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> graph[i][j];
                if (graph[i][j] == -1)
                    graph[i][j] = INF;
            }
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    graph.resize(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; i++)
    {
        MPI_Bcast(graph[i].data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (n < num_procs)
    {
        for (int k = 0; k < n; k++)
        {
            int i = tsk_id;
            if(i >= n){
                i = n;
            }
            for (int k = 0; k < n; k++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (graph[i][k] != INF && graph[k][j] != INF && graph[i][j] > graph[i][k] + graph[k][j])
                    {
                        graph[i][j] = graph[i][k] + graph[k][j];
                    }
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < n; i++)
        {
            MPI_Bcast(graph[i].data(), n, MPI_INT, i, MPI_COMM_WORLD);
        }
    }
    else if (n == num_procs)
    {
        for (int k = 0; k < n; k++)
        {
            int i = tsk_id;
            for (int k = 0; k < n; k++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (graph[i][k] != INF && graph[k][j] != INF && graph[i][j] > graph[i][k] + graph[k][j])
                    {
                        graph[i][j] = graph[i][k] + graph[k][j];
                    }
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < n; i++)
        {
            MPI_Bcast(graph[i].data(), n, MPI_INT, i, MPI_COMM_WORLD);
        }
    }
    else
    {

        int rows_per_thread = n / num_procs;
        int start_row = tsk_id * rows_per_thread;
        int end_row = start_row + rows_per_thread;
        if (tsk_id == num_procs - 1)
        {
            end_row = n;
        }
        for (int k = 0; k < n; k++)
        {
            for (int i = start_row; i < end_row; i++)
            {
                int corrected_i = i >= n ? n : i;
                for (int j = 0; j < n; j++)
                {
                    if (graph[i][k] != INF && graph[k][j] != INF && graph[i][j] > graph[i][k] + graph[k][j])
                    {
                        graph[i][j] = graph[i][k] + graph[k][j];
                    }
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < num_procs; i++)
        {
            int temp_start_row = i * rows_per_thread;
            int temp_end_row = temp_start_row + rows_per_thread;
            if (i == num_procs - 1)
            {
                temp_end_row = n;
            }

            for (int j = temp_start_row; j < temp_end_row; j++)
            {
                MPI_Bcast(graph[j].data(), n, MPI_INT, i, MPI_COMM_WORLD);
            }
        }
    }

    if (tsk_id == 0)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (graph[i][j] == INF)
                    cout << "INF"
                         << " ";
                else
                    cout << graph[i][j] << " ";
            }
            cout << endl;
        }
    }
    MPI_Finalize();
    return 0;
}