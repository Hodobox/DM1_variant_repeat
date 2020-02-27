import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init

args = args_init()
used_args = ['filename','point_width','adjust_length','matching_scales','alpha','X_limits','Y_limits','square_plot']
filename,point_width,adjust_length,matching_scales,alpha,X_limits,Y_limits,square_plot = [ args[name] for name in used_args ]

data = [[],[]]
titles = ['5\' CTG','CCGCTG','3\' CTG']

which = [int(x) for x in input().split()]

with open(filename,'r') as file:
    seqnum = int(file.readline().strip())
    for i in range(seqnum):
        params = [int(x) for x in file.readline().strip().split()]
        x = params[which[0]]
        y = params[which[1]]
        if X_limits == None or (x >= X_limits[0] and x <= X_limits[1]):
                if Y_limits == None or (y >= Y_limits[0] and y <= Y_limits[1]):
                    data[0].append(x)
                    data[1].append(y)

if matching_scales:
    minvalue = min( [ min(data[0]), min(data[1]) ] )
    maxvalue = max( [ max(data[0]), max(data[1]) ] )
    plt.xlim(minvalue-1,maxvalue+1)
    plt.ylim(minvalue-1,maxvalue+1)

if square_plot:
    plt.gca().set_aspect('equal', adjustable='box')


plt.title('Covariance of ' + titles[ which[0] ] + ' with ' + titles[ which[1] ])
plt.xlabel(titles[ which[0] ])
plt.ylabel(titles[ which[1] ])
plt.scatter(data[0],data[1], s = point_width, alpha = alpha, c = 'r')

plt.show()