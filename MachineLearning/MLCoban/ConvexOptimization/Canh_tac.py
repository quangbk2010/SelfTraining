#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep  9 16:01:29 2017

@author: quangbk2010
Find: (x, y) = arg (5x + 3y)
s.t.x + y <= 10; 2x + y <= 16; x + 4y <= 32; x, y > 0

Rewrite constraints: Gx <= h
G =
|1  1|
|2  1|
|1  4|
|-1 0|
|0 -1|
h =
|10|
|16|
|32|
|0 |
|0 |
"""

from cvxopt import matrix, solvers
c = matrix ([-5., -3.])
G = matrix ([[1., 2., 1., -1., 0.], [1., 1., 4., 0., -1.]])
h = matrix ([10., 16., 32., 0., 0.])

#solvers.options ['show_progress'] = False
sol = solvers.lp (c, G, h)

print (sol['x'])

