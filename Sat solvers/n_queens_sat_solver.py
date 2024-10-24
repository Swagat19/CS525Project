from python.tests.test_pycryptosat import Solver

def solve_sat(dimacs_file, output_file):
    # Create a solver instance
    solver = Solver()

    # Open the DIMACS CNF file and read it line by line
    with open(dimacs_file, 'r') as f:
        for line in f:
            if line.startswith('p'):
                continue
            # Each clause is space-separated literals ending with '0'
            clause = list(map(int, line.strip().split()))[:-1]
            # Add clause to the solver
            solver.add_clause(clause)

    # Solve the SAT problem
    satisfiable, solution = solver.solve()

    # Open the output file to write the results
    with open(output_file, 'w') as out:
        if satisfiable:
            print("Satisfiable")
            out.write("SATISFIABLE!\n")
            # Output the solution (non-negative variables indicate true assignments)
            for i, val in enumerate(solution):
                if i == 0:  # Skip the 0th element (solver index starts at 1)
                    continue
                out.write(f"Variable {i} is {'TRUE' if val else 'FALSE'}\n")
        else:
            print("Unsatisfiable")
            out.write("UNSATISFIABLE!\n")

    print(f"Results written to {output_file}")

solve_sat("Nqueens.cnf", "Nqueens_solver_output.txt")