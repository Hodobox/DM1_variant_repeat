import sys
from scipy import stats

fname = sys.argv[1]

X,Y,Z = [],[],[]

#print(fname)

datapoints = 0

with open(fname,'r') as f:
    n = int(f.readline().strip())
    for _ in range(n):
        params = [int(x) for x in f.readline().strip().split()]
        if len(params) != 3:
            print('Not 3 params')
            exit()

        if 0 in params:
            continue

        #if params[1] < 15:
        #    continue
        #if params[2] > 35:
        #    continue
        datapoints += 1
        X.append(params[0])
        Y.append(params[1])
        Z.append(params[2])

#print('datapoints:',datapoints)
w, p = stats.wilcoxon(X,Z,alternative='greater')
#print('Greater alternative hypoth. with wilcoxon: statistic =', w, 'p-value =',p)
print(p)