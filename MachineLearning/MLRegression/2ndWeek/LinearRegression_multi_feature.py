# -*- coding: utf-8 -*-
"""
Created on Fri Sep  1 11:59:13 2017

@author: quang
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import linear_model

# Plot fucntions
def plot_data_label (X, y, title,  color_shape, x_label, y_label):
    plt.plot (X, y, color_shape)
    plt.title (title)
    plt.xlabel (x_label)
    plt.ylabel (y_label)
    #plt.show ()
    
def plot_fitting_line (w, color):
    w_0 = w[0][0]
    w_1 = w[1][0]
    x0 = np.linspace (0, 14000, 140000)
    y0 = w_0 + x0 * w_1
    plt.plot (x0, y0, color)

# Read training data from csv train file
headers = ["idNo","date","price","bedroomNo","bathroomNo","sqft_living","sqft_lot","floorNo","waterfront","view","condition","grade","sqft_above","sqft_basement","yr_built","yr_renovated","zipcode","lat","long","sqft_living15","sqft_lot15"]
dtype_dict = {'bathroomNo':float, 'waterfront':int, 'sqft_above':int, 'sqft_living15':float, 'grade':int, 'yr_renovated':int, 'price':float, 'bedroomNo':float, 'zipcode':str, 'long':float, 'sqft_lot15':float, 'sqft_living':float, 'floorNo':str, 'condition':int, 'lat':float, 'date':str, 'sqft_basement':int, 'yr_built':int, 'idNo':str, 'sqft_lot':int, 'view':int}
train_data = pd.read_csv ('../Data/kc_house_train_data.csv', names = headers, dtype = dtype_dict, header = 0)

# Read the test data from csv test file
test_data = pd.read_csv ('../Data/kc_house_test_data.csv', names = headers, dtype = dtype_dict, header = 0)


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

def mul_feature_linear_regression(input_features, output):
    '''
    input_feature: an array contains name of features (coresponds to a matrix, with each row is a data point)
    output: name coressponds to training label
    => return: weights column
    '''
    
    X = get_training_data (input_features)
    y = get_training_label (output)
   
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)
    A = np.dot (Xbar.T, Xbar)
    b = np.dot (Xbar.T, y)    
    w = np.dot (np.linalg.pinv (A), b)
    return (X, y, w)

def mul_feature_linear_regression_using_library (input_feature, output)  :
    
    X = get_training_data (input_features)
    y = get_training_label (output)
         
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)
    
    # Fit the model by linear regression
    regr = linear_model.LinearRegression (fit_intercept = False)
    regr.fit (Xbar, y)
    return regr.coef_.T


def get_residual_sum_of_squares(input_features, output, w):
    """
    Calculate RSS when know input and label of test data, awa weights
    """
    X = get_test_data (input_features)
    y = get_test_label (output)
    
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)

    RSS = sum ((y - np.dot (Xbar, w)) ** 2)
    return RSS

def get_regression_predictions(input_features, w):
    '''
    input_feature: an array contains name of features (coresponds to a matrix, with each row is a data point)
    w: weights
    => return: predicted output
    '''
    X = get_test_data (input_features)
    
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)
    
    predicted_output = np.dot (Xbar, w)
    return (X, predicted_output)   
    
# Build model using ‘sqft_living’, ‘bedrooms’, ‘bathrooms’, ‘lat’, and ‘long’
# Using our method
print ('Build model using: ‘sqft_living’, ‘bedrooms’, ‘bathrooms’, ‘lat’, and ‘long’')
input_features = ['sqft_living', 'bedroomNo', 'bathroomNo', 'lat', 'long']
(X, y, w) = mul_feature_linear_regression (input_features, 'price')
print ('Weight (my method):', w)
RSS = get_residual_sum_of_squares (input_features, 'price', w)
print ('RSS: ', RSS)

# Draw the figure with sqft_living and price:
(X, predicted_output) = get_regression_predictions(input_features, w)
#predicted_output = predicted_output[500:1000, :]
ground_truth = get_test_label ('price')
#ground_truth = ground_truth[500:1000, :]
sqft_living = np.array ([test_data['sqft_living']]).T # Input vector (column) has only one parameter
#sqft_living = sqft_living[500:1000, :]
plot_data_label (sqft_living, predicted_output, 'Predicted price vs ground truth (Build model with multiple features)', 'vr', 'sqft_living', 'price')
plot_data_label (sqft_living, ground_truth, 'Predicted price vs ground truth (Build model with multiple features', 'o', 'sqft_living', 'price')
plt.show ()

# Using library method
w_using_library = mul_feature_linear_regression_using_library (input_features, 'price')
print ('Weight (using lib):', w_using_library)


# Build model using ‘sqft_living’, ‘bedrooms’, ‘bathrooms’, ‘lat’,‘long’, and ‘bed_bath_rooms’
# Using our method
print ('Build model using: ‘sqft_living’, ‘bedrooms’, ‘bathrooms’, ‘lat’,‘long’, and ‘bed_bath_rooms’')
input_features = ['sqft_living', 'bedroomNo', 'bathroomNo', 'lat', 'long', 'bed_bath_rooms']
(X, y, w) = mul_feature_linear_regression (input_features, 'price')
print ('Weight (my method):', w)
RSS = get_residual_sum_of_squares (input_features, 'price', w)
print ('RSS: ', RSS)

# Using library method
w_using_library = mul_feature_linear_regression_using_library (input_features, 'price')
print ('Weight (using lib):', w_using_library)


# Compute RSS
