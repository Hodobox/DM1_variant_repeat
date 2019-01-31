import matplotlib.pyplot as plt

filename = input()

with open(filename,'r') as data:
    lines = [line.strip().split() for line in data.readlines()]

    points = []
    for line in lines:
        score,n,m,l = [int(x) for x in line]
        if m < 190 and m > 30:
            points.append((n,m,l,score))

plt.title(filename)
plt.xlabel("(CCGCTG)M")
plt.ylabel("Score")
plt.scatter([p[1] for p in points], [p[3] for p in points], s = 9)
plt.show()
