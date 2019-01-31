import matplotlib.pyplot as plt

filename = input()

mcount = {}

with open(filename,'r') as data:
    lines = [line.strip().split() for line in data.readlines()]

    points = []
    for line in lines:
        score,n,m,l = [int(x) for x in line]
        if m < 190 and m > 30:
            if m in mcount:
                mcount[m] += 1
            else:
                mcount[m] = 1

points = [(m,mcount[m]) for m in mcount]
#print(mcount)
plt.title(filename)
plt.xlabel("M")
plt.ylabel("Number of best-scores")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 9)
plt.show()
