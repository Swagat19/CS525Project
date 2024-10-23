#include <bits/stdc++.h>

using namespace std;

int N; // Board size is N x N
vector<vector<int>> board;

// Map board positions (i, j) to unique variables for SAT solver
void generateVariables() {
    int var = 1;
    board.resize(N, vector<int>(N));  // Resize the board based on the user input

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            board[i][j] = var++;
        }
    }
}

// Add "at least one queen per row" clauses
void atLeastOneQueenPerRow(ostringstream &clauses, int &clauseCount) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            clauses << board[i][j] << " "; // List all variables in the row
        }
        clauses << "0\n"; // End of clause
        clauseCount++;
    }
}

// Add "at most one queen per row" clauses
void atMostOneQueenPerRow(ostringstream &clauses, int &clauseCount) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = j + 1; k < N; ++k) {
                clauses << -board[i][j] << " " << -board[i][k] << " 0\n";
                clauseCount++;
            }
        }
    }
}

// Add "at least one queen per column" clauses
void atLeastOneQueenPerColumn(ostringstream &clauses, int &clauseCount) {
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            clauses << board[i][j] << " ";
        }
        clauses << "0\n";
        clauseCount++;
    }
}

// Add "at most one queen per column" clauses
void atMostOneQueenPerColumn(ostringstream &clauses, int &clauseCount) {
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            for (int k = i + 1; k < N; ++k) {
                clauses << -board[i][j] << " " << -board[k][j] << " 0\n";
                clauseCount++;
            }
        }
    }
}

// Add diagonal non-attacking clauses
void nonAttackingDiagonals(ostringstream &clauses, int &clauseCount) {
    // Top-left to bottom-right diagonals
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 1; i + k < N && j + k < N; ++k) {
                clauses << -board[i][j] << " " << -board[i + k][j + k] << " 0\n";
                clauseCount++;
            }
        }
    }
    // Top-right to bottom-left diagonals
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 1; i + k < N && j - k >= 0; ++k) {
                clauses << -board[i][j] << " " << -board[i + k][j - k] << " 0\n";
                clauseCount++;
            }
        }
    }
}

int main() {
    cout << "Enter the size of the board (N): ";
    cin >> N;  // Take input for N from the user

    generateVariables();

    // Use ostringstream to temporarily store clauses
    ostringstream clauseStream;
    int clauseCount = 0;
    int varCount = N * N;

    // Generate all the clauses
    atLeastOneQueenPerRow(clauseStream, clauseCount);
    atMostOneQueenPerRow(clauseStream, clauseCount);
    atLeastOneQueenPerColumn(clauseStream, clauseCount);
    atMostOneQueenPerColumn(clauseStream, clauseCount);
    nonAttackingDiagonals(clauseStream, clauseCount);

    ofstream outFile("Nqueens.cnf");
    outFile << "p cnf " << varCount << " " << clauseCount << "\n";

    // Write the clauses stored in clauseStream
    outFile << clauseStream.str();

    outFile.close();
    cout << "DIMACS file Nqueens.cnf generated with " << varCount << " variables and " << clauseCount << " clauses.\n";

    return 0;
}