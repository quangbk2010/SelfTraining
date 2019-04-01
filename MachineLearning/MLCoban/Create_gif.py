# -*- coding: utf-8 -*-
"""
Created on Thu Jul 27 13:58:11 2017

@author: quang
"""

import sys
sys.path.insert (0, 'GradientDescent')
from GradientDescent import *
import matplotlib.animation as animation
from matplotlib.animation import FuncAnimation

def save_gif2(eta, x0):
    (x, it) = myGD1(eta, x0)
    y = [cost (xi) for xi in x]
    fig, ax = plt.subplots(figsize=(4,4))    
    plt.cla()
    plt.axis([1.5, 7, 0.5, 4.5])
    
    def update(ii):
        if ii == 0:
            plt.cla()
            delta = 0.025
            xg = np.arange(-6.0, 6.0, delta)
            yg = cost (xg)
            
            animlist = plt.plot(xg, yg)
        else:
            animlist = plt.plot(xg, yg, 'r-')
        
        animlist = plt.plot(x[ii], y[ii], 'ro', markersize = 4) 
        title = 'f(x)=$x^2$+5sin(x); x0 = ' + str(x0) 
        title += '; $\eta =$ ' + str(eta)
        ax.set_title (title)
        
        xlabel = 'iter ' + str(ii) + '/' + str(it) + ': cost = ' + str(cost (x[ii])) + 'grad = ' + str(grad(x[ii]))
        ax.set_xlabel(xlabel)
        
        return animlist, ax
       
    anim1 = FuncAnimation(fig, update, frames=np.arange(0, it), interval=200)
#     fn = 'img2_' + str(eta) + '.gif'
    fn = 'GD1.gif'
    anim1.save(fn, dpi=100, writer='imagemagick')

eta = .1 
x0 = -5
save_gif2(eta, x0)