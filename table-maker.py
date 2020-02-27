import sys
from scipy import stats

filenames = sys.argv[1]

HMM_PATH = 'HMM/outputs/PHRED_adjusted/'

ALIGN_PATH = 'outputs/alignments/medians/'

TABLE_PATH = 'TABLE.txt'

fnames = []

def summary(data):
    result = []
    assert len(data) > 0
    for block in range(len(data[0])):
        data.sort(key=lambda x: x[block])
        result.append(data[len(data)//2][block])
    
    return '/'.join([str(x) for x in result])


def mosaicism(data):
    
    # use middle 80% 
    INSTABILITY_THRESHOLD = 0.1

    result = []
    assert len(data) > 0
    for block in range(len(data[0])):
        data.sort(key=lambda x: x[block])
        result.append( (data[int(len(data)*INSTABILITY_THRESHOLD)][block], data[int(len(data)*(1-INSTABILITY_THRESHOLD) )][block]) )

    return '/'.join(  ['-'.join([str(x) for x in block])for block in result ] )

def wilcoxon(data):
    X = [ params[0] for params in data ]
    Z = [ params[-1] for params in data ]
    w, p = stats.wilcoxon(X,Z,alternative='greater')
    return p

with open(filenames,'r') as f:
    fnames = [ line.strip() for line in f.readlines() ]

with open(TABLE_PATH,'w+') as table:

    for fname in fnames:

        HMM_file = HMM_PATH + fname + '.HMM.txt'
        align_file = ALIGN_PATH + fname + '.txt'

        HMM_data = []

        with open(HMM_file,'r') as HMM:
            n = int(HMM.readline().strip())

            for _ in range(n):
                HMM_data.append([ int(x) for x in HMM.readline().strip().split() ])

        align_data = []

        #use median

        with open(align_file,'r') as align:
            n = int(align.readline().strip())
            for _ in range(n):
                align_data.append([ int(x) for x in align.readline().strip().split() ])


        # ID, sequence summary, HMM summary, align mosaicism, HMM mosaicism
        print(fname,end=' ',file = table)
        print(summary(align_data),end=' ',file = table)
        print(summary(HMM_data),end=' ',file = table)
        print(mosaicism(align_data),end=' ',file = table)
        print(mosaicism(HMM_data),end=' ',file = table)
        print('%.0E' % wilcoxon(align_data),file = table)