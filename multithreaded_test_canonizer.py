# expects test in format
# header line (e.g. testing X sequences vs. Y templates)
# pattern generation line (e.g. candidate generation: 1234 ms)
# several lines in format:
# Template x, sequence y = z ms
# some lines after

filename = input()

lines = []
results = []
index = 2

with open(filename,'r') as test:
	lines = [line.strip().split() for line in test.readlines()]
	results = []
	index = 2
	while lines[index][0] == "Template":
		template = int(lines[index][1][:-1])
		sequence = int(lines[index][3])
		score = int(lines[index][-1])
		results.append([template,sequence,score])

		index += 1


with open(filename,'w') as f:
	results = list(sorted(results))

	print(' '.join(lines[0]),file=f)
	print(' '.join(lines[1]),file=f)
	for res in results:
		print("Template {}, sequence {} = {}".format(res[0],res[1],res[2]),file=f)

	while index < len(lines):
		print(' '.join(lines[index]),file=f)
		index += 1