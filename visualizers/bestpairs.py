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

if len(sys.argv) == 4:
    alpha = float(sys.argv[3])
points = {}

with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    numseq = int(lines[0])
    curline = 1
    for i in range(numseq):
        score,num = [int(x) for x in lines[curline].split()]
        curline += 1
        for k in range(num):
            params = [int(x) for x in lines[curline].split()]
            param_value = params[param_displayed]
            if param_value in points:
                points[param_value].append(score)
            else:
                points[param_value] = [score]
            curline += 1

tmp = []

for p in points:
    for score in points[p]:
        tmp.append((p,score))

points = tmp

plt.title(filename)
plt.xlabel(param_names[param_displayed])
plt.ylabel("score")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 1, alpha=alpha)
plt.show()
