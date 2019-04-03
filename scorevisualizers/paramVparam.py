import matplotlib.pyplot as plt
from collections import defaultdict
from args import args_init

print("Enter X-value param as option, Y-value as input")

args = args_init()
used_args = ['filename','zero','alpha','score_limits','X_limits','Y_limits','param_displayed','param_name','point_width','matching_scales','square_plot']
filename,zero,alpha,score_limits,X_limits,Y_limits,param_displayed,param_name,point_width,matching_scales,square_plot = [ args[name] for name in used_args ]

Yparam = int(input("Index of Y-param: "))
Yparam_name = input("Display name of Y-param: ")

points = defaultdict(list)


with open(filename,'r') as data:
    lines = [line.strip() for line in data.readlines()]

    numseq = int(lines[0])
    curline = 1
    for i in range(numseq):
        score,num = [int(x) for x in lines[curline].split()]
        curline += 1
        for k in range(num):
            params = [int(x) for x in lines[curline].split()]
            param_value = params[param_displayed]
            Yparam_value = params[Yparam]

            use_datum = True
            use_datum &= (score_limits == None or (score >= score_limits[0] and score <= score_limits[1]) )
            use_datum &= (X_limits == None or (param_value >= X_limits[0] and param_value <= X_limits[1]) )
            use_datum &= (Y_limits == None or (Yparam_value >= Y_limits[0] and Yparam_value <= Y_limits[1]) )

            if use_datum:
                points[param_value].append(Yparam_value)
            curline += 1

tmp = []
for x in points:
    for y in points[x]:
        tmp.append((x,y))

points = tmp

plt.title(filename)
plt.xlabel(param_name)
plt.ylabel(Yparam_name)
plt.scatter([p[0] for p in points], [p[1] for p in points], s = point_width, alpha = alpha)

print(matching_scales)

if matching_scales:
    x_values = [ p[0] for p in points ]
    y_values = [ p[1] for p in points ]

    minvalue = min(min(x_values),min(y_values))
    maxvalue = max(max(x_values),max(y_values))

    plt.ylim(minvalue,maxvalue)
    plt.xlim(minvalue,maxvalue)
    
if zero:
    plt.ylim(bottom = 0)
    plt.xlim(left = 0)

if square_plot:
    plt.gca().set_aspect('equal', adjustable='box')

plt.show()
