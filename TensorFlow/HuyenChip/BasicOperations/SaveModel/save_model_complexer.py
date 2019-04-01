# -*- coding: utf-8 -*-
"""
Created on Tue Oct 25 10:53:42 2017

@author: Quang
"""
 
import tensorflow as tf

#Prepare to feed input, i.e. feed_dict and placeholders
w1 = tf.placeholder("float", name="w1")
w2 = tf.placeholder("float", name="w2")
b1= tf.Variable(2.0,name="bias")
feed_dict ={w1:4,w2:8}

#Define a test operation that we will restore
w3 = tf.add(w1,w2)
w4 = tf.multiply(w3,b1,name="op_to_restore")

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    
    #Create a saver object which will save all the variables
    saver = tf.train.Saver()
    
    #Run the operation by feeding input
    print (sess.run(w4,feed_dict))
    #Prints 24 which is sum of (w1+w2)*b1 
    
    #Now, save the graph
    saver.save(sess, './SavedModels/my_test_model_complexer',global_step=1000)
