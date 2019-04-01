# -*- coding: utf-8 -*-
"""
Created on Wed Aug 30 14:57:11 2017

@author: quang
"""

import numpy as np
#from pandas import DataFrame, read_csv
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

def simple_linear_regression(input_feature, output):
    '''
    input_feature: name of feature (coresponds to a row of data)
    output: name coressponds to training label
    => return: weights column
    '''
    X = np.array ([train_data[input_feature]]).T # Input vector (column) has only one parameter
    y = np.array ([train_data[output]]).T # Output vector: column
        
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)
    A = np.dot (Xbar.T, Xbar)
    b = np.dot (Xbar.T, y)    
    w = np.dot (np.linalg.pinv (A), b)
    return (X, y, w)

def linear_regression_using_library (input_feature, output)  :
    X = np.array ([train_data[input_feature]]).T # Input vector (column) has only one parameter
    y = np.array ([train_data[output]]).T # Output vector: column
        
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)
    
    # Fit the model by linear regression
    regr = linear_model.LinearRegression (fit_intercept = False)
    regr.fit (Xbar, y)
    return regr.coef_.T
    
# Build model using sqft_living
# Using our method
(X, y, w) = simple_linear_regression ('sqft_living', 'price')
print ('Weight (my method):', w)
w_test = w
plot_data_label (X, y, 'Training data', 'o', 'sqft_living', 'price')
plot_fitting_line (w, 'r')
# Using library method
w_using_library = linear_regression_using_library ('sqft_living', 'price')
print ('Weight (using lib):', w_using_library)
plot_fitting_line (w_using_library, 'y')
plt.show ()

# Build model using bathroomNo
(X, y, w) = simple_linear_regression ('bathroomNo', 'price')
print ('Weight:', w)
plt.axis ([0, 3, 0, 5000000])
plot_data_label (X, y, 'Training data', 'o', 'bathroomNo', 'price')
plot_fitting_line (w, 'r')
plt.show ()

def get_regression_predictions(input_feature, w):
    '''
    input_feature: name of feature (coresponds to a row of data)
    w: weights
    => return: predicted output
    '''
    X = np.array ([test_data[input_feature]]).T # Input vector (column has only one parameter
    #X = X[:100, :]
    
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)
    
    predicted_output = np.dot (Xbar, w)
    return (X, predicted_output)
    

(X, predicted_output) = get_regression_predictions ('sqft_living', w_test)
groundtruth = np.array ([test_data['price']]).T # Ground truth 
print ('Test price: ', groundtruth)
#groundtruth = groundtruth[:100, :]
plot_data_label (X, predicted_output, 'Predicted price vs ground truth', 'vr', 'sqft_living', 'price')
plot_data_label (X, groundtruth, 'Predicted price vs ground truth', 'o', 'sqft_living', 'price')
plt.show ()


def get_residual_sum_of_squares(input_feature, output, w):
    '''
    Calculate RSS when know input and label of test data, awa weights
    '''
    X = np.array ([test_data[input_feature]]).T # Input vector (column) has only one parameter
    y = np.array ([test_data[output]]).T # Ground truth 
    
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)
    
    # 1st way
    '''
    n = Xbar.shape[0]
    RSS = 0
    for i in range (n):
        RSS += (y[i] - np.dot (Xbar[i], w)) ** 2
    print ('RSS1: ', RSS)'''    
    # 2nd way
    RSS = sum ((y - np.dot (Xbar, w)) ** 2) #/ X.shape[0]
    return RSS
RSS = get_residual_sum_of_squares('sqft_living', 'price', w_test)
print ('Build model using sqft_living, RSS: ', RSS)
RSS = get_residual_sum_of_squares('bathroomNo', 'price', w_test)
print ('Build model using bathroom Number, RSS: ', RSS)
    
def inverse_regression_predictions(output_value, w):
    estimated_input = (output_value - w[0][0]) / w[1][0]
    return estimated_input

y = np.array ([test_data['price']]).T
estimated_input = inverse_regression_predictions(y, w_test)
groundtruth = np.array ([test_data['sqft_living']]).T # Ground truth 
index = np.arange (groundtruth.shape[0]).reshape (groundtruth.shape[0], 1)
plot_data_label (index, estimated_input, 'Estimated input vs ground truth', 'vr', 'Index', 'sqft_living')
plot_data_label (index, groundtruth, 'Estimated input vs ground truth', 'o', 'Index', 'sqft_living')
plt.show ()
