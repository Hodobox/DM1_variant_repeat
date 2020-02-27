import sys
from scipy import stats

testdir = 'tests/limits/'

fname = sys.argv[1]

LIMITS = {
    'GO9' : ((200,550),(20,35),(10,30)),
}

X,Y,Z = [],[],[]
nX,nY,nZ = [],[],[]

print(fname)

limitname = fname.split('/')[-1].split('.')[0]

print('Limits are:', LIMITS[limitname])

datapoints = 0

with open(fname,'r') as f:
    n = int(f.readline().strip())
    for _ in range(n):
        params = [int(x) for x in f.readline().strip().split()]
        if len(params) != 3:
            print('Not 3 params')
            exit()

        nX.append(params[0])
        nY.append(params[1])
        nZ.append(params[2])

        ignore = False
        for i,p in enumerate(params):
            if p <= LIMITS[limitname][i][0] or p >= LIMITS[limitname][i][1]:
                ignore = True

        if ignore:
            continue

        datapoints += 1
        X.append(params[0])
        Y.append(params[1])
        Z.append(params[2])

testfile = testdir + fname.split('/')[-1]
print(testfile)

with open(testfile,'w+') as out:
    print(len(X),file=out)
    for i in range(len(X)):
        print(X[i],Y[i],Z[i],file=out)

print("5' CTG vs. variant")
print(stats.spearmanr(X,Y).correlation)
print("3' CTG vs variant")
print(stats.spearmanr(Z,Y).correlation)
print("5' CTG vs 3' CTG")
print(stats.spearmanr(X,Z).correlation)
print('used ' + str(datapoints) + ' datapoints')

print('without limits:')
print("5' CTG vs. variant")
print(stats.spearmanr(nX,nY).correlation)
print("3' CTG vs variant")
print(stats.spearmanr(nZ,nY).correlation)
print("5' CTG vs 3' CTG")
print(stats.spearmanr(nX,nZ).correlation)
print(n,"sequences")