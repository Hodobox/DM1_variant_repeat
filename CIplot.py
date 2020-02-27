import matplotlib.pyplot as plt

font = {'family' : 'sans',
        'weight' : 'normal',
        'size'   : 20}
plt.rc('font', **font)


title = input()
points = []

with open('CI_names.txt','r') as names:
    sample_names = [ line.strip() for line in names.readlines() ]

while True:
    try:
        point = [float(x) for x in input().split()]
        points.append((point[1],point[2]))
    except:
        break

plt.title(title)
plt.xlabel('Correlation coefficient')
plt.ylabel('Sample')
plt.xlim(-0.6,0.2)

plt.yticks(ticks = [x for x in range(len(points),0,-1)],labels = sample_names)

for i in range(len(points)):
    plt.plot(points[i],[len(points)-i,len(points)-i])

fig = plt.gcf()
fig.set_size_inches(13, 8.5,forward = True)
fig.savefig('Figures/CAI_5_3.png', dpi=300)


plt.axvline(x=0, color='k')
plt.show()