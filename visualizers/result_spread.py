import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 2 or len(sys.argv) > 3:
    print("Usage: python3 result_spread.py filename [0-indexed parameter, default 1]")
    sys.exit(1)

filename = sys.argv[1]

param_names = ['(CTG)N','(CCGCTG)M','(CTG)L']
param_displayed = 1
if len(sys.argv) == 3:
    param_displayed = int(sys.argv[2])


points = {}

with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    numseq = int(lines[0])
    curline = 1
    for i in range(numseq):
        score,num = [int(x) for x in lines[curline].split()]
        curline += 1
        results = []
        for k in range(num):
            params = [int(x) for x in lines[curline].split()]
            param_value = params[param_displayed]
            results.append(param_value)
            curline += 1

        spread = max(results) - min(results)
        print(i,max(results))
        points[spread] = points.get(spread,0) + 1

points = list(sorted( [ (p,points[p])  for p in points ] ))
plt.title(filename)
plt.xlabel(param_names[param_displayed])
plt.ylabel("# of best-scoring templates")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 9)
plt.show()
