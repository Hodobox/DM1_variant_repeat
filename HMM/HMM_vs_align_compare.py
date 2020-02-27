import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init

args = args_init()
used_args = ['filename','param_displayed','param_name','point_width','X_limits']
filename,param_displayed,param_name,point_width,Xlim = [ args[name] for name in used_args ]

scorefile = input()

HMM_seq_params = []

HMM_lengths = []

with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    numseq = int(lines[0])
    curline = 1
    for i in range(numseq):
        #score,num = [int(x) for x in lines[curline].split()]
        #curline += 1   
        params = [int(x) for x in lines[curline].split()]
        param_value = params[param_displayed]
        HMM_lengths.append( 3*params[0]+6*params[1]+3*params[2] )
        HMM_seq_params.append(param_value)
        curline += 1


align_seq_params = []

align_lengths = []

with open(scorefile,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    numseq = int(lines[0])

    print(numseq,len(HMM_lengths))

    curline = 1
    for i in range(numseq):
        score,num = [int(x) for x in lines[curline].split()]
        curline += 1

        param_set = []
        length_set = []
        for k in range(num):
            params = [int(x) for x in lines[curline].split()]
            param_value = params[param_displayed]
            length_set.append(3*params[0]+6*params[1]+3*params[2])
            param_set.append(param_value)

            curline += 1

        align_seq_params.append(param_set)
        align_lengths.append(length_set)

points = defaultdict(int)
length_div = defaultdict(int)

for i in range(len(HMM_seq_params)):
    closest_param = 1000
    for param in align_seq_params[i]:
        if abs(HMM_seq_params[i]-param) < closest_param:
            closest_param = HMM_seq_params[i]-param

    if Xlim is None or (Xlim[0] <= closest_param and closest_param<= Xlim[1]):
        points[closest_param] += 1


    closest_length = 1000
    for length in align_lengths[i]:
        if abs(HMM_lengths[i]-length) < closest_length:
            closest_length = HMM_lengths[i]-length
    if Xlim is None or (Xlim[0] <= closest_length and closest_length <= Xlim[1]):
        length_div[closest_length] += 1

#print(length_div)

#plt.ylim(-10,10)
plt.title(filename)
plt.ylabel("Number of sequences")

plt.xlabel(param_name + ': HMM output - closest align output')
plt.bar([p for p in points], [points[p] for p in points])

#plt.xlabel(param_name + ' HMM length - align length')
#plt.scatter([p for p in length_div], [length_div[p] for p in length_div], s = point_width)

plt.show()
