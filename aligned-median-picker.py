import sys
from scipy import stats

filenames = sys.argv[1]
ALIGN_PATH = 'outputs/alignments/' 


fnames = []

with open(filenames,'r') as f:
    fnames = [ line.strip() for line in f.readlines() ]

for fname in fnames:

    
    align_file = ALIGN_PATH + fname + '.txt'
    
    align_data = []

    #use median

    with open(align_file,'r') as align:
        n = int(align.readline().strip())

        for _ in range(n):
            score,num = [int(x) for x in align.readline().strip().split()]

            parametrizations = []

            for seq in range(num):
                parametrizations.append([int(x) for x in align.readline().strip().split()])
                
            parametrizations.sort(key = lambda x: x[1])
            

            align_data.append(parametrizations[ num//2 ])


    with open(ALIGN_PATH + 'medians/' + fname + '.txt','w+') as converted_align:
        print(len(align_data),file=converted_align)
        for d in align_data:
            print(' '.join([str(x) for x in d]),file=converted_align)
