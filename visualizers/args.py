import argparse

ARGS = ['filename','alpha','param_displayed','param_limits','score_limits','param_name','point_width']
param_names = ['(CTG)N','CCGCTG(M)','CTG(L)']

def add_args(parser):
    parser.add_argument('filename', help = 'input file')
    parser.add_argument('-a', '--alpha', type = float, default = 1, help = 'alpha value of plotted points in (0,1]')
    parser.add_argument('-p', '--param_displayed', type = int, choices = [0,1,2], default = 1, help = 'index of parameter to display')
    parser.add_argument('-l', '--limits', nargs=2, type = int, dest = 'param_limits', help = 'lower and upper bound of displayed (parameter) values')
    parser.add_argument('-s', '--score', nargs=2, type = int, dest = 'score_limits', help = 'lower and upper bound of displayed scores')
    parser.add_argument('-n', '--param_name', type = str, help = 'parameter name to display on plot axis')
    parser.add_argument('-w', '--width', type = float, default = 1, dest = 'point_width', help = 'width of points on plot')

def args_init():
    argparser = argparse.ArgumentParser()
    add_args(argparser)
    args = argparser.parse_args()
    retval = {name : getattr(args, name) for name in ARGS}
    
    assert(retval['alpha'] > 0 and retval['alpha'] <= 1)

    if retval['param_name'] == None:
        retval['param_name'] = param_names[retval['param_displayed'] ]

    return retval