# -*- coding: utf-8 -*-
"""
Created on Mon Jul 24 18:09:26 2017

@author: quang
"""

import numpy as np
import matplotlib.pyplot as plt
from sklearn import neighbors, datasets

'''
    Load data points and show data samples
'''
iris = datasets.load_iris ()
iris_X = iris.data
iris_y = iris.target
print ('Number of classess:    %d' %len (np.unique (iris_y)))
print ('Number of data points: %d' %len(iris_y))

X0 = iris_X[iris_y == 0, :]
print ('\nSamples from class 0: \n', X0[:5,:])

X1 = iris_X[iris_y == 1, :]
print ('\nSamples from class 0: \n', X1[:5,:])

X2 = iris_X[iris_y == 2, :]
print ('\nSamples from class 0: \n', X2[:5,:])

'''
    Devide the data set -> train data set and test data set
'''
from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split (iris_X, iris_y, test_size=50)
print ('Training size: %d' %len(y_train))
print ('Test size:     %d' %len(y_test))

'''
    Predict labels for test data set using builtin method in scikit learn library
'''
clf = neighbors.KNeighborsClassifier (n_neighbors=1, p=2) # K = 1 (1NN), p -> distance calculation method: p==2 -> euclidean distance
clf.fit (X_train, y_train) # fit the model using X as training data and y as target values
y_pred = clf.predict (X_test)

print ('Print results for 20 test data points:')
print ('Predicted labels:', y_pred[20:40])
print ('Ground truth:    ', y_test[20:40])

'''
    Accuration Eveluation 
'''
from sklearn.metrics import accuracy_score
print ('Accuracy of 1NN: %.2f %%' %(100*accuracy_score (y_test, y_pred)))

# Try with 10NN
clf = neighbors.KNeighborsClassifier (n_neighbors=10, p=2) 
clf.fit (X_train, y_train) 
y_pred = clf.predict (X_test)
print ('Accuracy of 10NN: %.2f %%' %(100*accuracy_score (y_test, y_pred))) # the result may vary over time

# Try with 10NN, using weights: distance
clf = neighbors.KNeighborsClassifier (n_neighbors=10, p=2, weights='distance') 
clf.fit (X_train, y_train) 
y_pred = clf.predict (X_test)
print ('Accuracy of 10NN (1 / distance weights): %.2f %%' %(100*accuracy_score (y_test, y_pred))) # the results may vary over time, sometimes the results is worse than the 'uniform' weight

# Try with 10NN, using weights: customize
def my_weight (distances):
    sigma2 = .5
    return np.exp (-distances**2/sigma2)
clf = neighbors.KNeighborsClassifier (n_neighbors=10, p=2, weights=my_weight) 
clf.fit (X_train, y_train) 
y_pred = clf.predict (X_test)
print ('Accuracy of 10NN (customized weights): %.2f %%' %(100*accuracy_score (y_test, y_pred))) # the results may vary over time, sometimes the results is worse than the 'uniform' weight


