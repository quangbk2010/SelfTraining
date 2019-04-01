# -*- coding: utf-8 -*-
"""
Created on Tue Oct 24 17:28:39 2017

@author: Quang

steps to build a model:
    
    Phase 1. Assemble our graph:
        Step 1. Read data
        Step 2. Create placeholders for inputs and labels # tf.placeholder (dtype, shape=None, name=None)
        Step 3. Create weight and bias # tf.Variable (initial_value=None, trainable=True, collections=None, name=None, dtype=None, ...)
        Step 4. Build model to predict Y
        Step 5. Specify loss function # Eg. loss = tf.square(Y - Y_predicted, name="loss")
        Step 6. Create optimizer # tf.train.GradientDescentOptimizer(learning_rate=0.001).minimize(loss) --or-- AdamOptimizer, AdadeltaOptimizer, MomemtumOptimizer, ...
        
    Phase 2. Train our model:
        Step 1. Initialize variables
        Step 2. Run optimizer op with data fed into placeholders for inputs and labels #_, l = sess.run([optimizer, loss], feed_dict={X: x, Y:y})
        
    Phase 3. To see our model in TensorBoard:
        Step 1. writer = tf.sumary.FileWriter ("./mygraphs_directory", sess.graph)
        Step 2. $tensorboard --logdir="./mygraphs_directory" --port 6006
        Step 3. Open browser, -> http://localhost:6006 or http://Quang:6006 or use local_ip:6006
    
    Phase 4. Plot the results with matplotlib
        
        
"""

