import sys
from collections import defaultdict

def common(seqs, flank):
	occ = defaultdict(int)
	for seq in seqs:
		if len(seq) > len(flank) and flank in seq:
			index = len(seq)-len(flank)
			while seq[index:index+len(flank)] != flank:
				
				index -= 1
			
			if len(seq) > index+len(flank):
				occ[ seq[index+len(flank)] ] += 1

	best,cnt = 'C',0
	for key in occ:
		if occ[key] > cnt:
			best = key
			cnt = occ[key]
	return best

def contain(seqs, suff):
	res = 0
	for seq in seqs:
		if len(seq) >= len(suff) and suff in seq:
			res += 1
	return res

if len(sys.argv) < 3:
	print("Usage: python3 suffix_referencer.py path/to/inputFile.fastq INITIAL_FLANK")
	exit(0)

pattern = "CTG"
seqs = []

with open(sys.argv[1],'r') as fastq:
	while True:
		header = fastq.readline().strip()
		if len(header) == 0:
			break
		seq = fastq.readline().strip()
		strand = fastq.readline().strip()
		PHRED = fastq.readline().strip()
		assert( min([len(seq),len(strand),len(PHRED)]) > 0 )
		assert(len(PHRED) == len(seq))

		seqs.append(seq)

init_flank = sys.argv[2]

flank = init_flank[:]
threshold = 0.10

while True:

	while contain(seqs,flank)/len(seqs) >= threshold:
		flank += common(seqs,flank) 

	if len(flank) > 100:
		threshold = threshold + 0.05
		flank = init_flank[:]
	else:
		break

outputName = sys.argv[1] + "_derived_reference_suffix.fasta"

#if len(sys.argv) == 3:
#	outputName = sys.argv[2]

with open(outputName,'w') as out:
	lengthlimit = max([len(seq) for seq in seqs])
	step = 150
	length = 0

	while length*len(pattern) < 2*lengthlimit:
		length += step
		ref = pattern * length + flank
		print(">FLANKED_%s_%d" % (pattern, length),file = out)
		print(ref, file = out)

print("Converted %d sequences with flank of length %d into %s" % (len(seqs), len(flank), outputName))