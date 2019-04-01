# -*- coding: utf-8 -*-
"""
Created on Thu Jul 20 13:37:34 2017

@author: quang
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.distance import cdist # used to calculate the distance  between 2 points in the set
np.random.seed (11) # The random data will be dupplicated at least one time after each 12 generator times.

# Create a set of points follow the normal (or gaussian) distribution with the means (or expectation, and also 
# called as median or mode) of the distribution: (2, 2), (8, 3), (3, 6) and the same covariance matrix is a
# unit matrix (or identity matrix). Each cluster has 500 points (each  point is a line of data matrix)
means = [[2,2], [8, 3], [3, 6]]
cov = [[1, 0], [0, 1]]
N = 500

# Xi coressponds to a cluster
X0 = np.random.multivariate_normal (means[0], cov, N)
X1 = np.random.multivariate_normal (means[1], cov, N)
X2 = np.random.multivariate_normal (means[2], cov, N)

# X is a set of all points
X = np.concatenate ((X0, X1, X2), axis = 0)
K = 3

# An array (size: 1500) stores the initial label of all points
original_label = np.asarray ([0]*N + [1]*N + [2]*N).T

def kmeans_display (X, label):
    #K = np.amax (label) + 1
    X0 = X[label == 0, :]
    X1 = X[label == 1, :]
    X2 = X[label == 2, :]
    
    plt.plot (X0[:, 0], X0[:, 1], 'b^', markersize = 4, alpha = .8)
    plt.plot (X1[:, 0], X1[:, 1], 'go', markersize = 4, alpha = .8)
    plt.plot (X2[:, 0], X2[:, 1], 'rs', markersize = 4, alpha = .8)
    
    #plt.axis ('equal')
    plt.plot()
    plt.show()
    
#kmeans_display (X, original_label)

# Initiate the center
def kmeans_init_centers (X, k):
    # randomly pick k rows of X as initial centers
    return X[np.random.choice (X.shape[0], k, replace=False)] # X.shape[0] = the number of points in X, 
                                        # the first argument of 'choice' function only takes 1-dimensional array
# Assign labels for every points when know the centers
def kmeans_assign_labels (X, centers):
    # Calculate pairwise distances 
    D = cdist (X, centers)
    #print ('D =', D)
    # Return index of the closest center
    return np.argmin (D, axis = 1)
    
# Update centers after each labels re-assignment
def kmeans_update_centers (X, labels, K):
    centers = np.zeros ((K, X.shape[1]))
    #print ('initial centers:', centers)
    
    for k in range (K):
        # Collect all points assigned to the k-th cluster
        Xk = X[labels == k, :]
        #print ('X-%d =' %k,  Xk)
        # Take the average
        centers[k, :] = np.mean (Xk, axis = 0)
        #print ('update_center = ', centers[k, :])
    
    return centers

# Check the stop-point of the problem (using tol: tolerance (#dung sai))
def has_converged (centers, new_centers, tol):
    if tol == 0:
        return np.array_equal (centers, new_centers)
    else:
        return np.allclose (centers, new_centers, tol)
    #return set (tuple (e) for e in centers) == set (tuple (e) for e in new_centers)

# The main of kmeans function
def kmeans (X, K, tol):
    centers = kmeans_init_centers (X, K)
    count = 1
    while True:
        labels = kmeans_assign_labels (X, centers)
        new_centers = kmeans_update_centers (X, labels, K)
        if has_converged (centers, new_centers, tol):
            break
        centers = new_centers
        count += 1
        
    return (centers, labels, count)
    
    

#init_centers = kmeans_init_centers (X, 3)
#labels = kmeans_assign_labels (X, init_centers) 
#update_centers = kmeans_update_centers (X, labels, K)
#
#print (has_converged (init_centers, update_centers))
(centers, labels, count) = kmeans (X, K, 0)
print ('Count:', count)
print ('Centers:', centers)
print ('Labels:', labels)

kmeans_display (X, labels)

(centers, labels, count) = kmeans (X, K, 0.1)
print ('Count:', count)
print ('Centers:', centers)
print ('Labels:', labels)

kmeans_display (X, labels)

# Kmeans clustering uses scikit learn library
def kmeans_scikit (X, K):
    from sklearn.cluster import KMeans
    kmeans = KMeans(n_clusters=3, random_state=0).fit(X)
    print ('Centers found by scikit-learn:', kmeans.cluster_centers_)
    print ('Labels found by scikit-learn:', kmeans.predict(X))
    kmeans_display (X, kmeans.predict(X))
    
kmeans_scikit (X, K)




