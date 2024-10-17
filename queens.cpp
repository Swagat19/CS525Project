#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

const int N = 100; // Board size is N x N
vector<vector<int>> board(N, vector<int>(N));

// Map board positions (i, j) to unique variables for SAT solver
void generateVariables() {
    int var = 1;
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

// // Add diagonal non-attacking clauses (top-left to bottom-right diagonals)
// void nonAttackingMainDiagonals(ofstream &out, int &clauseCount) {
//     for (int d = -N + 1; d < N; ++d) {
//         vector<int> diag;
//         for (int i = 0; i < N; ++i) {
//             int j = i - d;
//             if (j >= 0 && j < N) {
//                 diag.push_back(board[i][j]);
//             }
//         }
//         // Now apply pairwise constraints for this diagonal
//         for (size_t i = 0; i < diag.size(); ++i) {
//             for (size_t j = i + 1; j < diag.size(); ++j) {
//                 out << -diag[i] << " " << -diag[j] << " 0\n";
//                 clauseCount++;
//             }
//         }
//     }
// }

// // Add diagonal non-attacking clauses (top-right to bottom-left diagonals)
// void nonAttackingAntiDiagonals(ofstream &out, int &clauseCount) {
//     for (int d = 0; d < 2 * N - 1; ++d) {
//         vector<int> diag;
//         for (int i = 0; i < N; ++i) {
//             int j = d - i;
//             if (j >= 0 && j < N) {
//                 diag.push_back(board[i][j]);
//             }
//         }
//         // Now apply pairwise constraints for this diagonal
//         for (size_t i = 0; i < diag.size(); ++i) {
//             for (size_t j = i + 1; j < diag.size(); ++j) {
//                 out << -diag[i] << " " << -diag[j] << " 0\n";
//                 clauseCount++;
//             }
//         }
//     }
// }

int main() {
    generateVariables();

    // Use ostringstream to temporarily store clauses
    ostringstream clauseStream;
    int clauseCount = 0;
    int varCount = N * N; // 10000 variables for 100x100 board

    // First generate all the clauses
    atLeastOneQueenPerRow(clauseStream, clauseCount);
    atMostOneQueenPerRow(clauseStream, clauseCount);
    atLeastOneQueenPerColumn(clauseStream, clauseCount);
    atMostOneQueenPerColumn(clauseStream, clauseCount);
    nonAttackingDiagonals(clauseStream, clauseCount);

    // Now open the file and write the header first
    ofstream outFile("100queens.cnf");
    outFile << "p cnf " << varCount << " " << clauseCount << "\n";

    // Write the clauses stored in clauseStream
    outFile << clauseStream.str();

    outFile.close();
    cout << "DIMACS file 100queens.cnf generated with " << varCount << " variables and " << clauseCount << " clauses.\n";

    return 0;
}