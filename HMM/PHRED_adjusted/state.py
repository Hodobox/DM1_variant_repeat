from equations import P_X_given_Y_statekeep

class State:
    def __init__(self, model, transitionProb, ALPHABET='ACTG'):
        self.nextState = transitionProb
        self.alphabet = ALPHABET
        self.occurence = {}
        self.model = model
        self.M_NO_ERROR = {}
        self.ERROR_SUBSTITUTIONS_DEST = {}
        self.start = {}
        self.end = {}

        for x in self.alphabet:
            self.occurence[x] = sum([1 if c is x else 0 for c in self.model]) / len(self.model)
            #self.start[x] = 1-startPHRED if model[0]==x else startPHRED/(len(self.alphabet)-1)
            #self.end[x] = 1-endPHRED if model[-1]==x else endPHRED/(len(self.alphabet)-1)
            self.ERROR_SUBSTITUTIONS_DEST[x] = {y : 0 if x==y else 1/(len(self.alphabet)-1) for y in self.alphabet}

        curmodel = model+model[0]

        for x in self.alphabet:
            self.M_NO_ERROR[x] = {}
            for y in self.alphabet:
                self.M_NO_ERROR[x][y] = 0

        num_pairs = len(curmodel) - 1
        for i in range(num_pairs):
            self.M_NO_ERROR[ curmodel[i] ][ curmodel[i+1] ] += 1

        for x in self.alphabet:
            if x not in self.model:
                for y in self.alphabet:
                    self.M_NO_ERROR[x][y] = 1/len(self.alphabet)
            else:
                total_count = 0
                for y in self.alphabet:
                    total_count += self.M_NO_ERROR[x][y]
                for y in self.alphabet:
                    self.M_NO_ERROR[x][y] /= total_count


        # first order probability matrix

        #self.P_X_Y = {}
        #for x in self.alphabet:
        #    self.P_X_Y[x] = {}
        #    for y in self.alphabet:
        #        self.P_X_Y[x][y] = P_X_given_Y_statekeep(self, x, y)
