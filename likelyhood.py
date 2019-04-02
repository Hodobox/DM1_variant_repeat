# probability that, for any given character, it is not (in our sequence) what it actually was in reality
THREAT = 0.1

# all characters we care about
ALPHABET = 'ACTG'

# how often any given character should occur in our model repeat
OCCURENCE = {
    'A': 0,
    'C': 1/3,
    'T': 1/3,
    'G': 1/3,

}

# how likely it is that our sequence starts off with any given character
START = {
    'A':THREAT/3,
    'C':1-THREAT,
    'T':THREAT/3,
    'G':THREAT/3,
}

# if no errors existed, how likely it is that [after character X][there will be character Y]
M_NO_ERROR = {
    'A': {'A':1,'C':0,'T':0,'G':0},
    'C': {'A':0,'C':0,'T':1,'G':0},
    'T': {'A':0,'C':0,'T':0,'G':1},
    'G': {'A':0,'C':1,'T':0,'G':0},
}

# for each character, how likely it is to 'morph' into another in an error
ERROR_SUBSTITUTIONS_DEST = {
    'A': {'A':0,'C':1/3,'T':1/3,'G':1/3},
    'C': {'A':1/3,'C':0,'T':1/3,'G':1/3},
    'T': {'A':1/3,'C':1/3,'T':0,'G':1/3},
    'G': {'A':1/3,'C':1/3,'T':1/3,'G':0},
}

def init(model):
    global OCCURENCE
    for x in ALPHABET:
        OCCURENCE[x] = sum([1 if c is x else 0 for c in model]) / len(model)
        START[x] = 1-THREAT if x == model[0] else THREAT/3

    model += model[0]

    for x in ALPHABET:
        for y in ALPHABET:
            M_NO_ERROR[x][y] = 0

    num_pairs = len(model) - 1
    for i in range(num_pairs):
        M_NO_ERROR[ model[i] ][ model[i+1] ] += 1

    for x in ALPHABET:
        if x not in model:
            M_NO_ERROR[x][x] = 1
        else:
            total_count = 0
            for y in ALPHABET:
                total_count += M_NO_ERROR[x][y]
            for y in ALPHABET:
                M_NO_ERROR[x][y] /= total_count



# how likely is it that a randomly chosen character X is a result of an error
# = sum over the characters C of the alphabet of
# [probability that C was there originally] * [probability that it was changed] * [how likely was it to turn into our character]
# divided by all occurences of the character, which is that + when it was there and does not change
def P_Error(X):
    result = 0
    for originally_there in ALPHABET:
        result += OCCURENCE[originally_there] * THREAT * ERROR_SUBSTITUTIONS_DEST[originally_there][X]
    result = result / (result + OCCURENCE[X] * (1-THREAT)) 
    return result

# probability that, given that no errors took place, X occured after Y
def P_RealX_given_RealY(X, Y):
    return M_NO_ERROR[Y][X]

# probability of getting character X when character Y was before it (before errors were introduced)
def P_X_given_RealY(X, Y):
    result = 0
   
    # one option is that no error occured, and X simply occured after Y:
    result += (1-THREAT) * P_RealX_given_RealY(X, Y)

    # the other option is that an error occured, and X was supposed to be something else:
    for originally_there in ALPHABET:
        result += THREAT * ERROR_SUBSTITUTIONS_DEST[originally_there][X] * P_RealX_given_RealY(originally_there, Y)

    return result

# probability of getting character X after character Y
def P_X_given_Y(X, Y):
    result = 0

    # probability that the character Y is really itself
    result += (1-P_Error(Y)) * P_X_given_RealY(X,Y)

    # otherwise, probability that it was something different (but now we know for sure)
    whole_error = 0
    for originally_there in ALPHABET:
        whole_error += OCCURENCE[originally_there] * THREAT * ERROR_SUBSTITUTIONS_DEST[originally_there][Y]

    for originally_there in ALPHABET:
        result += P_Error(Y) * ( (OCCURENCE[originally_there] * THREAT * ERROR_SUBSTITUTIONS_DEST[originally_there][Y]) / whole_error ) * P_X_given_RealY(X, originally_there)

    return result

def equal(A, B):
    #print(A, B)
    return abs(A-B) < 0.0000000001

for X in 'ACTG':
    for Y in 'ACTG':
        print("P(" + X + "|" + Y + ") = " + str(P_X_given_Y(X,Y)))

assert P_Error('A') == 1

assert equal(P_Error('C'),(THREAT * (OCCURENCE['T'] * ERROR_SUBSTITUTIONS_DEST['T']['C'] + OCCURENCE['G'] * ERROR_SUBSTITUTIONS_DEST['G']['C'])) / (THREAT * (OCCURENCE['T'] * ERROR_SUBSTITUTIONS_DEST['T']['C'] + OCCURENCE['G'] * ERROR_SUBSTITUTIONS_DEST['G']['C']) + OCCURENCE['C'] * (1-THREAT)) )

assert equal(P_X_given_RealY('A','C'), THREAT * ERROR_SUBSTITUTIONS_DEST['T']['A'] )

assert equal(P_X_given_Y('A','A'), (M_NO_ERROR['C']['T'] * THREAT * ERROR_SUBSTITUTIONS_DEST['T']['A']) )


init('CTG')

for X in 'ACTG':
    for Y in 'ACTG':
        print("P(" + X + "|" + Y + ") = " + str(P_X_given_Y(X,Y)))

assert P_Error('A') == 1

assert equal(P_Error('C'),(THREAT * (OCCURENCE['T'] * ERROR_SUBSTITUTIONS_DEST['T']['C'] + OCCURENCE['G'] * ERROR_SUBSTITUTIONS_DEST['G']['C'])) / (THREAT * (OCCURENCE['T'] * ERROR_SUBSTITUTIONS_DEST['T']['C'] + OCCURENCE['G'] * ERROR_SUBSTITUTIONS_DEST['G']['C']) + OCCURENCE['C'] * (1-THREAT)) )

assert equal(P_X_given_RealY('A','C'), THREAT * ERROR_SUBSTITUTIONS_DEST['T']['A'] )

assert equal(P_X_given_Y('A','A'), (M_NO_ERROR['C']['T'] * THREAT * ERROR_SUBSTITUTIONS_DEST['T']['A']) )



seq = input()

probability = START[seq[0]]

for i in range(len(seq)-1):
    probability *= P_X_given_Y(seq[i+1], seq[i])


print(probability,'CTG')

init('CAG')

probability = START[seq[0]]

for i in range(len(seq)-1):
    probability *= P_X_given_Y(seq[i+1], seq[i])

print(probability,'CAG')