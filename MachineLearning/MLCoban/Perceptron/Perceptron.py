#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 3 15:35:33 2017

@author: quangbk2010

Perceptron Learning Algorithm:
    - Problem: Given 2 labeled classes, finding a line, a plane or hyperplane that divide the set of points into
      2 parts (assumes this exists).
    - Idea: Begin with an initiatial plane, over each iterate it is closer to the optimum one (based on the loss function 
      (using SGD)).   
      
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.distance import cdist
np.random.seed (2)

# Generate data
means = [[2, 2], [4, 2]]
cov = [[.3, .2], [.2, .3]]
N = 8
"""X0 = np.random.multivariate_normal (means[0], cov, N).T
X1 = np.random.multivariate_normal (means[1], cov, N).T"""
X0 = np.array (([[6, 2], [3,1], [6, 2], [4, 3],[2, 3],[1, 5],[1, 6],[3, 4]])).T
X1 = np.array (([[11, 6],[4, 10],[2, 9],[3, 8],[6, 9],[8, 6],[8, 8],[6, 6]])).T

plt.plot (X0[0, :], X0[1, :], 'or')
plt.plot (X1[0, :], X1[1, :], 'ob')


X = np.concatenate ((X0, X1), axis = 1)
y = np.concatenate ((np.ones ((1, N)), -1 * np.ones ((1, N))), axis = 1)
# Xbar
X = np.concatenate ((np.ones ((1, 2* N)), X), axis = 0)

# Functions
# Compute output based on input x and weihts w
def h(w, x):
    return np.sign (np.dot (w.T, x))

# Check the convergence by coparing h(w,x) and the ground truth y
def has_converged (X, y, w):
    return np.array_equal (h (w, X), y)

# PLA 
def perceptron (X, y, w_init):
    w = [w_init]
    N = X.shape[1]
    d = X.shape[0]
    mis_points = []
    
    while True:
        mix_id = np.random.permutation (N)
        
        for i in range (N):
            xi = X[:, mix_id[i]].reshape (d, 1)
            yi = y[0, mix_id[i]]
            
            if h(w[-1], xi)[0] != yi: # Misclassified point
                mis_points.append (mix_id[i])
                w_new = w[-1] + yi * xi
                w.append (w_new)
        
        if has_converged (X, y, w[-1]):
            break
        
    return (w[-1], mis_points)

d = X.shape[0]
w_init = np.random.randn (d, 1)
(w, m) = perceptron (X, y, w_init)

    
    
print ('w:', w)
# test with other w:
w = np.array ([[10], [-1], [-1]])

x_ = np.linspace (0, 15, 100)
y_= -(w[0][0] + w[1][0] * x_) / w[2][0]
plt.plot (x_, y_)
plt.show()
    
    
