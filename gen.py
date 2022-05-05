import sys
import math
import cmath
import random

for _ in range(50):
    x = random.random() * 500 - 250
    y = random.random() * 500 - 250
    print(f"sphere\n{x} {y} -10000 5")
    print(f"box\n{x - 1} {y + 8} -10000 2 4 1")
    print(f"box\n{x - 1} {y - 2} -10000 2 4 1")
    print(f"box\n{x + 2} {y + 1} -10000 4 2 1")
    print(f"box\n{x - 8} {y + 1} -10000 4 2 1")
