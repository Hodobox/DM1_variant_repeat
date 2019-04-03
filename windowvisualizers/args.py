import argparse

ARGS = ['filename','seqnum','repeats']

def add_args(parser):
    parser.add_argument('filename', help = 'input file')
    parser.add_argument('-s', '--seqnum', type = int, default = 1, help = 'index of sequence to use')
    parser.add_argument('-r', '--repeats', type = str, nargs = '*', default = argparse.SUPPRESS, help = 'repeats to plot')

def args_init():
    argparser = argparse.ArgumentParser()
    add_args(argparser)
    args = argparser.parse_args()
    retval = {name : getattr(args, name) for name in ARGS}
    return retval