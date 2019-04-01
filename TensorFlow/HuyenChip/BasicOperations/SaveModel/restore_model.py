# -*- coding: utf-8 -*-
"""
Created on Tue Oct 25 10:53:42 2017

@author: Quang
"""
 
import tensorflow as tf

with tf.Session() as sess:    
    saver = tf.train.import_meta_graph('./SavedModels/my_test_model.meta')
    saver.restore(sess,tf.train.latest_checkpoint('./SavedModel/'))
    print(sess.run('w1:0'))