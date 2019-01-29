import matplotlib.pyplot as plt

filename = input()

with open(filename,'r') as data:
    lines = [line.strip().split() for line in data.readlines()]

    points = []
    for line in lines:
        if "params" in line:
            score = int(line[0][:-1])
            n,m,l = [int(x) for x in line[-3:]]    
            points.append((m,score))
            print(score,n,m,l)

plt.title(filename)
plt.xlabel("(CCGCTG)M")
plt.ylabel("Score")
plt.scatter([p[0] for p in points], [p[1] for p in points], s = 9)
plt.show()
