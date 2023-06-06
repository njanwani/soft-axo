from tqdm import tqdm
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


def plot(dataframe, pergraph):
    keys = dataframe.keys()
    fig, ax = plt.subplots((len(dataframe.columns)) // pergraph)
    fig.set_size_inches(10,1 * len(ax))
    fig.tight_layout()
    idx = 0
    for key in keys:
        ax[int(idx / pergraph)].plot(dataframe.index,dataframe[key].values)
        ax[int(idx / pergraph)].set_title(key.upper())
        idx += 1

    plt.show()


def mean(x):
    return np.mean(x)

def median(x):
    return np.median(x)

def stddev(x):
    return np.std(x)

def first(x):
    return list(x)[0]

def last(x):
    return list(x)[-1]

def maximum(x):
    return np.max(x)

def minimum(x):
    return np.min(x)


def pre_process(df,
                window_size=300,
                funcs=[mean, median, stddev, first, last, maximum, minimum],
                filename=None,
                datalim=float('inf'),
                keys=None):
    if not keys: keys = df.keys()[1:]
    newdf = pd.DataFrame()
    for key in tqdm(keys):
        for func in funcs:
            name = f'{key}_{func.__name__}'
            data = []
            for i in range(min(datalim,len(df[key]) - window_size)):
                data.append(func(df[key].values[i:i+window_size]))
            newdf.insert(len(newdf.columns),column=name,value=data)

    newdf = newdf.set_index(df.index[:min(datalim,len(df[key]) - window_size)])
    return newdf

