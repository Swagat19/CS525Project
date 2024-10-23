# Test Case Problem:
## Place 100 non-attacking queens on a 100 x 100 board
(To generalise this problem, we have taken it to be N queens on a N x N board)

### To generate the SAT encoding in DIMACS format:
- g++ -o nqueens n_queens.cpp
- ./nqueens
- Enter value of N.

File with name Nqueens.cnf is generated containing the encoding in CNF.

### To run the SAT solver (Cryptominisat):
(Ensure that pycryptosat is installed using pip)
- python n_queens_sat_solver.py
