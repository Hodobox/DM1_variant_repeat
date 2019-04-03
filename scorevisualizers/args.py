import argparse

ARGS = ['filename','alpha','param_displayed','X_limits','Y_limits','score_limits','param_name','point_width','matching_scales','square_plot','zero']
param_names = ['(CTG)N','CCGCTG(M)','CTG(L)']

def add_args(parser):
    parser.add_argument('filename', help = 'input file')
    parser.add_argument('-a', '--alpha', type = float, default = 1, help = 'alpha value of plotted points in (0,1]')
    parser.add_argument('-p', '--param_displayed', type = int, choices = [0,1,2], default = 1, help = 'index of parameter to display')
    parser.add_argument('-s', '--score', nargs=2, type = int, dest = 'score_limits', help = 'lower and upper bound of displayed scores')
    parser.add_argument('-n', '--param_name', type = str, help = 'parameter name to display on plot axis')
    parser.add_argument('-w', '--width', type = float, default = 1, dest = 'point_width', help = 'width of points on plot')
    parser.add_argument('-m', '--matching_scales', action = 'store_true', help = 'set scales of the two axis to be the same')
    parser.add_argument('-z', '--zero', action = 'store_true', help = 'axes start at 0')
    parser.add_argument('-S', '--square', dest='square_plot', action='store_true', help= 'if set, X and Y axis will have the same number of pixels')
    parser.add_argument('-X', '--X_limits', nargs = 2, type = int, help = 'set lower and upper bound for X values')
    parser.add_argument('-Y', '--Y_limits', nargs = 2, type = int, help = 'set lower and upper bound for Y values')
   
def args_init():
    argparser = argparse.ArgumentParser()
    add_args(argparser)
    args = argparser.parse_args()
    retval = {name : getattr(args, name) for name in ARGS}

    assert(retval['alpha'] > 0 and retval['alpha'] <= 1)

    if retval['X_limits'] != None:
        assert(retval['X_limits'][0] <= retval['X_limits'][1])

    if retval['Y_limits'] != None:
        assert(retval['Y_limits'][0] <= retval['Y_limits'][1])

    if retval['param_name'] == None:
        retval['param_name'] = param_names[retval['param_displayed'] ]

    return retval