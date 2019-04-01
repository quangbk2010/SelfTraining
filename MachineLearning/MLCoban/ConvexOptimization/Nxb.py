#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep  9 16:19:01 2017

@author: quangbk2010

Another example of Linear Programming
"""

from cvxopt import matrix, solvers
c = matrix ([5., 10., 15., 4.])
G = matrix ([[1., 0., -1., 0., 0., 0.], [1., 0., 0., -1., 0., 0.], [0., 1., 0., 0., -1., 0.], [0., 1., 0., 0., 0., -1.]])
h = matrix ([800., 700., 0., 0., 0., 0.])
A = matrix ([[1., 0.], [0, 1.], [1., 0.], [0., 1.]])
b = matrix ([600., 400.])

solvers.options ['show_progress'] = False
sol = solvers.lp (c, G, h, A, b)

print (sol ['x'])