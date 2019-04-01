#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 5 16:13:33 2017

@author: quangbk2010

Stochastic Gradient Descent:
    - Problem: Compute GD faster than Batch GD
      
    - Idea: Compute gradient (of object function) based on only a sample point but all points in training data set.
    
      + Notice: Stopping criteria 
         . https://machinelearningcoban.com/2017/01/16/gradientdescent2/#-stopping-criteria-dieu-kien-dung
         . https://en.wikipedia.org/wiki/Early_stopping
      
"""

# to support both python 2 and python 3
from __future__ import division, print_function, unicode_literals
import numpy as np
np.random.seed (3)

X = np.random.rand (1000, 1)
y = 4 + 3 * X + .2*np.random.randn (1000, 1) # noise added

# Building Xbar
one = np.ones ((X.shape[0], 1))
Xbar = np.concatenate ((one, X), axis = 1)

def single_grad (w, i):
    xi = Xbar[i, :]
    yi = y[i]
    return (xi * (np.dot (xi, w) - yi)).reshape (2, 1)
def SGD (w_init, eta):
    w = [w_init]
    N = X.shape[0]
    iter_check_w_num = 10
    iter_check_w = 0
    epoc_num_limit = 10
    
    for epoc_num in range (epoc_num_limit):
        # shuffle data
        rd_id = np.random.permutation (N)
        for i in rd_id:
            iter_check_w += 1
            w_new = w[-1] - eta * single_grad (w[-1], i)
            w.append (w_new)
            if (iter_check_w % iter_check_w_num == 0):
                #if np.linalg.norm (w[-1]-w[-2])/len(w_init) < 1e-3:
                if np.linalg.norm (w[-1]-w[-iter_check_w_num])/len(w_init) < 1e-3:
                #if np.linalg.norm (w[-1]-w[-iter_check_w_num]) < 1e-3:
                    return (w, epoc_num, iter_check_w)
                
    return (w, epoc_num, iter_check_w)

# A Tiep code--------
def sgrad(w, i, rd_id):
    true_i = rd_id[i]
    xi = Xbar[true_i, :]
    yi = y[true_i]
    a = np.dot(xi, w) - yi
    return (xi*a).reshape(2, 1)

def SGD2(w_init, eta):
    w = [w_init]
    w_last_check = w_init
    iter_check_w = 10
    N = X.shape[0]
    count = 0
    for it in range(10):
        # shuffle data 
        rd_id = np.random.permutation(N)
        for i in range(N):
            count += 1 
            g = sgrad(w[-1], i, rd_id)
            w_new = w[-1] - eta*g
            w.append(w_new)
            if count%iter_check_w == 0:
                w_this_check = w_new                 
                if np.linalg.norm(w_this_check - w_last_check)/len(w_init) < 1e-3:                                    
                    return (w, it, count)
                w_last_check = w_this_check
    return (w, it, count)

#------------------

w_init = np.array ([[2], [1]])
(w, epoc_num, iter_num) = SGD (w_init, .1)

print ('Solution found by SGD: w =', w[-1], ', after %d epoc and %d iteration.' %(epoc_num, iter_num))

(w, epoc_num, iter_num) = SGD2 (w_init, .1)

print ('Solution found by SGD2: w =', w[-1], ', after %d epoc and %d iteration.' %(epoc_num, iter_num))
                       