import matplotlib.pyplot as plt

filename = input()

with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    points = []
    for line in lines:
        score,minM = [int(x) for x in line.split()]
        points.append((minM,score))

plt.title(filename)
plt.xlabel("Minimum length")
plt.ylabel("Score")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 4)
plt.show()
