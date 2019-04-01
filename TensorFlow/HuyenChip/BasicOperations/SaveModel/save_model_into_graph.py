# -*- coding: utf-8 -*-
"""
Created on Tue Oct 25 10:53:42 2017

@author: Quang
"""
 
import tensorflow as tf

w1 = tf.Variable(tf.random_normal(shape=[2]), name='w1')
w2 = tf.Variable(tf.random_normal(shape=[5]), name='w2')
w3 = tf.constant (2, name = "w3")
saver = tf.train.Saver([w1,w2])
sess = tf.Session()
sess.run(tf.global_variables_initializer())
saver.save(sess, './SavedModels/my_test_model')#,global_step=1000)

