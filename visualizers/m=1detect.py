import matplotlib.pyplot as plt

filename = input()

points = {}
detected = {}

sequences = []
with open('sequences/garbagefree.txt') as seqs:
    lines = [line.strip() for line in seqs.readlines()]
    sequences = lines[1:]

with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    seq = 0
    numseq = int(lines[0])
    curline = 1
    for i in range(numseq):
        score,num = [int(x) for x in lines[curline].split()]
        curline += 1
        for k in range(num):
            n,m,l = [int(x) for x in lines[curline].split()]

            if m == 1 and seq not in detected:
                detected[seq] = True
                print(seq)
                print(sequences[seq])

            if m in points:
                points[m] += 1
            else:
                points[m] = 1

            curline += 1
        seq += 1

points = list(sorted( [ (m,points[m])  for m in points ] ))
plt.title(filename)
plt.xlabel("(CCGCTG)M")
plt.ylabel("# of best-scoring templates")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 9)
plt.show()
