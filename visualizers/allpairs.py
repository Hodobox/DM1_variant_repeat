import matplotlib.pyplot as plt

filename = input()

points = {}

with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    for line in lines:
        n,m,l,seq,score = [int(x) for x in line.split()]
        if m in points:
            points[m].append(score)
        else:
            points[m] = [score]

tmp = []

for m in points:
    for score in points[m]:
        tmp.append((m,score))

points = tmp

plt.title(filename)
plt.xlabel("(CCGCTG)M")
plt.ylabel("score")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 1)
plt.show()
