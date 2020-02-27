import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init

args = args_init()
used_args = ['filename','param_displayed','param_name','point_width']
filename,param_displayed,param_name,point_width = [ args[name] for name in used_args ]

points = defaultdict(int)


with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    numseq = int(lines[0])
    curline = 1
    for i in range(numseq):
        #score,num = [int(x) for x in lines[curline].split()]
        #curline += 1   
        params = [int(x) for x in lines[curline].split()]
        param_value = params[param_displayed]
        
        points[param_value] += 1
        curline += 1

scorefile = input()

alignpoints = defaultdict(int)

with open(scorefile,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    numseq = int(lines[0])
    curline = 1
    for i in range(numseq):
        score,num = [int(x) for x in lines[curline].split()]
        curline += 1
        for k in range(num):
            params = [int(x) for x in lines[curline].split()]
            param_value = params[param_displayed]
            alignpoints[param_value] += 1
            curline += 1

points = list(sorted( [ (p,points[p])  for p in points ] ))
plt.title(filename)
plt.xlabel(param_name)
plt.bar([p[0] for p in points], [p[1] for p in points])

alignpoints = list(sorted( [ (p,alignpoints[p])  for p in alignpoints ] ))
plt.ylabel("# of sequences with optimal parameter")
plt.scatter([p[0] for p in alignpoints], [p[1] for p in alignpoints], s = point_width, c='blue')
plt.show()
