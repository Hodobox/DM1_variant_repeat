#from state import State
from markov import HMM
import sys
from equations import *
#from algorithms import likelyhood 

repeats = ['CTG','CCGCTG','CTG']

if len(sys.argv) > 1:
	numrepeats = int(sys.argv[1])
	assert( len(sys.argv) == 2+numrepeats )
	repeats = sys.argv[2:]

seqnum = int(input())
print(seqnum)
for i in range(seqnum):
    seq = input()

    markovmodel = HMM(repeats,'ACTG',len(seq))
    markovmodel.compute(seq)
    stateSeq = markovmodel.bestStateSeq()
    
    nml = []
    for k in range(len(markovmodel.modelchain)):
        nml.append(sum([1 if c==k else 0 for c in stateSeq])//len(markovmodel.modelchain[k].model))
    #print(str(i) + '/' + str(seqnum),file=sys.stderr)
    print(*nml)
print(*repeats)
