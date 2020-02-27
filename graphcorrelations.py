import matplotlib.pyplot as plt 
import sys
import math

fname = sys.argv[1]
which = int(sys.argv[2])

datalines = []

with open(fname,'r') as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip()
        if line[0] != '(':
            datalines.append( [line,[]]  )
        else:
            datalines[-1][1].append(line)

data = datalines[which]

print(data[0])

parsed_data = []

for point in data[1]:
    point = point[1:-1]
    point = point.split(', ')
    point = [ float(x) for x in point ]
    parsed_data.append(point)

#print(parsed_data)

plt.ylabel('Pearson correlation coefficient')
plt.xlabel('Probability')
plt.xscale('log')
for i,p in enumerate(parsed_data):
    y,x = p
    plt.annotate(i+1, (x/3,y), fontsize=12)
plt.gca().invert_xaxis()
plt.scatter( [  p[1] for p in parsed_data ], [ p[0] for p in parsed_data ] )
plt.show()