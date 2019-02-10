import matplotlib.pyplot as plt

filename = input()

points = {}

with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    numseq = int(lines[0])
    curline = 1
    for i in range(numseq):
        score,num = [int(x) for x in lines[curline].split()]
        curline += 1
        for k in range(num):
            n,m,l = [int(x) for x in lines[curline].split()]
            if m in points:
                points[m].append(score)
            else:
                points[m] = [score]
            curline += 1

tmp = []

for m in points:
    for score in points[m]:
        tmp.append((m,score))

points = tmp

plt.title(filename)
plt.xlabel("(CCGCTG)M")
plt.ylabel("score")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 9)
plt.show()
