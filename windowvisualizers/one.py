import matplotlib.pyplot as plt
from args import args_init

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

COLOURS = 'brgkm'

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

for i,repeat in enumerate(repeats):
    for data in chosen_seq_data:
        if len(repeat) == data[0]:
            if repeat not in data[2]:
                print(repeat + ' not found in sequence ' + str(seqnum) + '!')
            else:
                complete_coverage = data[1]-data[0]+1;
                X = [ pair[0] for pair in data[2][repeat] ]
                Y = [ round(pair[1]/complete_coverage*100) for pair in data[2][repeat] ]
                plt.plot(X, Y, COLOURS[i]+'-')


plt.show()