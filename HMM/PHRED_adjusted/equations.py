# how likely is it that a randomly chosen character X is a result of an error
# = sum over the characters C of the alphabet of
# [probability that C was there originally] * [probability that it was changed] * [how likely was it to turn into our character]
# divided by all occurences of the character, which is that + when it was there and does not change
def P_Error(modelstate, X, PHRED):
    result = 0
    for originally_there in modelstate.alphabet:
        result += modelstate.occurence[originally_there] * PHRED * modelstate.ERROR_SUBSTITUTIONS_DEST[originally_there][X]
    result = result / (result + modelstate.occurence[X] * (1-PHRED)) 
    return result

# probability that, given that no errors took place, X occured after Y
def P_RealX_given_RealY(modelstate, X, Y):
    return modelstate.M_NO_ERROR[Y][X]

# probability of getting character X when character Y was before it (before errors were introduced)
def P_X_given_RealY(modelstate, X, Y, PHRED_X):
    result = 0
   
    # one option is that no error occured, and X simply occured after Y:
    result += (1-PHRED_X) * P_RealX_given_RealY(modelstate, X, Y)

    # the other option is that an error occured, and X was supposed to be something else:
    for originally_there in modelstate.alphabet:
        result += PHRED_X * modelstate.ERROR_SUBSTITUTIONS_DEST[originally_there][X] * P_RealX_given_RealY(modelstate, originally_there, Y)

    return result

# probability of getting character X after character Y
def P_X_given_Y_statekeep(modelstate, X, Y, PHRED_X, PHRED_Y):
    result = 0

    # probability that the character Y is really itself
    result += (1-P_Error(modelstate,Y, PHRED_Y)) * P_X_given_RealY(modelstate,X,Y, PHRED_X)

    # otherwise, probability that it was something different (but now we know for sure)
    whole_error = 0
    for originally_there in modelstate.alphabet:
        whole_error += modelstate.occurence[originally_there] * modelstate.ERROR_SUBSTITUTIONS_DEST[originally_there][Y]

    for originally_there in modelstate.alphabet:
        result += P_Error(modelstate,Y,PHRED_Y) * ( (modelstate.occurence[originally_there] * modelstate.ERROR_SUBSTITUTIONS_DEST[originally_there][Y]) / whole_error ) * P_X_given_RealY(modelstate, X, originally_there, PHRED_X)

    return result

# fix, its shorthand
def P_X_given_Y_stateswitch(firstState, secondState, X, Y, PHRED_X, PHRED_Y):

    # X had to really be last of firstState, Y had to really be first of secondState

    #print(X,Y,PHRED_X,PHRED_Y)

    if X == firstState.model[-1] and Y == secondState.model[0]:
        return (1-PHRED_X) * (1-PHRED_Y)
    elif X == firstState.model[-1] and Y != secondState.model[0]:
        # had to be an error of secondState.model[0] -> Y
        return (1-PHRED_X) * PHRED_Y * secondState.ERROR_SUBSTITUTIONS_DEST[ secondState.model[0] ][Y]
    elif X != firstState.model[-1] and Y == secondState.model[0]:
        # had to be an error of firstState.model[-1] -> X
        return PHRED_X * firstState.ERROR_SUBSTITUTIONS_DEST[ firstState.model[-1] ][X] * (1-PHRED_Y)
    else:
        # had to be both errors
        return PHRED_X * firstState.ERROR_SUBSTITUTIONS_DEST[ firstState.model[-1] ][X] * PHRED_Y * secondState.ERROR_SUBSTITUTIONS_DEST[ secondState.model[0] ][Y]

