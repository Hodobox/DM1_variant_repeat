import matplotlib.pyplot as plt
import sys
from collections import defaultdict
from args import args_init
args_init()
from args import filename, score_limits, param_limits, alpha, param_displayed, param_names

points = defaultdict(list)

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

            if param_limits == None or (param_value >= param_limits[0] and param_value <= param_limits[1]):
                if score_limits == None or (score >= score_limits[0] and score <= score_limits[1]):
                    points[param_value].append(score)
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
