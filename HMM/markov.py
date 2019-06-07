from state import State
from equations import *
from math import log

class markovState:
    def __init__(self):
        self.value = -10**9
        self.modelsrc = -1

    def set(self,value,src):
        self.value = value
        self.modelsrc = src

class HMM:
    def __init__(self, models, alphabet, seqlen):
        self.alphabet = alphabet
        self.modelchain = [ State(models[i], 0 if i == len(models)-1 else 1/seqlen, 0.1, self.alphabet) for i in range(len(models)) ]
        self.seqlen = seqlen
        self.states = [ [ markovState() for _ in range(len(models))] for _ in range(seqlen)]

    def compute(self, sequence):
        self.states[0][0].set(log(self.modelchain[0].start[sequence[0]]),-1)
        for idx in range(1,self.seqlen):
            for st in range(len(self.modelchain)):
                # stay in same state
                stepProb =  log(1 - self.modelchain[st].nextState) + log(P_X_given_Y_statekeep(self.modelchain[st],sequence[idx],sequence[idx-1]))
                totalProb = stepProb + self.states[idx-1][st].value
                if totalProb > self.states[idx][st].value:
                    self.states[idx][st].set(totalProb,st)
                # transfer to next state
                if st+1 < len(self.modelchain):
                    stepProb = log(self.modelchain[st].nextState) + log(P_X_given_Y_stateswitch(self.modelchain[st], self.modelchain[st+1],sequence[idx],sequence[idx-1]))
                    totalProb = stepProb + self.states[idx-1][st].value
                    if totalProb > self.states[idx][st+1].value:
                        self.states[idx][st+1].set(totalProb,st)
                    

    def bestStateSeq(self):


        #for st in range(len(self.modelchain)):
        #    for idx in range(self.seqlen):
        #        print(str(self.states[idx][st].value)+','+str(self.states[idx][st].modelsrc),end=' ')
        #    print()

        bestValue,bestState = -1,-1

        for i in range(len(self.modelchain)):
            if self.states[self.seqlen-1][i].value > bestValue:
                bestValue = self.states[self.seqlen-1][i].value
                bestState = i

        idx = self.seqlen - 1

        stateSeq = [bestState]

        for idx in range(self.seqlen-1,0,-1):
            bestState = self.states[idx][bestState].modelsrc
            stateSeq.append(bestState)

        stateSeq = stateSeq[::-1]
        return stateSeq