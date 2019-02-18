import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init

args = args_init()
used_args = ['filename','alpha','score_limits','param_limits','param_displayed','param_name','point_width']
filename,alpha,score_limits,param_limits,param_displayed,param_name,point_width = [ args[name] for name in used_args ]

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
plt.xlabel(param_name)
plt.ylabel("Score")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = point_width, alpha=alpha)
plt.show()
