import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 2 or len(sys.argv) > 4:
    print("Usage: python3 bestreads.py filename [0-indexed parameter, default 1] [alpha in (0,1]")
    sys.exit(1)

filename = sys.argv[1]

param_names = ['(CTG)N','(CCGCTG)M','(CTG)L']
param_displayed = 1
if len(sys.argv) >= 3:
    param_displayed = int(sys.argv[2])

alpha = 1

points = {}

with open(filename,'r') as data:
    counter = 0
    for line in data:
        counter += 1
        if counter % 1000000 == 0:
            print(counter)
        line = line.strip()
        n,m,l,seq,score = [int(x) for x in line.split()]
        params = [n,m,l]
        param_value = params[param_displayed]
        if param_value in points:
            points[param_value].append(score)
        else:
            points[param_value] = [score]

tmp = []

for p in points:
    for score in points[p]:
        tmp.append((p,score))

points = tmp

plt.title(filename)
plt.xlabel(param_names[param_displayed])
plt.ylabel("score")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 1,alpha=alpha)
plt.show()
