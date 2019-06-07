from equations import *

def likelyhood(modelstate, sequence):
    probability = modelstate.start[sequence[0]]

    for i in range(len(sequence)-1):
        probability *= P_X_given_Y(modelstate,sequence[i+1], sequence[i])

    return probability