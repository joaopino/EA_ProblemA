#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>



using namespace std;

// Definição de estado da matriz
struct State {
    vector<vector <int> > grid;
    int moves; // Número de movimentos realizados até o estado atual
};

bool printGrid(vector<vector<int> > grid, int R, int C, int N_grid);
int solve(const vector<vector <int> >& initialGrid, int R, int C, int M);

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
                cin >> grid[i][j];
            }
        }
        
        int moves = solve(grid,R,C,M);
        
        if (moves == -1){
            cout << "the treasure is lost!\n";
        }
        else{
            cout << moves << "\n";
        }

    }
};

vector<vector<int> > rigth_rotation(vector<vector<int> > grid,int x_pos_first,int y_pos_first){
    swap(grid[x_pos_first][y_pos_first], grid[x_pos_first][y_pos_first+1]);
    swap(grid[x_pos_first][y_pos_first], grid[x_pos_first+ 1][y_pos_first+ 1]);
    swap(grid[x_pos_first][y_pos_first], grid[x_pos_first + 1][y_pos_first]);

    return grid;
}

vector<vector<int> > left_rotation(vector<vector<int> > grid,int x_pos_first,int y_pos_first){
    swap(grid[x_pos_first][y_pos_first], grid[x_pos_first + 1][y_pos_first]);
    swap(grid[x_pos_first][y_pos_first], grid[x_pos_first+ 1][y_pos_first+ 1]);
    swap(grid[x_pos_first][y_pos_first], grid[x_pos_first][y_pos_first+1]);

    return grid;
}
 
bool isCorrectPosition(vector<vector<int> > grid, int R, int C) {
    for (int r = 0;r < R-1; r++){
        for(int c = 0; c < C; c++){
            if(grid[r][c] != r+1){
                return false;
            }
        }
    }
    return true;
}

bool printGrid(vector<vector<int> > grid, int R, int C, int N_grid){
    cout << "Grid " << N_grid << "\n";
    for(int r = 0; r < R; r++){
        for(int c = 0; c < C; c++){
            cout << grid[r][c] << " ";
        }
        cout <<"\n";
    }
    return true;
}

vector< vector <int> > updateMemGrid(vector< vector <int> > memGrid,int r, int c, int add_value){
    for (vector <int> row : memGrid) {
        fill(row.begin(), row.end(), 0);
    }
    memGrid[r][c] = add_value;
    return memGrid; 
}

// Função para gerar todos os estados alcançáveis a partir de um estado
vector<State> generateNextStates(State currentState) {
    vector<State> nextStates;
    int rows = currentState.grid.size();
    int cols = currentState.grid[0].size();
    
    for (int i = 0; i < rows - 1; ++i) {
        for (int j = 0; j < cols - 1; ++j) {
            // Pruning cases in which the moves added to the queue to proccess will be redundant.
            // It checks if it made a opposite type of rotation on the previous move and also checks if it's the
            // third time making the same move on the same submatrix.
            vector <int> current_pivot(i,j);

            // Rotates the submatrix 1 time in the right and opens new recursion 
            vector<vector <int> > rotatedClockwise = rigth_rotation(currentState.grid, i, j);
            //rotate right equals +1 in memgrid
            State rotatedClockwise_State = {rotatedClockwise, currentState.moves+1};
            nextStates.push_back(rotatedClockwise_State);
            
            // Rodar no sentido anti-horário
            vector<vector <int> > rotatedCounterClockwise = left_rotation(currentState.grid, i, j);
            State rotatedCounterClockwise_State = {rotatedCounterClockwise,currentState.moves+1};
            nextStates.push_back(rotatedCounterClockwise_State);
            
        }
    }

    return nextStates;
}

bool manhattan_distance(const State& currentState, int R, int C, int M){
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            int absValuerows = std::abs(i - (currentState.grid[i][j] - 1));
            absValuerows += currentState.moves;
            if (absValuerows > M){
                //cout << "AQUI -> " << absValuerows << " e " << M << "\n";
                //printGrid(currentState.grid,R,C,404);
                //cout << "\n";
                return false;
            }
        }
    }
    return true;
}


// Função para resolver o problema
int solve(const vector<vector <int> >& initialGrid, int R, int C, int M) {
    queue<State> bfsQueue;
    unordered_set<string> visited; // Para evitar estados repetidos
    State initial_state = {initialGrid, 0};
    bfsQueue.push(initial_state);
    
    visited.insert(""); // Marcar o estado inicial como visitado

    while (!bfsQueue.empty()) {
        State currentState = bfsQueue.front();
        bfsQueue.pop();

        if (isCorrectPosition(currentState.grid,R,C)) {
            return currentState.moves;
        }

        else if((currentState.moves != M) & (manhattan_distance(currentState,R,C,M) )){
            // regista todos os estados possíveis e, caso não tiverem sido visitados ainda
            // são adicionadaos a bfsqueue para poder visitar. Regista tbm todos os que foram visitados
            // no unordered_set de forma a não repetir estados caso haja demasiadas rotações e volte ao 
            // estado.
            vector<State> nextStates = generateNextStates(currentState);
            for (State nextState : nextStates) {
                string nextStateHash = ""; // Hash do estado para evitar repetições
                for (vector<int> row : nextState.grid) {
                    for (int val : row) {
                        nextStateHash += to_string(val);
                    }
                }

                if (!manhattan_distance(nextState,R,C,M)){
                    visited.insert(nextStateHash);

                }
                else if (visited.find(nextStateHash) == visited.end()) {
                    bfsQueue.push(nextState);
                    visited.insert(nextStateHash);
                }
            }
        }
        
    }

    return -1; // Não foi encontrada uma solução
}



