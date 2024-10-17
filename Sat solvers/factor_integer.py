from python.tests.test_pycryptosat import Solver
import numpy as np



# making functions for different operations :- 

def And_operation(c,a,b):
    clause.append([-c,a])
    clause.append([-c,b])
    clause.append([c,-a,-b])

def Or_operation(c,a,b):
    clause.append([-c,a,b])
    clause.append([c,-a])
    clause.append([c,-b])

def Assign_operation(c,a):
    clause.append([-c,a])
    clause.append([c,-a])

def Xor_operation(c,a,b):
    clause.append([-c,a,b])
    clause.append([-c,-a,-b])
    clause.append([c,a,-b])
    clause.append([c,-a,b])

def Double_Xor_and_carry_operation(p,q,r,offset):

    # xor operation
    offset = offset + 1
    Xor_operation(offset,p,q)
    temp1 = offset
    offset = offset + 1
    Xor_operation(offset,temp1,r)

    val = offset

    # finding carry
    offset = offset + 1
    And_operation(offset,p,q)
    temp2 = offset
    offset = offset + 1
    And_operation(offset,temp1,r)
    temp3 = offset
    offset = offset + 1
    Or_operation(offset,temp2,temp3)
    
    carry = offset
    return offset,val,carry


def binary_representation(num, m, n):
    # Convert the number to binary string without the '0b' prefix
    bin_str = bin(num)[2:]
    
    # Reverse the binary string to make LSB at index 0
    bin_str_reversed = bin_str[::-1]
    
    # Pad the binary string with leading zeros to make its length equal to (m + n)
    padded_bin_str = bin_str_reversed.ljust(m + n, '0')
    
    # Convert the padded binary string into a list of integers (0 or 1)
    bin_list = [int(bit) for bit in padded_bin_str]
    
    return bin_list





num = int(input("Enter a number to factor: "))
print("Total bits = ", num.bit_length())

m = int(input("Enter a number of bits: "))
n = int(input("Enter a number of bits: "))



if m>n:
    temp = m
    m = n
    n = temp

# if m + n != num.bit_length() or m < 1 or n < 1:
#     print("Invalid Input")
#     exit()

# Variables Description 

# number stored in z1, z2, z3, ... z_m+n(MSB) :- 1 , 2 , 3 , 4 ... m+n

# number stored in x1, x2, x3, ... x_m(MSB) :- offset1+1 , offset1+2 , offset1+3 , offset1+4 ... offset1+m

# number stored in y1, y2, y3, ... y_n(MSB) :- offset2+1 , offset2+2 , offset2+3 , offset2+4 ... offset2+n
  
# intermidiate variables in each bins which are stored and newly created :- offset+cnt , offset+cnt+1 , offset+cnt+2 , offset+cnt+3 ... offset+...


# Storing clause info :- 

clause = []

offset1=100
offset2=200
offset =300

# Creating bins :- 

bins = {}

for i in range(2,m+n+1):
    bins[i] = []
    for j in range(1,min(m+1,i)):
        temp = i - j
        if temp < n+1 :

            # storing the variables in bins[i] :-
            offset = offset + 1
            And_operation(offset , offset1 + j , offset2 + temp)
            bins[i].append(offset)




bins[m+n+1] = []

for i in range(2,m+n+2):
    

    while(len(bins[i]) > 2):
        
        offset,val,carry = Double_Xor_and_carry_operation(bins[i][0],bins[i][1],bins[i][2],offset)
        bins[i] = bins[i][3:]

        bins[i].append(val)
        bins[i+1].append(carry)

    if len(bins[i]) == 0:
        clause.append([-(i-1)])

    if len(bins[i]) == 1:
        Assign_operation((i-1),bins[i][0])

    if len(bins[i]) == 2:
        Xor_operation((i-1),bins[i][0],bins[i][1])

        # carry 
        offset = offset + 1
        And_operation(offset,bins[i][0],bins[i][1])
        carry = offset
        bins[i+1].append(carry)

    


# assign z variables to the number :-

assign_num = binary_representation(num,m,n)

for i in range(1,m+n+1):
    if assign_num[i-1] == 1:
        clause.append([i])
    else:
        clause.append([-i])

# Print all bins
for i in range(2, m + n + 1):
    print(f"Bin {i}: {bins[i]}")

print("number of clauses = ", len(clause))
# for cl in clause:
#     print(cl)

s = Solver()
for cl in clause:
    s.add_clause(cl)

sat, solution = s.solve()
print("does solution exists: -  ",sat)
if sat == False :
    exit()





# print solution :- 
# print("z = ")
# for i in range(1,m+n+1):
#     print(i, " = ", solution[i])

# print("x = ")
# for i in range(1,m+1):
#     print(offset1+i, " = ", solution[offset1+i])

# print("y = ")
# for i in range(1,n+1):
#     print(offset2+i, " = ", solution[offset2+i])

# finding  the factors :-

x = []
fac1 = 0
for i in range(1,m+1):
    x.append(solution[offset1+i])

for i in range(m):
    fac1 = fac1 + x[i]*(2**i)

y = []
fac2 = 0

for j in range(1,n+1):
    y.append(solution[offset2+j])

for j in range(n):
    fac2 = fac2 + y[j]*(2**j)

print("First factor = ", fac1)
print("Second factor = ", fac2)