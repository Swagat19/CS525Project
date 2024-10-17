from python.tests.test_pycryptosat import Solver

s = Solver()
s.add_clause([1])
s.add_clause([-2])
s.add_clause([-1, 2, 3])
sat, solution = s.solve()
print(sat)
print(solution)