import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init
import random

args = args_init()
used_args = ['filename','alpha','Y_limits','X_limits','param_displayed','param_name','point_width']
filename,alpha,Y_limits,X_limits,param_displayed,param_name,point_width = [ args[name] for name in used_args ]

seq_displayed = 0#int(input())

points = defaultdict(list)

allParams = []
paramScoreMap = {}

with open(filename,'r') as data:
    for i,line in enumerate(data):
        line = line.strip()
        n,m,l,seq,score = [int(x) for x in line.split()]
        params = (n,m,l)
        param_value = params[param_displayed]
        paramScoreMap[params] = score 
        if seq == seq_displayed:
            if X_limits == None or (param_value >= X_limits[0] and param_value <= X_limits[0]):
                if Y_limits == None or (score >= Y_limits[0] and score <= Y_limits[1]):
                    allParams.append(params)
                    points[param_value].append(score)

tmp = []


for p in points:
    for score in points[p]:
        tmp.append((p,score))

points = tmp

plt.title(filename)
plt.xlabel(param_name)
plt.ylabel("Score")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = point_width, alpha=alpha)

highlight = [
    (0,0),
    (50,0),
    (100,0),
    (150,0),
    (200,0),
    (250,0),
    (300,0),
    (350,0),
    (400,0),

    (0,100),
    (0,150),
    (0,200),
    (0,250),
    (0,300),
    (0,400),
    (0,450),
    (0,460),

    (291,55),
    (102,116),
    (130,150),
    (144,64),
    (272,52),
    (305,65),
    (163,121)
]

for i in range(len(allParams)):
    if (allParams[i][0],allParams[i][2]) in highlight:
        plt.annotate('(' + str(allParams[i][0])+',' + str(allParams[i][2]) + ')',xy=(allParams[i][1],paramScoreMap[allParams[i]]), xytext=(10,5), textcoords='offset points', color = 'r')
        plt.scatter([ allParams[i][1] ], [ paramScoreMap[allParams[i]] ], s = point_width*4, alpha = 1, c = 'r' )

plt.show()
