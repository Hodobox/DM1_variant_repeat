from equations import *

#def likelyhood(modelstate, sequence, phred):
#    probability = modelstate.start[sequence[0]]
#    phred = phredvalues(phred)
#
#    for i in range(len(sequence)-1):
#        probability *= P_X_given_Y(modelstate,sequence[i+1], sequence[i])
#
#    return probability

def phredvalues(PHRED):
    res = []
    for phred in PHRED:
        Q = ord(phred)-33
        # Q = -10log_10(p)
        # -Q/10 = log_10(p)
        # 10^(-Q/10) = p
        p = 10 ** (-Q/10)
        res.append(p)
    return res