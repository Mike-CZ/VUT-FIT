#!/usr/bin/env python3

from satispy import Variable, Cnf
from satispy.solver import Minisat

BIS_FILE = 'in/bis.txt'
BIS_FILE_ENCRYPTED = 'in/bis.txt.enc'

N_B = 32
N = 8 * N_B

# define bits array of boolean values
bits = [Variable("bit_%d" % i) for i in range(N)]


# define rule
# expression has to be in CNF
# 001 -> 1
# 010 -> 1
# 100 -> 1
# 110 -> 1
def rule(x, y, z):
    return (x | y | z) & (x | -y | -z) & (-x | y | -z) & (-x | -y | -z)


# get expression to get next value
def get_expression(expression_bits):
    # append msb and lsb
    expression_bits = [expression_bits[-1]] + expression_bits + [expression_bits[0]]
    return [rule(expression_bits[i + 2], expression_bits[i + 1], expression_bits[i]) for i in range(N)]

bis_txt = open(BIS_FILE, "rb").read(N_B)
bis_enc = open(BIS_FILE_ENCRYPTED, "rb").read(N_B)

# get first 32 bytes from keystream
key_stream = int.from_bytes(bis_txt, 'little') ^ int.from_bytes(bis_enc, 'little')

# get expression to calculate next value
expression = get_expression(bits)

solver = Minisat()

# loop 128 times
for i in range(128):
    exp = Cnf()

    # create expression
    for j in range(N):
        bit_value = ((1 << j) & key_stream) >= 1
        exp &= (expression[j] if bit_value is True else -expression[j])

    # get solution
    solution = solver.solve(exp)

    # update key stream
    if solution.success:
        result = 0
        for j in range(N):
            result |= (1 if solution[bits[j]] is True else 0) << j
        key_stream = result
    else:
        print('Could not find the key.')
        exit(1)

# the result is stored in key stream
print(key_stream.to_bytes(N_B, 'little').decode(), end='')

