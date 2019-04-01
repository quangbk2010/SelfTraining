# -*- coding: utf-8 -*-
"""
Created on Tue Sep  5 14:20:31 2017

@author: quang
"""

import numpy as np
#from pandas import DataFrame, read_csv
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import linear_model
from sklearn.preprocessing import PolynomialFeatures

# Plot fucntions
def axis_limit (axis_x_min, axis_x_max, axis_y_min, axis_y_max):
    plt.axis ([axis_x_min, axis_x_max, axis_y_min, axis_y_max])
def plot_data_label (X, y, title,  color_shape, x_label, y_label):
    plt.plot (X, y, color_shape)
    plt.title (title)
    plt.xlabel (x_label)
    plt.ylabel (y_label)
    #plt.show ()
    
def plot_fitting_line (w, color, degree):
    w_array = []
    for i in range (degree + 1):
        w_array.append (w[i][0])
    x0 = np.linspace (0, 14000, 140000)
    y0 = w_array[0]
    for i in range (1, degree + 1):
        y0 += w_array[i] * (x0 ** i)
    plt.plot (x0, y0, color)

#----------------------------------------------------------------------------------------------------------
def get_regression_predictions(regr, Xbar):
    '''
    input_feature: an array contains name of features (coresponds to a matrix, with each row is a data point)
    w: weights
    => return: predicted output
    '''    
    predicted_output = regr.predict (Xbar) 
    return predicted_output
    
def get_residual_sum_of_squares(predicted_output, y):
    """
    Calculate RSS when know input and label of test data, awa weights
    """
    RSS = sum ((y - predicted_output) ** 2)
    return RSS[0]

#----------------------------------------------------------------------------------------------------------
# Read training data from csv train file
headers = ["idNo","date","price","bedroomNo","bathroomNo","sqft_living","sqft_lot","floorNo","waterfront","view","condition","grade","sqft_above","sqft_basement","yr_built","yr_renovated","zipcode","lat","long","sqft_living15","sqft_lot15"]
dtype_dict = {'bathroomNo':float, 'waterfront':int, 'sqft_above':int, 'sqft_living15':float, 'grade':int, 'yr_renovated':int, 'price':float, 'bedroomNo':float, 'zipcode':str, 'long':float, 'sqft_lot15':float, 'sqft_living':float, 'floorNo':str, 'condition':int, 'lat':float, 'date':str, 'sqft_basement':int, 'yr_built':int, 'idNo':str, 'sqft_lot':int, 'view':int}
train_data = pd.read_csv ('../Data/3rdWeek/wk3_kc_house_train_data.csv', names = headers, dtype = dtype_dict, header = 0)

# Read the test data from csv test file
test_data = pd.read_csv ('../Data/3rdWeek/wk3_kc_house_test_data.csv', names = headers, dtype = dtype_dict, header = 0)
set1_data = pd.read_csv ('../Data/3rdWeek/wk3_kc_house_set_1_data.csv', names = headers, dtype = dtype_dict, header = 0)
set2_data = pd.read_csv ('../Data/3rdWeek/wk3_kc_house_set_2_data.csv', names = headers, dtype = dtype_dict, header = 0)
set3_data = pd.read_csv ('../Data/3rdWeek/wk3_kc_house_set_3_data.csv', names = headers, dtype = dtype_dict, header = 0)
set4_data = pd.read_csv ('../Data/3rdWeek/wk3_kc_house_set_4_data.csv', names = headers, dtype = dtype_dict, header = 0)
valid_data = pd.read_csv ('../Data/3rdWeek/wk3_kc_house_valid_data.csv', names = headers, dtype = dtype_dict, header = 0)
sales = pd.read_csv('../Data/3rdWeek/kc_house_data.csv', names = headers, dtype = dtype_dict, header = 0)
train_valid_shuffled = pd.read_csv('../Data/4thWeek/wk3_kc_house_train_valid_shuffled.csv', names = headers, dtype = dtype_dict, header = 0)
#sales.index += 1
#sales = sales.sort_values (['sqft_living','price'])

#----------------------------------------------------------------------------------------------------------
# Data preparation function
def prepare_poly_data (feature, degree): # feature is pandas.Series type
    
    X = np.array ([train_data[feature]]).T
   
    # Building Xbar
    one = np.ones ((X.shape[0], 1))
    Xbar = np.concatenate ((one, X), axis = 1)
    # first check if degree > 1
    if degree > 1:
        # then loop over the remaining degrees:
        for power in range (2, degree+1):
            Xbar = np.concatenate ((Xbar, X ** power), axis = 1)
    return Xbar

