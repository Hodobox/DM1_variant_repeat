import sys
files = [
'HMM/outputs/PHRED_adjusted/DMGV14C.HMM.txt',
'HMM/outputs/PHRED_adjusted/GO9.HMM.txt',
'HMM/outputs/PHRED_adjusted/X11_UC56.HMM.txt',
'HMM/outputs/PHRED_adjusted/X12_110.HMM.txt',
'HMM/outputs/PHRED_adjusted/X13_111.HMM.txt',
'HMM/outputs/PHRED_adjusted/X14_112.HMM.txt',
'HMM/outputs/PHRED_adjusted/X15_222.HMM.txt',
'HMM/outputs/PHRED_adjusted/X17_UC83.HMM.txt',
'HMM/outputs/PHRED_adjusted/X18_UC84.HMM.txt',
'HMM/outputs/PHRED_adjusted/X4_14C.HMM.txt']

prefix = sys.argv[1]

for f in files:
    fname = prefix + f
    with open(f,'r') as sample:
        samplesize = int(sample.readline().strip())
        print(samplesize)