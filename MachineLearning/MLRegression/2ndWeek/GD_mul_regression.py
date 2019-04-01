# -*- coding: utf-8 -*-
"""
Created on Fri Sep  1 16:18:40 2017

@author: quang
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import linear_model

#from LinearRegression_multi_feature import plot_data_label, plot_fitting_line, get_training_data, get_training_label, get_test_data, get_test_label

def get_training_data (input_features):
    '''
    input_feature: an array contains name of features (coresponds to a matrix, with each row is a data point)
    output: name coressponds to training label
    => return: input matrix X, and output vector y
    '''
    featureNo = len (input_features)
    X = np.array ([train_data[input_features[0]]]).T
    for i in range (1, featureNo):
        if input_features[i] in headers:
            X = np.concatenate ((np.array ([train_data[input_features[i]]]).T, X), axis = 1) # Input matrix (each column represents values of a feature, each row represents a data point)
        else:
            if input_features[i] == 'bedrooms_squared':
                bedrooms_squared_feature = train_data['bedroomNo'] ** 2 # elementwise
                X = np.concatenate ((np.array ([bedrooms_squared_feature]).T, X), axis = 1)
            elif input_features[i] == 'bed_bath_rooms':
                bed_bath_rooms = train_data['bedroomNo'] * train_data['bathroomNo'] # elementwise
                X = np.concatenate ((np.array ([bed_bath_rooms]).T, X), axis = 1)
            elif input_features[i] == 'log_sqft_living':
                log_sqft_living = np.log10 (train_data['sqft_living'])
                X = np.concatenate ((np.array ([log_sqft_living]).T, X), axis = 1)
            elif input_features[i] == 'lat_plus_long':
                lat_plus_long = train_data['lat'] + train_data['long']
                X = np.concatenate ((np.array ([lat_plus_long]).T, X), axis = 1)
    
    return X
    
def get_training_label (output):
    y = np.array ([train_data[output]]).T # Output vector: column
    return y
    
def get_test_data (input_features):
    '''
    input_feature: an array contains name of features (coresponds to a matrix, with each row is a data point)
    output: name coressponds to training label
    => return: input matrix X, and output vector y
    '''
    featureNo = len (input_features)
    X = np.array ([test_data[input_features[0]]]).T
    for i in range (1, featureNo):
        if input_features[i] in headers:
            X = np.concatenate ((np.array ([test_data[input_features[i]]]).T, X), axis = 1) # Input matrix (each column represents values of a feature, each row represents a data point)
        else:
            if input_features[i] == 'bedrooms_squared':
                bedrooms_squared_feature = test_data['bedroomNo'] ** 2 # elementwise
                X = np.concatenate ((np.array ([bedrooms_squared_feature]).T, X), axis = 1)
            elif input_features[i] == 'bed_bath_rooms':
                bed_bath_rooms = test_data['bedroomNo'] * test_data['bathroomNo'] # elementwise
                X = np.concatenate ((np.array ([bed_bath_rooms]).T, X), axis = 1)
            elif input_features[i] == 'log_sqft_living':
                log_sqft_living = np.log10 (test_data['sqft_living'])
                X = np.concatenate ((np.array ([log_sqft_living]).T, X), axis = 1)
            elif input_features[i] == 'lat_plus_long':
                lat_plus_long = test_data['lat'] + test_data['long']
                X = np.concatenate ((np.array ([lat_plus_long]).T, X), axis = 1)
    
    return X
    
def get_test_label (output):
    y = np.array ([test_data[output]]).T # Output vector: column
    return y

# Read training data from csv train file
headers = ["idNo","date","price","bedroomNo","bathroomNo","sqft_living","sqft_lot","floorNo","waterfront","view","condition","grade","sqft_above","sqft_basement","yr_built","yr_renovated","zipcode","lat","long","sqft_living15","sqft_lot15"]
dtype_dict = {'bathroomNo':float, 'waterfront':int, 'sqft_above':int, 'sqft_living15':float, 'grade':int, 'yr_renovated':int, 'price':float, 'bedroomNo':float, 'zipcode':str, 'long':float, 'sqft_lot15':float, 'sqft_living':float, 'floorNo':str, 'condition':int, 'lat':float, 'date':str, 'sqft_basement':int, 'yr_built':int, 'idNo':str, 'sqft_lot':int, 'view':int}
train_data = pd.read_csv ('../Data/kc_house_train_data.csv', names = headers, dtype = dtype_dict, header = 0)

# Read the test data from csv test file
test_data = pd.read_csv ('../Data/kc_house_test_data.csv', names = headers, dtype = dtype_dict, header = 0)

input_features = ['sqft_living']#, 'bedroomNo', 'bathroomNo', 'lat', 'long']
output = 'price'
X = get_training_data (input_features)
y = get_training_label (output)
# Building Xbar
one = np.ones ((X.shape[0], 1))
Xbar = np.concatenate ((one, X), axis = 1)
print ('Xbar:', Xbar)

def grad (w, Xbar):
    N = Xbar.shape[0]
    return 1/N * Xbar.T.dot (Xbar.dot (w) - y)

def gradient_descent_mul_regression (w_init, Xbar, eta, tolerance):
    w = [w_init]
    for i in range (1000000):
        print ('i = ', i)
        w_new = w[-1] - eta * grad(w[-1], Xbar)
        w.append (w_new)
        print ('w[-1]: ', w[-1])
        if np.linalg.norm (grad (w[-1]) / len (w[-1])) < tolerance:
            break
    return (w, i)

'''
# Using single feature
w_init = np.array ([[-47000.], [270.]])#, [0], [0], [0], [0]]) # Equal with size of input_features + 1
eta = 7e-12
tolerance = 2.5e7
(w, iter_num) = gradient_descent_mul_regression (w_init, eta, tolerance)

print ('Solution found by GD: w =', w[-1])'''

'''
Result:
i =  93015
w[-1]:  [[-46999.89100827]
 [   272.31686086]]
i =  93016
w[-1]:  [[-46999.89100813]
 [   272.31721091]]
i =  93017
w[-1]:  [[-46999.89100799]
 [   272.31756094]]
i =  93018
w[-1]:  [[-46999.89100785]
 [   272.31791096]]
i =  93019
w[-1]:  [[-46999.89100771]
 [   272.31826096]]
Solution found by GD: w = [[-46999.89100771]
 [   272.31826096]]
'''


# Using multiple features
input_features = ['sqft_living', 'sqft_living_15']
X = get_training_data (input_features)
# Building Xbar
one = np.ones ((X.shape[0], 1))
Xbar = np.concatenate ((one, X), axis = 1)
print ('Xbar:', Xbar)
w_init = np.array ([[-100000.], [1.], [1.]])#, [0], [0], [0], [0]]) # Equal with size of input_features + 1
eta = 4e-12
tolerance = 1e9
#(w, iter_num) = gradient_descent_mul_regression (w_init, Xbar, eta, tolerance)

#print ('Solution found by GD: w =', w[-1])