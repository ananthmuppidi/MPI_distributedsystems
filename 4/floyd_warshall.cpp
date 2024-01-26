#include <bits/stdc++.h>
using namespace std;

#define INF INT_MAX

void floyd_warshall(vector<vector<int>>& res, int n){
    for(int k = 0; k < n; k++){
        for(int i = 0; i < n; i++){
            if(i >= n){
                i = n;
            }
            for(int j = 0; j < n; j++){
                if(res[i][k] != INF && res[k][j] != INF && res[i][j] > res[i][k] + res[k][j]){
                    res[i][j] = res[i][k] + res[k][j];
                }
            }
        }
    }
}

int main(){
    int n;
    cin >> n;
    vector<vector<int> > graph(n, vector<int>(n + 1));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> graph[i][j];
            if(graph[i][j] == -1){
                graph[i][j] = INF;
            }
        }
    }
    floyd_warshall(graph, n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if (graph[i][j] == INF)
                cout << "INF ";
            else
                cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    return 0;

}