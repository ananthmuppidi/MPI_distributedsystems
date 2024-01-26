#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0)); // Seed the random number generator with the current time

    int n = 8; // Matrix size
    int min_val = -1; // Minimum value for elements (to include -1)
    int max_val = 10; // Maximum value for elements (adjust as needed)

    vector<vector<int>> matrix(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int random_value = rand() % (max_val - min_val + 1) + min_val;
            matrix[i][j] = random_value;
        }
    }

    // Print the generated matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
