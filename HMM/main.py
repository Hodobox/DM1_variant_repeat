#from state import State
from markov import HMM
#from algorithms import likelyhood 

seqnum = int(input())
print(seqnum)
for i in range(seqnum):
    seq = input()

    markovmodel = HMM(['CTG','CCGCTG','CTG'],'ACTG',len(seq))
    markovmodel.compute(seq)
    stateSeq = markovmodel.bestStateSeq()
    #print(stateSeq)
    nml = []
    for i in range(len(markovmodel.modelchain)):
        nml.append(sum([1 if c==i else 0 for c in stateSeq])//len(markovmodel.modelchain[i].model))

    print(*nml)