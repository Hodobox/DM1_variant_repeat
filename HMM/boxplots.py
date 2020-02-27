import numpy as np 
import matplotlib.pyplot as plt 
import sys
from random import randrange
from collections import defaultdict


font = {'family' : 'sans',
        'weight' : 'normal',
        'size'   : 20}
plt.rc('font', **font)

filename = sys.argv[1]

with open(filename,'r') as scores:
    lines = [ line.strip() for line in scores.readlines() ]

idx = 0
points = defaultdict(list)
while idx < len(lines):
    name,repeats = lines[idx].split()
    repeats = int(repeats)
    idx += 1
    for i in range(repeats):
        scoring = lines[idx].split()
        idx += 1
        variant = scoring[2][:-1]
        progenitor_length = int(scoring[3].split('-')[0])
        progenitor_length *= len(variant)
        
        instability = int(scoring[4])
        
        

        if progenitor_length > 0:
            instability = instability / progenitor_length
            points[variant].append((progenitor_length,instability))

CTG = [ x[1] for x in points['CTG'] ]
CCGCTG = [ x[1] for x in points['CCGCTG'] ]

#for i,v in enumerate(CTG):
#    print(i,v)

fig1, ax1 = plt.subplots()
#ax1.set_title('Box and whiskers of repeat instability')
plt.ylabel('Instability per base', labelpad = 20)
ax1.boxplot([CTG,CCGCTG],labels=['CTG','CCGCTG'])
ax1.scatter([1+randrange(-10,10)*0.003 for i in range(len(CTG))], CTG, alpha = 0.4, s = 15)

ax1.scatter([2+randrange(-10,10)*0.003 for i in range(len(CCGCTG))], CCGCTG, alpha = 0.4, s=15)

fig = plt.gcf()
fig.set_size_inches(12, 8.5,forward = True)
fig.savefig('Figures/Box-and-whiskers.png', dpi=300)


plt.show()