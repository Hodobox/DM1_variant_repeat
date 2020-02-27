import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init

args = args_init()
used_args = ['filename','point_width','adjust_length','matching_scales']
filename,point_width,adjust_length,matching_scales = [ args[name] for name in used_args ]

with open(filename,'r') as scores:
	lines = [ line.strip() for line in scores.readlines() ]

idx = 0
points = defaultdict(list)
while idx < len(lines):
	name,repeats = lines[idx].split()
	repeats = int(repeats)
	idx += 1
	for i in range(repeats):
		scoring = lines[idx].split()
		idx += 1
		variant = scoring[2][:-1]
		progenitor_length = int(scoring[3].split('-')[0])
		if adjust_length:
			progenitor_length *= len(variant)
		
		instability = int(scoring[4])

		if progenitor_length > 0:
			instability = instability / progenitor_length
			#points[variant].append((progenitor_length,instability))
			if repeats == 3 and i != 1:
				points[0].append((progenitor_length,instability))
			elif repeats == 1:
				points[1].append((progenitor_length,instability))


			#if repeats == 1:
			#	index = 3
			#else:
			#	index = i

			#points[index].append((progenitor_length,instability))


print(points)
plt.title(filename)
plt.xlabel("Progenitor length")
maxx,maxy=0,0
plt.ylabel("Instability per base")

for rep in points:
	#if rep == "CTG" or rep == 0:
	#	colour = 'b'
	#elif rep == 'CCGCTG' or rep == 1:
	#	colour = 'r'
	#else:
	#	colour = 'g'
	colours = 'brgkm'
	colour = colours[rep]
	plt.scatter([ p[0] for p in points[rep] ], [ p[1] for p in points[rep] ], c=colour, s = point_width)
	maxx = max(maxx, max([ p[0] for p in points[rep] ]) )
	maxy = max(maxy, max([ p[1] for p in points[rep] ]) )


if matching_scales:
	plt.xlim(0,max(maxx,maxy)+1)
	plt.ylim(0,max(maxx,maxy)+1)
else:
	plt.xlim(0,maxx+5)
	plt.ylim(0,maxy+0.01)

plt.show()
