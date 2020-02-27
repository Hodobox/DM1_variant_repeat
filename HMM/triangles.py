import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init

args = args_init()
used_args = ['filename','point_width','adjust_length','triangles']
filename,point_width,adjust_length,triangles = [ args[name] for name in used_args ]

with open(filename,'r') as scores:
	lines = [ line.strip() for line in scores.readlines() ]

idx = 0
datasets = []
points = defaultdict(list)
while idx < len(lines):
	name,repeats = lines[idx].split()
	repeats = int(repeats)
	idx += 1
	dataset = []
	for i in range(repeats):
		scoring = lines[idx].split()
		idx += 1
		variant = scoring[2][:-1]
		progenitor_length = int(scoring[3].split('-')[0])
		if adjust_length:
			progenitor_length *= len(variant)
		instability = int(scoring[4]) 
		dataset.append([variant,progenitor_length,instability])
		points[variant].append((progenitor_length,instability))
	datasets.append(dataset)

print(points)
plt.title(filename)
plt.xlabel("progenitor length")
maxx,maxy=0,0
plt.ylabel("instability")

for rep in points:
	if rep == "CTG":
		colour = 'b'
	else:
		colour = 'r'
	plt.scatter([ p[0] for p in points[rep] ], [ p[1] for p in points[rep] ], c=colour, s = point_width)
	maxx = max(maxx, max([ p[0] for p in points[rep] ]) )
	maxy = max(maxy, max([ p[1] for p in points[rep] ]) )
plt.xlim(0,maxx+1)
plt.ylim(0,maxy+1)


if triangles:
	which = -1
	try:
		which = int(input())
	except:
		pass

	for i,dataset in enumerate(datasets):
		if len(dataset) != 3:
			continue
		if [dataset[0][0],dataset[1][0],dataset[2][0]] != ["CTG","CCGCTG","CTG"]:
			continue
		
		if which == -1 or which == i:
			xdata = [ repeat[1] for repeat in dataset ]
			xdata.append(dataset[0][1])
			ydata = [ repeat[2] for repeat in dataset ]
			ydata.append(dataset[0][2])
			plt.plot(xdata,ydata)

	plt.show()