#----------------------------------------------------------------------------------------------------------
# main function
# Compare the fitting line of all model trained with degree = 1-15 and using ridge regression
def poly_ridge_regression ():
    l2_small_penalty = 1.5e-5
    max_degree = 15
    X_train_data = np.array ([train_data['sqft_living']]).T
    price = np.array ([train_data['price']]).T
    
    for degree in range (1, max_degree + 1):
        # The 1st way to prepare data for training model -> ok
        """poly15_data = prepare_poly_data ('sqft_living', degree)"""
        
        # Try the 2nd way: -> ok
        poly = PolynomialFeatures (degree)
        X_train_data_bar = poly.fit_transform (X_train_data)
        
        regr = linear_model.Ridge (alpha = l2_small_penalty, normalize = True)
        regr.fit (X_train_data_bar, price)
        
        plot_data_label (X_train_data, price, 'Polynomial degree ' + str (degree) + ', using training data', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
        plot_fitting_line (regr.coef_.T, 'rv', degree)
        axis_limit (0, 14000, 0, 1e7)
        plt.show ()

#----------------------------------------------------------------------------------------------------------
# compare the fitting line of a model with degree = 15, using ridge regression with 4 different data sets
def poly15_ridge_regression ():
    l2_small_penalty = 1.23e-3
    degree = 15
    
    # Using set1 data
    X_set1_data = np.array ([set1_data['sqft_living']]).T
    price = np.array ([set1_data['price']]).T
    np.sort (price)
    poly = PolynomialFeatures (degree)
    X_set1_data_bar = poly.fit_transform (X_set1_data)
    
    regr = linear_model.Ridge (alpha = l2_small_penalty, normalize = True)
    regr.fit (X_set1_data_bar, price)
    
    print ('Coefficients: ', regr.coef_.T)
    
    plot_data_label (X_set1_data, price, 'Polynomial degree ' + str (degree) + ', using set1 data', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
    plot_fitting_line (regr.coef_.T, 'rv', degree)
    axis_limit (0, 10000, 0, 1e7)
    plt.show ()
    
    # Using set2 data
    X_set2_data = np.array ([set2_data['sqft_living']]).T
    price = np.array ([set2_data['price']]).T
    np.sort (price)
    poly = PolynomialFeatures (degree)
    X_set2_data_bar = poly.fit_transform (X_set2_data)
    
    regr = linear_model.Ridge (alpha = l2_small_penalty, normalize = True)
    regr.fit (X_set2_data_bar, price)
    
    print ('Coefficients: ', regr.coef_.T)
    
    plot_data_label (X_set2_data, price, 'Polynomial degree ' + str (degree) + ', using set2 data', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
    plot_fitting_line (regr.coef_.T, 'rv', degree)
    axis_limit (0, 10000, 0, 1e7)
    plt.show ()

#----------------------------------------------------------------------------------------------------------
# Using k-fold geralization and ridge regression to train model
degree = 15
def kfold_ridge_regression (k, l2_penalty):#, data, output):
    n = len (train_valid_shuffled)
    sum_RSS = 0
    for i in range (k):
        start = ( n * i) // k
        end = (n * (i + 1)) // k - 1
        valid_set_data = train_valid_shuffled[start : (end + 1)]
        train_set_data = train_valid_shuffled[0 : start].append (train_valid_shuffled[end + 1 : n])
        
        X_train_set_data = np.array ([train_set_data['sqft_living']]).T
        y_train_set_price = np.array ([train_set_data['price']]).T
        X_valid_set_data = np.array ([valid_set_data['sqft_living']]).T
        y_valid_set_price = np.array ([valid_set_data['price']]).T       
        
        poly = PolynomialFeatures (degree)
        X_train_set_data_bar = poly.fit_transform (X_train_set_data)
        X_valid_set_data_bar = poly.fit_transform (X_valid_set_data)
        
        regr = linear_model.Ridge (alpha = l2_penalty, normalize = True)
        regr.fit (X_train_set_data_bar, y_train_set_price)
            
        predicted_output = get_regression_predictions(regr, X_valid_set_data_bar)
        RSS = get_residual_sum_of_squares(predicted_output, y_valid_set_price)
        sum_RSS += RSS
        print ('RSS for dataset ' + str (i) + ' with degree ' + str (degree) + ' and penalty ' + str (l2_penalty) + ' is: ', RSS)
    return sum_RSS/k

"""average_RSS = kfold_ridge_regression (10, 10e3)
print ('Average RSS: ', average_RSS)"""

#----------------------------------------------------------------------------------------------------------
# Find the model that minimizes thae average validation error
l2_penalty_array = np.logspace (3, 9, num = 13)
len_array = len (l2_penalty_array)
RSS_array = []

for i in range (1, len_array):
    average_RSS = kfold_ridge_regression (10, l2_penalty_array[i])
    RSS_array.append (average_RSS)
        
l2_penalty_best = l2_penalty_array[np.argmin (RSS_array)]
print ('l2_penalty best: ', l2_penalty_best)

#----------------------------------------------------------------------------------------------------------
# Apply l2_penalty into the model, and compute RSS on the test data
X_test_data = np.array ([test_data['sqft_living']]).T
y_test_price = np.array ([test_data['price']]).T

poly = PolynomialFeatures (degree)
X_test_data_bar = poly.fit_transform (X_test_data)

regr = linear_model.Ridge (alpha = l2_penalty_best, normalize = True)
regr.fit (X_test_data_bar, y_test_price)
    
predicted_output = get_regression_predictions(regr, X_test_data_bar)
RSS = get_residual_sum_of_squares(predicted_output, y_test_price)

print ('RSS over the test data set using the best l2_penalty trained from valid data set: ', RSS)