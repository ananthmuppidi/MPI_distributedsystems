#include <bits/stdc++.h>
#include "mpi.h"
using namespace std;
#define INF INT_MAX

void print_state(vector<vector<int>>& a){
    cout << "-----------------" << endl;
    for(int i = 0; i < a.size(); i++){
        for(int j = 0; j < a[0].size(); j++) {
            if (a[i][j] == INF)
                cout << "INF ";
            else
                cout << a[i][j] << " ";
        }

        cout << endl;
    }
    cout << "-----------------" << endl;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int tsk_id, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &tsk_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int n;
    vector<vector<int>> graph;

    // Root process reads input and broadcasts
    if (tsk_id == 0)
    {
        cin >> n;
        graph.resize(n, vector<int>(n));
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

    // Broadcast the graph size
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Resize graph in all processes
    graph.resize(n, vector<int>(n));

    // Broadcast initial state of the graph
    for (int i = 0; i < n; i++)
    {
        MPI_Bcast(graph[i].data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Parallel computation with row distribution as in Conway's Game of Life
    for (int k = 0; k < n; k++)
    {
        // Each process updates specific rows based on its rank (tsk_id)
        for (int i = tsk_id; i < n; i += num_procs)
        {
            for (int j = 0; j < n; j++)
            {
                if (graph[i][k] != INF && graph[k][j] != INF && graph[i][j] > graph[i][k] + graph[k][j])
                {
                    graph[i][j] = graph[i][k] + graph[k][j];
                }
            }
        }

        // Synchronize the updated graph after each iteration of k
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < n; i++)
        {`
            MPI_Bcast(graph[i].data(), n, MPI_INT, i % num_procs, MPI_COMM_WORLD);
        }
    }

    // Print final graph in process 0
    if (tsk_id == 0)
    {
        print_state(graph);
    }

    MPI_Finalize();
    return 0;
}
