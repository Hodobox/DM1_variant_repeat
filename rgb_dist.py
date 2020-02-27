import itertools
from collections import OrderedDict
from functools import partial

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
from cycler import cycler

import random

seqnum = int(input())

def filled_hist(ax, edges, values, bottoms=None, orientation='v',
                **kwargs):
    """
    Draw a histogram as a stepped patch.

    Extra kwargs are passed through to `fill_between`

    Parameters
    ----------
    ax : Axes
        The axes to plot to

    edges : array
        A length n+1 array giving the left edges of each bin and the
        right edge of the last bin.

    values : array
        A length n array of bin counts or values

    bottoms : scalar or array, optional
        A length n array of the bottom of the bars.  If None, zero is used.

    orientation : {'v', 'h'}
       Orientation of the histogram.  'v' (default) has
       the bars increasing in the positive y-direction.

    Returns
    -------
    ret : PolyCollection
        Artist added to the Axes
    """
    #print(orientation)
    if orientation not in 'hv':
        raise ValueError("orientation must be in {{'h', 'v'}} "
                         "not {o}".format(o=orientation))

    kwargs.setdefault('step', 'post')
    edges = np.asarray(edges)
    #print(edges)
    values = np.asarray(values)
    if len(edges) - 1 != len(values):
        raise ValueError('Must provide one more bin edge than value not: '
                         'len(edges): {lb} len(values): {lv}'.format(
                             lb=len(edges), lv=len(values)))

    if bottoms is None:
        bottoms = 0
    bottoms = np.broadcast_to(bottoms, values.shape)

    values = np.r_[values, values[-1]]
    bottoms = np.r_[bottoms, bottoms[-1]]
    if orientation == 'h':
        return ax.fill_betweenx(edges, values, bottoms,
                                **kwargs)
    elif orientation == 'v':
        return ax.fill_between(edges, values, bottoms,
                               **kwargs)
    else:
        raise AssertionError("you should never be here")

repeats_plotted = 0

def stack_hist(ax, stacked_data, sty_cycle, bottoms=None,
               hist_func=None, labels=None,
               plot_func=None, plot_kwargs=None):

    global repeats_plotted

    """
    ax : axes.Axes
        The axes to add artists too

    stacked_data : array or Mapping
        A (N, M) shaped array.  The first dimension will be iterated over to
        compute histograms row-wise

    sty_cycle : Cycler or operable of dict
        Style to apply to each set

    bottoms : array, optional
        The initial positions of the bottoms, defaults to 0

    hist_func : callable, optional
        Must have signature `bin_vals, bin_edges = f(data)`.
        `bin_edges` expected to be one longer than `bin_vals`

    labels : list of str, optional
        The label for each set.

        If not given and stacked data is an array defaults to 'default set {n}'

        If stacked_data is a mapping, and labels is None, default to the keys
        (which may come out in a random order).

        If stacked_data is a mapping and labels is given then only
        the columns listed by be plotted.

    plot_func : callable, optional
        Function to call to draw the histogram must have signature:

          ret = plot_func(ax, edges, top, bottoms=bottoms,
                          label=label, **kwargs)

    plot_kwargs : dict, optional
        Any extra kwargs to pass through to the plotting function.  This
        will be the same for all calls to the plotting function and will
        over-ride the values in cycle.

    Returns
    -------
    arts : dict
        Dictionary of artists keyed on their labels
    """
    # deal with default binning function
    if hist_func is None:
        hist_func = np.histogram

    # deal with default plotting function
    if plot_func is None:
        plot_func = filled_hist

    # deal with default
    if plot_kwargs is None:
        plot_kwargs = {}
    #print(plot_kwargs)
    try:
        l_keys = stacked_data.keys()
        label_data = True
        if labels is None:
            labels = l_keys

    except AttributeError:
        label_data = False
        if labels is None:
            labels = itertools.repeat(None)

    if label_data:
        loop_iter = enumerate((stacked_data[lab], lab, s)
                              for lab, s in zip(labels, sty_cycle))
    else:
        loop_iter = enumerate(zip(stacked_data, labels, sty_cycle))

    VALS = []
    LABELS = []
    STYS = []
    JS = []
    EDGES = []
    for j, (data,
        label, sty) in loop_iter:
        if label is None:
            label = 'dflt set {n}'.format(n=j)
        label = sty.pop('label', label)
        vals, edges = hist_func(data)

        sty.update(plot_kwargs)
        JS.append(j)
        VALS.append(vals)
        EDGES.append(edges)
        STYS.append(sty)
        LABELS.append(label)

        #print(sty)
        

    arts = {}
    for j in range(len(VALS)):
        if bottoms is None:
            bottoms = np.zeros_like(vals)
            for i in range(len(bottoms)):
                bottoms[i] = -VALS[j][i] - VALS[j+1][i]
            repeats_plotted += 1
        elif repeats_plotted < 2:
            repeats_plotted += 1
            for i in range(len(bottoms)):
                pass#bottoms[i] = -VALS[j][i]
        top = bottoms + VALS[j]
        sty = STYS[j]
        sty.update(plot_kwargs)
        #print(sty)
        ret = plot_func(ax, EDGES[j], top, bottoms=bottoms,
                        label=LABELS[j], **sty)
        bottoms = top

        arts[label] = ret
    ax.legend(fontsize=10)
    return arts


# set up histogram function to fixed bins
edges = np.linspace(0, seqnum+1, seqnum+1, endpoint=True)
hist_func = partial(np.histogram, bins=edges)

# set up style cycles
color_cycle = cycler(facecolor=plt.rcParams['axes.prop_cycle'][:3])
label_cycle = cycler(label=['CTG','CCGCTG','CTG'])
hatch_cycle = cycler(hatch=['/', '*', '+'])

# Fixing random state for reproducibility
np.random.seed(19680801)

#stack_data = np.random.randn(4, 12250)
stack_data = [ [],[],[] ]
params = []

#for i in range(100):
#	ln = random.randrange(500,600)
#	rgbset = set()
#	
#	rgbset.add( int(np.random.normal(loc=160,scale=60) // 1) )
#	rgbset.add( int(np.random.normal(loc=370,scale=60) // 1) )
#
#	rgb = [0] + [ x for x in rgbset ] + [ln]
#	rgb = list(sorted(rgb))
#	rgb = [ rgb[i]-rgb[i-1] for i in range(1,4)]
#	
#	params.append(rgb)

for i in range(seqnum):
    params.append([int(x) for x in input().split()])
    if params[-1][1] < 1:
        params[-1][0] += params[-1][-1]
        params[-1][-1] = 0

patterns = ['CTG','CCGCTG','CTG']
#params = list(sorted(params,key = lambda x:  sum( [ -x[i]*len(patterns[i]) for i in range(3) ] ) ))
params = list(sorted(params,key = lambda x: -x[2]*len(patterns[2])))

#print(params)


for i in range(len(params)):
    for k in range(0,3):
        for _ in range(params[i][k]):
            for __ in range(len(patterns[k])):
                stack_data[k].append(i+1)

dict_data = OrderedDict(zip((c['label'] for c in label_cycle), stack_data))

fig, ax2 = plt.subplots(1, 1, figsize=(10, 100), tight_layout=True)

#print(stack_data)

arts = stack_hist(ax2, stack_data, color_cycle,
                  hist_func=hist_func, labels = ['CTG','CCGCTG','CTG'],
                  plot_kwargs=dict(edgecolor='g', orientation='h', linewidth=0))

ax2.set_xlabel('bases')
ax2.set_ylabel('seq')

plt.show()