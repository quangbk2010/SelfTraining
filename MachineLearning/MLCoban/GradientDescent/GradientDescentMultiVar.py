"""
Created on Thu Jul 27 13:58:11 2017

@author: quang
"""

# to support both python 2 and python 3
from __future__ import division, print_function, unicode_literals
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
np.random.seed (2)

X = np.random.rand (1000, 1)
y = 4 + 3 * X + .2*np.random.randn (1000, 1) # noise added

# Building Xbar
one = np.ones ((X.shape[0], 1))
Xbar = np.concatenate ((one, X), axis = 1)

A = np.dot (Xbar.T, Xbar)
b = np.dot (Xbar.T, y)
w_lr = np.dot (np.linalg.pinv (A), b)
print ('Solution found by formula: w =', w_lr.T)

# Display result
w= w_lr
w_0 = w[0][0]
w_1 = w[1][0]
x0 = np.linspace (0, 1, 2)
y0 = w_0 + w_1 * x0

# Draw the fitting line
plt.plot (X.T, y.T, 'b.') # data
plt.plot (x0, y0, 'y', linewidth = 2) # the fitting line
plt.axis ([0, 1, 0, 10])
plt.show()

def grad (w):
    N = Xbar.shape[0]
    return 1/N * Xbar.T.dot (Xbar.dot (w) - y)

def myGD (w_init, grad, eta):
    w = [w_init]
    for i in range (100):
        w_new = w[-1] - eta * grad (w[-1])
        w.append (w_new)
        if np.linalg.norm (grad(w_new)) / len (w_new) < 1e-3:
            break
        
    
    return (w, i)

w_init = np.array ([[2], [1]])
(w, iter_num) = myGD (w_init, grad, 1)

print ('Solution found by GD: w =', w[-1].T, ', after %d iteration.' %(iter_num + 1))