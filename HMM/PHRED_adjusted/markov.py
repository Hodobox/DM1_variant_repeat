from state import State
from equations import *
from algorithms import phredvalues
from math import log

class markovState:
    def __init__(self):
        self.value = -10**9
        self.modelsrc = -1

    def set(self,value,src):
        self.value = value
        self.modelsrc = src


def safelog(x):
    if x > 0:
        return log(x)
    return -1000

class HMM:
    def __init__(self, models, alphabet, seqlen):
        self.alphabet = alphabet
        self.modelchain = [ State(models[i], 0 if i == len(models)-1 else 1/seqlen, self.alphabet) for i in range(len(models)) ]
        self.seqlen = seqlen
        self.states = [ [ markovState() for _ in range(len(models))] for _ in range(seqlen)]

    def compute(self, sequence, phred):
        phred = phredvalues(phred)
        #print(phred)
        if sequence[0] == self.modelchain[0].model[0]:
            self.states[0][0].set(safelog(1-phred[0]),0)
        else:
            self.states[0][0].set(safelog(phred[0] / ( len(self.modelchain[0].alphabet) - 1) ),0)

        for idx in range(1,self.seqlen):
            for st in range(len(self.modelchain)):
                # stay in same state
                stepProb =  safelog(1 - self.modelchain[st].nextState) + safelog(P_X_given_Y_statekeep(self.modelchain[st], sequence[idx], sequence[idx-1], phred[idx], phred[idx-1]) ) #safelog(P_X_given_Y_statekeep(self.modelchain[st],sequence[idx],sequence[idx-1]))
                totalProb = stepProb + self.states[idx-1][st].value
                if totalProb > self.states[idx][st].value:
                    self.states[idx][st].set(totalProb,st)
                # transfer to next state
                if st+1 < len(self.modelchain):
                    #print(P_X_given_Y_stateswitch(self.modelchain[st], self.modelchain[st+1],sequence[idx],sequence[idx-1],phred[idx],phred[idx-1]))
                    stepProb = safelog(self.modelchain[st].nextState) + safelog(P_X_given_Y_stateswitch(self.modelchain[st], self.modelchain[st+1],sequence[idx],sequence[idx-1],phred[idx],phred[idx-1]))
                    totalProb = stepProb + self.states[idx-1][st].value
                    if totalProb > self.states[idx][st+1].value:
                        self.states[idx][st+1].set(totalProb,st)
                    

    def bestStateSeq(self):

        #for st in range(len(self.modelchain)):
        #    for idx in range(self.seqlen):
        #        print(str(self.states[idx][st].value)[:5]+','+str(self.states[idx][st].modelsrc),end=' ')
        #    print()

        bestValue,bestState = -(10**9),-1

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