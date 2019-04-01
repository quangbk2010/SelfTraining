#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Sep  3 23:04:39 2017

@author: quangbk2010
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
#sales.index += 1
#sales = sales.sort_values (['sqft_living','price'])

#----------------------------------------------------------------------------------------------------------
# Main function
def poly_regression (feature, degree): # feature is pandas.Series type
    
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
    
'''
# Using polydata instead of numpy.array

poly1_data = poly_regression ('sqft_living', 1)
price = np.array ([train_data['price']]).T
regr = linear_model.LinearRegression (fit_intercept = False)
regr.fit (poly1_data, price)

print ('Coefficients: ', regr.coef_.T)
plot_data_label (poly1_data [:, 1], price, 'Polynomial degree 1', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
plot_fitting_line (regr.coef_.T, 'r')
plt.show ()

poly2_data = poly_regression ('sqft_living', 2)
price = np.array ([train_data['price']]).T
regr = linear_model.LinearRegression (fit_intercept = False)
regr.fit (poly2_data, price)'''


# Using set1 data
X = np.array ([set1_data['sqft_living']]).T
y = np.array ([set1_data['price']]).T
np.sort (y)
degree = 5
poly = PolynomialFeatures (degree)
Xbar = poly.fit_transform (X)

regr = linear_model.LinearRegression () #fit_intercept = False)
regr.fit (Xbar, y)

print ('Coefficients: ', regr.coef_.T)

plot_data_label (X, y, 'Polynomial degree ' + str (degree) + ', using set1 data', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
plot_fitting_line (regr.coef_.T, 'rv', degree)
axis_limit (0, 14000, 0, 1e7)
plt.show ()


# Using set2 data
X = np.array ([set2_data['sqft_living']]).T
y = np.array ([set2_data['price']]).T
np.sort (y)
#degree = 15
poly = PolynomialFeatures (degree)
Xbar = poly.fit_transform (X)

regr = linear_model.LinearRegression () #fit_intercept = False)
regr.fit (Xbar, y)

print ('Coefficients: ', regr.coef_.T)

plot_data_label (X, y, 'Polynomial degree ' + str (degree) + ', using set1 data', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
plot_fitting_line (regr.coef_.T, 'rv', degree)
axis_limit (0, 14000, 0, 1e7)
plt.show ()


# Using set3 data
X = np.array ([set3_data['sqft_living']]).T
y = np.array ([set3_data['price']]).T
np.sort (y)
#degree = 15
poly = PolynomialFeatures (degree)
Xbar = poly.fit_transform (X)

regr = linear_model.LinearRegression () #fit_intercept = False)
regr.fit (Xbar, y)

print ('Coefficients: ', regr.coef_.T)

plot_data_label (X, y, 'Polynomial degree ' + str (degree) + ', using set1 data', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
plot_fitting_line (regr.coef_.T, 'rv', degree)
axis_limit (0, 14000, 0, 1e7)
plt.show ()


# Using set4 data
X = np.array ([set4_data['sqft_living']]).T
y = np.array ([set4_data['price']]).T
np.sort (y)
#degree = 15
poly = PolynomialFeatures (degree)
Xbar = poly.fit_transform (X)

regr = linear_model.LinearRegression () #fit_intercept = False)
regr.fit (Xbar, y)

print ('Coefficients: ', regr.coef_.T)

plot_data_label (X, y, 'Polynomial degree ' + str (degree) + ', using set1 data', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
plot_fitting_line (regr.coef_.T, 'rv', degree)
axis_limit (0, 14000, 0, 1e7)
plt.show ()


#----------------------------------------------------------------------------------------------------------
# Using training data, and validation data to choose the degree for the model
X = np.array ([train_data['sqft_living']]).T
y = np.array ([train_data['price']]).T
X_valid_data = np.array ([valid_data['sqft_living']]).T
y_valid_data = np.array ([valid_data['price']]).T
np.sort (y)
RSS_array = []
max_degree = 15
for degree in range (1, max_degree + 1):
    poly = PolynomialFeatures (degree)
    Xbar = poly.fit_transform (X)
    valid_databar = poly.fit_transform (X_valid_data)
    
    regr = linear_model.LinearRegression () #fit_intercept = False)
    regr.fit (Xbar, y)
    
    plot_data_label (X, y, 'Polynomial degree ' + str (degree) + ', using validation data', 'go', 'sqft_living', 'price') #(X, y, title,  color_shape, x_label, y_label)
    plot_fitting_line (regr.coef_.T, 'rv', degree)
    axis_limit (0, 14000, 0, 1e7)
    plt.show ()
    
    predicted_output = get_regression_predictions(regr, valid_databar)
    RSS = get_residual_sum_of_squares(predicted_output, y_valid_data)
    print ('RSS' + str (degree) + ': ', RSS)
    RSS_array.append (RSS)
print ('RSS_array: ', RSS_array)
print ('The model coressponding the degree ' + str (np.argmin (RSS_array) + 1) + ' has the smallest RSS')
    
index = range (1, max_degree + 1)
plt.plot (index, RSS_array, 'y')
plt.title ('RSS over degree from 1 to 15')

plt.xlabel ('Degree')
plt.ylabel ('RSS')
plt.show ()

#----------------------------------------------------------------------------------------------------------
# Using test data to calculate RSS for the model with the degree selected from Validation data
best_degree = np.argmin (RSS_array) + 1
X_test_data = np.array ([test_data['sqft_living']]).T
y_test_data = np.array ([test_data['price']]).T
poly = PolynomialFeatures (best_degree)
X_test_databar = poly.fit_transform (X_test_data)

regr = linear_model.LinearRegression () #fit_intercept = False)
regr.fit (X_test_databar, y_test_data)

predicted_output = get_regression_predictions(regr, X_test_databar)
RSS = get_residual_sum_of_squares(predicted_output, y_test_data)
print ('The model coressponding the degree ' + str (best_degree) + ', in the test data has the smallest RSS: ' + str (RSS))
