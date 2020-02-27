from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import sys

removed = 0

def outlier1(val, ave):
    #print(val,ave)
    return (val > 3*ave or val < ave/3) and abs(ave-val) > 10

def outlier(X,Y,Z,idx):
    aveX = sum(X) / len(X)
    aveY = sum(Y) / len(Y)
    aveZ = sum(Z) / len(Z)

    isOutlier = outlier1(X[idx],aveX) or outlier1(Y[idx],aveY) or outlier1(Z[idx],aveZ)
    global removed
    if isOutlier:
        removed += 1
    return isOutlier

fname = sys.argv[1]
X,Y,Z = [],[],[]

with open(fname,'r') as f:
    n = int(f.readline().strip())
    for _ in range(n):
        params = [int(x) for x in f.readline().strip().split()]
        if len(params) != 3:
            print('Not 3 params')
            exit()
        X.append(params[0])
        Y.append(params[1])
        Z.append(params[2])

outliers = [ outlier(X,Y,Z,i) for i in range(len(X))]

filtered = []

for i in range(len(outliers)):
    if not outliers[i]:
        filtered.append([X[i],Y[i],Z[i]])

print('{0} outliers in {1} points'.format(removed, len(X)) )

X = [ filtered[i][0] for i in range(len(filtered)) ]
Y = [ filtered[i][1] for i in range(len(filtered)) ]
Z = [ filtered[i][2] for i in range(len(filtered)) ]


fig = plt.figure()
ax = plt.axes(projection='3d')
ax.scatter3D(X,Y,Z)
ax.set_xlabel('Left CTG')
ax.set_ylabel('CCGCTG')
ax.set_zlabel('Right CTG')

plt.show()