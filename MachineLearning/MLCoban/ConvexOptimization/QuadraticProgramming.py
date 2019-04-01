#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep  9 17:19:20 2017

@author: quangbk2010

Example of Quadratic Programming

"""

from cvxopt import matrix, solvers

P = matrix([[1., 0.], [0., 1.]])
q = matrix([-10., -10.])
G = matrix([[1., 2., 1., -1., 0.], [1., 1., 4., 0., -1.]])
h = matrix([10., 16., 32., 0., 0])

solvers.options ['show_progress'] = False
sol = solvers.qp (P, q, G, h)

print (sol['x'])