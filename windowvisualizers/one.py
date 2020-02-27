import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from args import args_init
from matplotlib import *

font = {'family' : 'sans',
        'weight' : 'normal',
        'size'   : 20}

plt.rc('font', **font)

args = args_init()
used_args = ['filename','seqnum','repeats']
filename,seqnum,repeats = [ args[name] for name in used_args ]

def parse_seq_data(file):
    numstrings = int(file.readline().strip())
    data = {}
    for i in range(numstrings):
        string,num_occurences = file.readline().strip().split()
        num_occurences = int(num_occurences)
        occurences = file.readline().strip().split()
        occurences = [ occ.split(',') for occ in occurences ]
        occurences = [ ( int(occ[0]), int(occ[1]) ) for occ in occurences ]
        data[string] = occurences
    return data

COLOURS = 'gbrnkm'

chosen_seq_data = []

with open(filename,'r') as file:
    file.readline() # header message
    repeat_sizes = int(file.readline().strip())

    for i in range(repeat_sizes):
        repsize, seqcount, windowsize = [int(x) for x in file.readline().strip().split()]
        for k in range(seqcount):
            seqdata = parse_seq_data(file)
            if k == seqnum:
                chosen_seq_data.append((repsize,windowsize,seqdata))

nonexistent_repeats = []
X_boundary = 0

legend_handles = []

for i,repeat in enumerate(repeats):
    for data in chosen_seq_data:
        if len(repeat) == data[0]:
            legend_handles.append( mpatches.Patch(color=COLOURS[i],label = repeat) )
            if repeat not in data[2]:
                print(repeat + ' not found in sequence ' + str(seqnum) + '!')
                nonexistent_repeats.append((i,repeat))
            else:
                complete_coverage = data[1]-data[0]+1
                X = [ pair[0] for pair in data[2][repeat] ]
                X_boundary = max(X_boundary, X[-1])
                Y = [ round(pair[1]/complete_coverage*100) for pair in data[2][repeat] ]
                #Y = [ pair[1] for pair in data[2][repeat] ]
                plt.plot(X, Y, COLOURS[i]+'-')

for index,repeat in nonexistent_repeats:
    plt.plot([0,X_boundary],[1,1],COLOURS[index]+'-')

plt.ylabel('Percentage of window covered by repeat')
plt.xlabel('Position in sequence (in bases)')
plt.ylim(0,105)
plt.legend(bbox_to_anchor=(1, 1),
           bbox_transform=plt.gcf().transFigure, handles = legend_handles)
plt.show()