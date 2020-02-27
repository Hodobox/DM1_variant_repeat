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
        try:
            val = float(line)
            datalines[-1][1].append(val)    
        except:
            datalines.append( [line,[]]  )

data = datalines[which]

print(data)

parsed_data = []

for i,point in enumerate(data[1]):
    Spearmanr = point
    #pval = float(point[1].split('=')[-1])
    #print('r =',Spearmanr,'n = ',samplesizes[i])
    delta = 1.96 * (1.0 / math.sqrt(1901-3))
    lower = math.tanh(math.atanh(Spearmanr) -  delta)
    upper = math.tanh(math.atanh(Spearmanr) + delta)
    #print("95% CI: ",lower,upper)
    print(Spearmanr,lower,upper)