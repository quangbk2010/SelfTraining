# -*- coding: utf-8 -*-
"""
Created on Tue Jul 25 10:57:35 2017

@author: quang
Input: f(x) = x**2 + 5*sin (x)
Ouput: find the global minimum by using Gradient descent (based on local minimum)

- More reference: 
    + https://phvu.net/2012/07/08/gradient-based-learning/
"""

# To support both python 2 and python 3
from __future__ import division, print_function, unicode_literals # Eg. in python2: 8/7 = 1, but in python 3: 8/7=1.142857

#import math
import numpy as np
import matplotlib.pyplot as plt

# Compute the gradient of f(x)
def grad (x):
    return 2*x + 5*np.cos (x)
    
# compute the value of f(x) 
def cost (x):
    return x**2 + 5*np.sin(x)

# Gradient descent main
def myGD1(eta, x0):
    x = [x0]
    for it in range (100):
        x_new = x[-1] -eta * grad(x[-1])
        if abs(grad(x_new)) < 1e-3: # when the gradient is small enough, x_new approximates to x[-1]
            break
        x.append (x_new)
    return (x, it)
        
(x1, it1) = myGD1(.1, -5)
print ('With x1 = %f, cost = %f, obtained after %d iterations' %(x1[-1], cost(x1[-1]), it1))


(x2, it2) = myGD1(.1, 5)
print ('With x1 = %f, cost = %f, obtained after %d iterations' %(x2[-1], cost(x2[-1]), it2))

