import argparse

def add_args(parser):
    parser.add_argument('filename', help = 'input file')
    parser.add_argument('-a', '--alpha', type = float, help = 'alpha value of plotted points in (0,1]')
    parser.add_argument('-p', '--param_displayed', type = int, choices = [0,1,2], help = 'index of parameter to display')
    parser.add_argument('-l', '--limits', nargs=2, type = int, help = 'lower and upper bound of displayed (parameter) values')
    parser.add_argument('-s', '--score', nargs=2, type = int, help = 'lower and upper bound of displayed scores')

filename = None
score_limits = None
param_limits = None
alpha = 1
param_displayed = 1
param_names = ['(CTG)N','(CCGCTG)M','(CTG)L']

def args_init():
    global filename, score_limits, param_limits, alpha, param_displayed, param_names
 
    argparser = argparse.ArgumentParser()
    add_args(argparser)
    args = argparser.parse_args()
 
    filename = args.filename
 
    param_displayed = 1
    if args.param_displayed != None:
        param_displayed = args.param_displayed

    alpha = 1
    if args.alpha != None:
        assert args.alpha > 0 and args.alpha <=1
        alpha = args.alpha

    param_limits = None
    if args.limits != None:
        param_limits = args.limits

    score_limits = None
    if args.score != None:
        score_limits = args.score