sequences = []

with open('sequences/githubdata.fasta','r') as data:
	lines = data.readlines()
	current = ''
	for line in lines:
		line = line.strip()
		if line[0] == '>': # new sequence
			if len(current):
				sequences.append(current)
				current = ''
		else:
			current += line

	if len(current):
		sequences.append(current)

with open('sequences/real.txt', 'w+') as output:
	output.write(str(len(sequences)) + "\n")
	for seq in sequences:
		output.write(seq + "\n")	
