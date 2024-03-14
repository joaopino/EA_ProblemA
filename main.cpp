#include <iostream>
#include <vector>

using namespace std;


bool printGrid(const vector<vector<int> >& grid, int R, int C, int N_grid);


int main() {
    string N_testcases;
    ios_base::sync_with_stdio(false); // Improve input/output performance
    cin.tie(NULL);

    // Get the number of test cases
    getline(cin, N_testcases);
    int N_testcases_parsed = std::stoi(N_testcases);
    
    // Receive the test cases and solve them
    for(int i = 0; i < N_testcases_parsed; i++){
        // Get the configs
        int R, C, M;
        cin >> R >> C >> M; // Rows, Columns, Maximum moves
        
        //Get the grid values
        vector< vector<int> > grid(R, vector<int>(C));
        for (int i = 0; i < R; ++i) {
            for (int j = 0; j < C; ++j) {
                // Using scanf for faster input
                scanf("%d", &grid[i][j]);
            }
        }
        //print grid just to debug
        printGrid(grid,R,C,i);
    }
};

void RotateRight(const vector<vector<int> >& grid){
    
}

// temos de ter uma lista com os numero pertencentes da matriz  
bool isCorrectPosition(const vector<vector<int> >& grid, int r, int c) {
    int expectedValue = 1;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (grid[i][j] != expectedValue) {
                return false;
            }
            expectedValue = (expectedValue % r) + 1;
        }
    }
    return true;
}

bool printGrid(const vector<vector<int> >& grid, int R, int C, int N_grid){
    cout << "Grid " << N_grid << "\n";
    for(int r = 0; r < R; r++){
        for(int c = 0; c < C; c++){
            cout << grid[r][c] << " ";
        }
        cout <<"\n";
    }
    return true;
}

