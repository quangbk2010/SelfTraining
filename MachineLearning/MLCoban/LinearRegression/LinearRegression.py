# -*- coding: utf-8 -*-
"""
Created on Tue Jul 18 14:10:34 2017

@author: quang
"""
import numpy as np
import matplotlib.pyplot as plt

X = np.array([[147, 150, 153, 158, 163, 165, 168, 170, 173, 175, 178, 180, 183]]).T
# weight (kg)
y = np.array([[ 49, 50, 51,  54, 58, 59, 60, 62, 63, 64, 66, 67, 68]]).T

def linear_regression():
    '''
        Apply linear regression to the problem: predict the weight of a person when know his height, and
        the (height, weight) of a set of other people.
    '''
    plot_input()
    ret = plot_fitting_line() 
    return ret

def plot_input():
    
    # Visualize data 
    plt.plot(X, y, 'ro')
    plt.axis([140, 190, 45, 75])
    plt.xlabel('Height (cm)')
    plt.ylabel('Weight (kg)')
    plt.show()

def plot_fitting_line():
    # Building Xbar
    one = np.ones ( (X.shape[0], 1))
    Xbar = np.concatenate ( (one, X), axis = 1)
    
    # Calculating weights of the fitting line
    A = np.dot (Xbar.T, Xbar)
    b = np.dot (Xbar.T, y)
    w = np.dot (np.linalg.pinv(A), b)
    print ('w = ', w)
    # Preparing the fitting line
    w_0 = w[0][0]
    w_1 = w[1][0]
    x0 = np.linspace (145, 185, 2)
    y0 = w_0 + w_1*x0
    
    # Drawing the fitting line
    plt.plot (X.T, y.T, 'ro')
    plt.plot (x0, y0)
    plt.axis ([140, 190, 45, 75])
    plt.xlabel('Height (cm)')
    plt.ylabel('Weight (kg)')
    plt.show()
    
    # Predict weight based on height
    y1 = w_1 * 155 + w_0
    y2 = w_1 * 160 + w_0
    print ('Predict weight of person with height 155 cm: %.2f (kg), real number: 52 (kg)' %(y1))    
    print ('Predict weight of person with height 160 cm: %.2f (kg), real number: 56 (kg)' %(y2)) 
    return w.T
    
def linear_regression_using_library():
    from sklearn import linear_model
    
    # Building Xbar
    one = np.ones ( (X.shape[0], 1))
    Xbar = np.concatenate ( (one, X), axis = 1)
    
    # Fit the model by Linear Regression
    regr = linear_model.LinearRegression (fit_intercept=False)
    regr.fit (Xbar, y)
    
    return (regr.coef_)

# Compare tw0 results
print ('Theorical solution:   ', linear_regression()) 
print ('Scikit-learn solution:', linear_regression_using_library())

  
