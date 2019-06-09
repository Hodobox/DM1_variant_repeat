# how likely is it that a randomly chosen character X is a result of an error
# = sum over the characters C of the alphabet of
# [probability that C was there originally] * [probability that it was changed] * [how likely was it to turn into our character]
# divided by all occurences of the character, which is that + when it was there and does not change
def P_Error(modelstate, X):
    result = 0
    for originally_there in modelstate.alphabet:
        result += modelstate.occurence[originally_there] * modelstate.phred * modelstate.ERROR_SUBSTITUTIONS_DEST[originally_there][X]
    result = result / (result + modelstate.occurence[X] * (1-modelstate.phred)) 
    return result

# probability that, given that no errors took place, X occured after Y
def P_RealX_given_RealY(modelstate, X, Y):
    return modelstate.M_NO_ERROR[Y][X]

# probability of getting character X when character Y was before it (before errors were introduced)
def P_X_given_RealY(modelstate, X, Y):
    result = 0
   
    # one option is that no error occured, and X simply occured after Y:
    result += (1-modelstate.phred) * P_RealX_given_RealY(modelstate, X, Y)

    # the other option is that an error occured, and X was supposed to be something else:
    for originally_there in modelstate.alphabet:
        result += modelstate.phred * modelstate.ERROR_SUBSTITUTIONS_DEST[originally_there][X] * P_RealX_given_RealY(modelstate, originally_there, Y)

    return result

# probability of getting character X after character Y
def P_X_given_Y_statekeep(modelstate, X, Y):
    result = 0

    # probability that the character Y is really itself
    result += (1-P_Error(modelstate,Y)) * P_X_given_RealY(modelstate,X,Y)

    # otherwise, probability that it was something different (but now we know for sure)
    whole_error = 0
    for originally_there in modelstate.alphabet:
        whole_error += modelstate.occurence[originally_there] * modelstate.ERROR_SUBSTITUTIONS_DEST[originally_there][Y]

    for originally_there in modelstate.alphabet:
        result += P_Error(modelstate,Y) * ( (modelstate.occurence[originally_there] * modelstate.ERROR_SUBSTITUTIONS_DEST[originally_there][Y]) / whole_error ) * P_X_given_RealY(modelstate, X, originally_there)

    return result

def P_X_given_Y_stateswitch(firstState, secondState, X, Y):
    return firstState.end[Y] * secondState.start[X];

