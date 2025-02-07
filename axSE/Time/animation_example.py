# use dft phase to back out the gait percentage

from matplotlib import pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation 
import csv, os

START = 40000
END = 41000
N = END - START
DFT_SIZE = 500
assert(DFT_SIZE < END - START)
FILENAME = r'q.csv'
# PROFILE = [np.cos(2 * np.pi * x) + np.cos(9 * 2 * np.pi * x) \
#            for x in np.linspace(0, 1, N)]
# TIME = [x for x in range(len(PROFILE))]

PROFILE = []
TIME = []
with open(FILENAME) as file:
    reader = csv.reader(file)
    for i, row in enumerate(reader):
        if i > START:
            # PROFILE.append(float(row[1]))
            PROFILE.append(np.arcsin(2*(float(row[1])*float(row[3]) - float(row[2])*float(row[4]))))
            TIME.append(float(row[0]))
        if i > END:
            print(row)
            break

def Euclid_GCD(x, y):
    if x == 0: return y
    elif y == 0: return x
    else:
        r = x % y
        return Euclid_GCD(y, r)

def DFT(x):
    """
    Function to calculate the 
    discrete Fourier Transform 
    of a 1D real-valued signal x
    """

    _N = len(x)
    n = np.linspace(0, _N, _N)
    k = n.reshape((_N,1))
    e = np.exp(-2j * np.pi * k * n / _N)
    
    X = np.dot(e, x)
    print("shape", np.shape(e))
    return X

   
# initializing a figure in 
# which the graph will be plotted
fig = plt.figure() 
   
# marking the x-axis and y-axis
plt.subplot(2,1,1)
s, e = 0, DFT_SIZE
dft = DFT(PROFILE[s:e])[:50]
bar = np.array([np.linalg.norm(x) for x in dft]) * 2 / len(PROFILE)
angles = np.array([np.arctan2(np.imag(x), np.real(x)) for x in dft])
barchart = plt.bar([x for x in range(len(bar))], bar)

plt.subplot(2,1,2)
line, = plt.plot(TIME[s:e], PROFILE[s:e])


# data which the line will 
# contain (x, y)
# def init(): 
#     dft = DFT(PROFILE)[:50]
#     bar = np.array([np.linalg.norm(x) for x in dft]) * 2 / len(PROFILE)
#     angles = np.array([np.arctan2(np.imag(x), np.real(x)) for x in dft])
#     line = plt.bar([x for x in range(len(bar))], bar)
#     return line,
   
def animate(i):
    global s, e, line
    s += 5
    e += 5
    if e > END - START:
        s = 0
        e = DFT_SIZE
    dft = DFT(PROFILE[s:e])[:50]
    bar = np.array([np.linalg.norm(x) for x in dft]) * 2 / len(PROFILE)
    angles = np.array([np.arctan2(np.imag(x), np.real(x)) for x in dft])
    for i, elem in enumerate(barchart):
        elem.set_height(np.linalg.norm(bar[i]))

    line.set_ydata(PROFILE[s:e])
    os.system('cls' if os.name == 'nt' else 'clear')
    i = 0
    biggest = []
    for elem in np.flip(np.sort(bar)):
        biggest.append(list(bar).index(elem))
        print(list(bar).index(elem), elem)
        i += 1
        if i > 5:
            break

    print("GCD = ", Euclid_GCD(biggest[1], biggest[2]))
    # for now, we hardcode. really, this should be the gcd of main freq
    print('\nPercent gait =', str(round((np.pi + np.arctan2(dft[4].imag, dft[4].real)) / (2 * np.pi) * 100)) + '%')

anim=FuncAnimation(fig,animate,repeat=True,blit=False,frames=100,
                             interval=100)

plt.show()