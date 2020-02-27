import matplotlib.pyplot as plt 
import sys
import math

samplesizes = [
598,
2291,
834,
861,
345,
361,
274,
1868,
2009,
222,]

fname = sys.argv[1]
which = int(sys.argv[2])


datalines = []

with open(fname,'r') as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip()
        if 'SpearmanrResult(correlation=' not in line:
            datalines.append( [line,[]]  )
        else:
            datalines[-1][1].append(line)

data = datalines[which]

print(data[0])

parsed_data = []

for i,point in enumerate(data[1]):
    point = point[1:-1]
    point = point.split(', ')
    Spearmanr = float(point[0].split('=')[-1])
    pval = float(point[1].split('=')[-1])
    #print('r =',Spearmanr,'n = ',samplesizes[i])
    delta = 1.96 * (1.0 / math.sqrt(samplesizes[i]-3))
    lower = math.tanh(math.atanh(Spearmanr) -  delta)
    upper = math.tanh(math.atanh(Spearmanr) + delta)
    #print("95% CI: ",lower,upper)
    print(Spearmanr,lower,upper)