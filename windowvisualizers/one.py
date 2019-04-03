import matplotlib.pyplot as plt
from args import args_init

args = args_init()
used_args = ['filename','seqnum','repeats']
filename,seqnum,repeats = [ args[name] for name in used_args ]

def parse_seq_data(file):
    numstrings = int(file.readline().strip())
    data = {}
    for i in range(numstrings):
        string = file.readline().strip()
        num_occurences = int(file.readline().strip())
        occurences = file.readline().strip().split()
        occurences = [ occ.split(',') for occ in occurences ]
        occurences = [ ( int(occ[0]), int(occ[1]) ) for occ in occurences ]
        data[string] = occurences
    return data

COLOURS = 'brgkm'

with open(filename,'r') as data:
    numseqs = int(data.readline().strip())
    for _ in range(seqnum-1):
        parse_seq_data(data)

    seqdata = parse_seq_data(data)

    for i,repeat in enumerate(repeats):
        X = [ pair[0] for pair in seqdata[repeat] ]
        Y = [ pair[1] for pair in seqdata[repeat] ]
        plt.plot(X, Y, COLOURS[i]+'-')

    plt.show()