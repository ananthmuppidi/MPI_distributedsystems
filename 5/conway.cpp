#include <bits/stdc++.h>
using namespace std;

void print_state(vector<vector<int>>& a){
    for(int i = 0; i < a.size(); i++){
        for(int j = 0; j < a[0].size(); j++) cout << a[i][j] << " ";
        cout << endl;
    }
    cout << "-----------------" << endl;

}

int get_neighbour_live_cnt(vector<vector<int>>& a, int i, int j){
    int n = a.size();
    int m = a[0].size();
    int cnt = 0;
    for(int k = i - 1; k <= i + 1; k++){
        for(int l = j - 1; l <= j + 1; l++){
            if(k == i && l == j) continue;
            if(k < 0 || k >= n || l < 0 || l >= m) continue;
            if(a[k][l] == 1) cnt++;

        }
    }
    return cnt;
}

void simulate_step(vector<vector<int>>& a, vector<vector<int>>& b){
    int n = a.size();
    int m = a[0].size();
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            int cnt = get_neighbour_live_cnt(a, i, j);
            if(a[i][j] == 1){
                if(cnt < 2) b[i][j] = 0;
                else if(cnt == 2 || cnt == 3) b[i][j] = 1;
                else b[i][j] = 0;
            }
            else{
                if(cnt == 3) b[i][j] = 1;
                else b[i][j] = 0;
            }
        }
    }
}

int main(){
    int n, m, t;
    cin >> n >> m >> t;
    vector<vector<int>> a(n, vector<int>(m));
    vector<vector<int>> b(n, vector<int>(m));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++) cin >> a[i][j];
    }
    // print the current state
    cout << "Initial state" << endl;
    print_state(a);
    for(int i = 1; i <= t; i++) {
        simulate_step(a, b);
        a = b;
    }
    cout << "After " << t << " steps: " << endl;
    print_state(b);

}