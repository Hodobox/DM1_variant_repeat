import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init

args = args_init()
used_args = ['filename','param_displayed','param_name','point_width']
filename,param_displayed,param_name,point_width = [ args[name] for name in used_args ]

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

points = []
length_div = []

for i in range(len(HMM_seq_params)):
    closest_param = 1000
    for param in align_seq_params[i]:
        if abs(HMM_seq_params[i]-param) < closest_param:
            closest_param = HMM_seq_params[i]-param
    points.append((i,closest_param))


    closest_length = 1000
    for length in align_lengths[i]:
        if abs(HMM_lengths[i]-length) < closest_length:
            closest_length = HMM_lengths[i]-length
        length_div.append((i,closest_length))

plt.title(filename)
plt.xlabel(param_name)
plt.ylabel("Deviance of HMM param from best-scoring align param")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = point_width)
#plt.scatter([p[0] for p in length_div], [p[1] for p in length_div], s = point_width)
plt.show()
