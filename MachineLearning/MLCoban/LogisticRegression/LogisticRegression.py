#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 3 2312:33 2017

@author: quangbk2010

Logistic sigmoid regression:
    - Problem: Given labeled training set, finding outputs as probability
      
    - Idea: Using sigmoid function as the activation function and optimize the loss function using SGD.
        + y: vector of actual labels of training points
        + sigmoid function of x (f (w.T * xi) ), actually is P (yi = 1|xi, w) 
    - Note: The boundary of Logistic regression model is linear. Because
        The boundary of Logistic regression model is for probability (differiate with actual values of Linear Regression).
      
"""

from __future__ import division, print_function, unicode_literals
import numpy as np
import matplotlib.pyplot as plt
np.random.seed (2)

# Initiate training data
X = np.array ([[0.50, 0.75, 1.00, 1.25, 1.50, 1.75, 1.75, 2.00, 2.25, 2.50, 
              2.75, 3.00, 3.25, 3.50, 4.00, 4.25, 4.50, 4.75, 5.00, 5.50]])
y = np.array([0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1])

# Extend data
X = np.concatenate ((np.ones ((1, X.shape[1])), X), axis = 0)

def sigmoid (s):
    return 1 / (1 + np.exp(-s))

def logistic_sigmoid_regression (X, y, w_init, eta):
    w = [w_init]
    N = X.shape[1]
    d = X.shape[0]
    epoc = 0
    epoc_num = 1000
    iter_check_w = 0
    iter_check_w_num = 20
    
    for epoc in range (epoc_num):
        rd_id = np.random.permutation (N)
        
        for i in rd_id:
            xi = X[:, i].reshape (d, 1)
            yi = y[i]
            zi = sigmoid (np.dot (w[-1].T, xi))
            w_new = w[-1] + eta * (yi - zi) * xi
            iter_check_w += 1
            w.append(w_new)
            
            # Check stoping condition
            if iter_check_w % iter_check_w_num == 0:
                #if np.linalg.norm (w[-1]-w[-2]) < 1e-3:
                if np.linalg.norm(w_new - w[-iter_check_w_num]) < 1e-3:
                    return (w, epoc, iter_check_w)
                
    return (w, epoc, iter_check_w)

def logistic_sigmoid_regression2(X, y, w_init, eta, tol = 1e-3, max_count = 10000):
    w = [w_init]    
    it = 0
    N = X.shape[1]
    d = X.shape[0]
    count = 0
    check_w_after = 20
    while count < max_count:
        # mix data 
        mix_id = np.random.permutation(N)
        for i in mix_id:
            xi = X[:, i].reshape(d, 1)
            yi = y[i]
            zi = sigmoid(np.dot(w[-1].T, xi))
            w_new = w[-1] + eta*(yi - zi)*xi
            count += 1
            # stopping criteria
            if count%check_w_after == 0:                
                if np.linalg.norm(w_new - w[-check_w_after]) < tol:
                    return (w, count)
            w.append(w_new)
    return (w, count)
  
w_init = np.random.randn (X.shape[0], 1)
(w, epoc, iter_check_w) = logistic_sigmoid_regression (X, y, w_init, .05)
print ('Solution found by Logistic reression: w =', w[-1], 'after %d epoch, %d iteration' %(epoc, iter_check_w))

(w, iter_check_w) = logistic_sigmoid_regression2 (X, y, w_init, .05)    
print ('Solution found by Logistic reression2: w =', w[-1], 'after %d iteration' %iter_check_w)

# Predicted output (y) based on the sigmoid function
print (sigmoid (np.dot (w[-1].T, X)))

# Show results 
X0 = X[1, np.where (y == 0)][0]
y0 = y[np.where (y == 0)]
X1 = X[1, np.where (y == 1)][0]
y1 = y[np.where (y == 1)]

plt.plot (X0, y0, 'ro', markersize = 8)
plt.plot (X1, y1, 'bs', markersize = 8)

sigmoid_x = np.linspace (0, 6, 1000)
w0 = w[-1][0][0]
w1 = w[-1][1][0]
threshold = -w0 / w1
sigmoid_y = sigmoid (w0 + w1 * sigmoid_x)
boundary_y = w0 + w1 * sigmoid_x

plt.axis ([-2, 8, -1, 2])
plt.plot (sigmoid_x, sigmoid_y, 'g-', markersize = 2)
plt.plot (sigmoid_x, boundary_y, 'k-', markersize = 2)
plt.plot (threshold, .5, 'y^', markersize = 8)
plt.xlabel ('studying hours')
plt.ylabel ('Predicted probability of pass')

plt.show()






