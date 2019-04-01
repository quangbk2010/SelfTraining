#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jul 23 00:15:01 2017

@author: quangbk2010
"""

import numpy as np
from mnist import MNIST
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from display_network import *

mndata = MNIST ('./MNIST/')
mndata.load_testing()
X = mndata.test_images
X0 = np.asarray (X)[:1000, :] / 256.0
X = X0

K = 10

kmeans = KMeans (n_clusters=K).fit (X)

pred_label = kmeans.predict(X)


A = display_network (kmeans.cluster_centers_.T, K, 1)
f1 = plt.imshow (A, interpolation='nearest', cmap='jet')
f1.axes.get_xaxis().setvisible (False)
f1.axes.get_yaxis().setvisible (False)
plt.show()