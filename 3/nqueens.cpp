#include <bits/stdc++.h>
using namespace std;    

int dfs(int n, int row, unordered_set<int>& posDiag, unordered_set<int>& negDiag, unordered_set<int>& col){
    int ans = 0;
    if(row == n)
        return 1;
    for(int c = 0; c < n; c++){
        if(posDiag.find(row + c) != posDiag.end() || negDiag.find(row - c) != negDiag.end() || col.find(c) != col.end())
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


int main(){
    int n;
    cin >> n;
    unordered_set<int> posDiag, negDiag, col;
    cout << dfs(n, 0, posDiag, negDiag, col) << endl;
    return 0;

}

// 4 - 2, 5 - 10, 