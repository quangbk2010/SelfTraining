# -*- coding: utf-8 -*-
"""
Created on Tue Oct 24 10:53:42 2017

@author: Quang

A TF program often has 2 phases:
1. Assemble a graph
2. Use a session to execute operations in the graph.
⇒ Can assemble the graph first without knowing the values needed for computation


Element-wise mathematial operations: add, sub, mul, div, exp, log, greater, lass, equal, ...
Array operations: concat, slice, split, constant, rank, shape, shuffle, ...
matric operations: matmul, matrixinverse, matrixDeterminant, ...
stateful operations: varianle, assign, assignAdd, ...
Neural network building blocks: softMax, sigmoid, ReLU, convolution2D, maxPool, ...
Checkpointing operations: save, restore
Queue and synchronyzation operations: enqueue, dequeue, mutexAccuquire, mutexRelease, ...
Control flow operations: Merge, Switch, Enterm Leave, NextIteration

- Note:
    + lazy-loading: is a non-bug bug, occurs when you defer declaring/initializing an object until it is loaded. 
                    In the context of Tensorflow, it means you defer creating an op until you need to compute it.
                    E.g; 
                    x = a variable
                    y = a variable
                    with tf.Session() as sess:
                        for _ in range (10):
                            sess.run (tf.add(a, b)) # -> only create the op add when need to compute it
                    => consequence: graph (when visualizing on TensorBoard) -> don't see "Add" node, but the protobuf for the graph is loadded 10 times
                    -> add a new node whenever want to compute z
                    => graph definition becomes bloated, slow to load and expensive to pass around
        -> Avoid:
            1. Separate the assembling of graph and executing ops
            2. Use Python attribute to ensure a function is only loaded the first time it’s called

"""

import tensorflow as tf
import numpy as np

""" Constants"""
"""
Constant values are stored in the graph definition.
Sessions allocate memory to store variable values.

=> Only use constants for primitive types. Use variables or readers for more data that requires more memory
To know why, run this:
my_const = tf.constant ([1.0, 2.0], name = "my_const")
    with tf.Session() as sess:
        print (sess.graph_def)
    """
str1 = tf.constant ("Hello world!")

a = tf.constant (value = 2, name = "a", dtype = None, shape = None, verify_shape = False)
b = tf.constant (3, name = "b")
c = tf.add (a, b, name = "add")
d = tf.constant (value = [1,2,3], shape = [4, 1]) # Reshape, if verify_shape = True -> wrong, but if verify_shape = False -> [1,2,3,3]

a2 = tf.constant ([2, 2], name = "a2") # create a constant row
b22 = tf.constant ([[0, 1], [2, 3]], name = "b22") # create a matrix
c22 = tf.add (a2, b22, name = "add22")
d22 = tf.multiply (a2, b22, name = "mul22")
e = tf.matmul (tf.reshape(a2, [1,2]), tf.reshape (b22, [2, 2]))

def test1(sess):
    print (sess.run (c))
    print (sess.run (d))
    x, y= sess.run ([c22, d22])
    print (x, "\n\n", y)
    print ("\nmatmul:", sess.run (e))

# Create all-zeros, all-ones, all-"number"
zeros23 = tf.zeros (shape = [2, 3], dtype = tf.int32, name = None)
zeros22_child = tf.zeros_like (b22)
ones24 = tf.ones (shape = [2, 4], dtype = tf.float16)
ones24_child = tf.ones_like (ones24, dtype = tf.float32, name = "ones24_child", optimize = True)
eights25 = tf.fill (dims = [2, 5], value = 8, name = None)


def test2(sess):
    print ("zeros:")
    print (sess.run ([zeros23]), "\n\n", sess.run (zeros22_child))
    print ("ones:")
    print (sess.run ([ones24]), "\n\n", sess.run (ones24_child))
    print ("\neights:")
    print (sess.run (eights25))

# Create sequences
sequences = tf.linspace (start = 1.0, stop = 10.0, num = 4, name = None)
aRange = tf.range (start = 1, limit = 10, delta = 1, dtype = None, name = "range")

def test3(sess):
    print ("Sequence:")
    print (sess.run (sequences))
    print ("Range:")
    print (sess.run (aRange))

# Create random number
rand_normal = tf.random_normal (shape = [2, 3], mean = 0.0, stddev = 1.0, dtype = tf.float16, seed = None, name = None)
truncated_normal = tf.truncated_normal (shape = [2, 3], mean = 0.0, stddev = 1.0, dtype = tf.float16, seed = None, name = None)
rand_uniform = tf.random_uniform (shape = [2, 3], minval = 0, maxval = 3, dtype = tf.float16, seed = None, name = None)
rand_shuffle1 = tf.random_shuffle (value = [1, 2, 3, 4], seed = None, name = None) # shuffle in an array
rand_shuffle2 = tf.random_shuffle (value = [[1, 2, 3, 4], [0.1, 0.2, 0.3, 0.4]], seed = None, name = None) # shuffle 2 row, but not elements in an array
    
def test4(sess):
    print ("Random normal:")
    print (sess.run (rand_normal))
    print ("Truncated normal:")
    print (sess.run (truncated_normal))
    print ("Random uniform:")
    print (sess.run (rand_uniform))
    
    print ("Random shuffle:")
    print (sess.run (rand_shuffle1))
    print (sess.run (rand_shuffle2))
    
""" Variables"""
a = tf.Variable (2, name = "scalar")
a_assign_op = a.assign (4)
a_copy = tf.Variable (2 * a.initialized_value()) # -> safer to ensure that a is initialized before

b = tf.Variable (initial_value = [2, 3], name = "vector")
c = tf.Variable (initial_value = [[0, 1], [2, 3]], name = "matrix")
w = tf.Variable (initial_value = tf.zeros (shape = [784, 10]), name = "tensor_784_10")
random_w = tf.Variable (initial_value = tf.truncated_normal ([2, 5]), name = "random_truncated_var")

def test5(sess):
    #init = tf.global_variables_initializer () # this line equals to 2 below lines
    init = tf.variables_initializer ([a, b, c])#
    sess.run (w.initializer)                   #
    sess.run (init)
    #print ("a:", sess.run (a))
    print ("a::", a.eval())
    #print ("a:::", sess.run (a_assign_op))
    print ("a::::", a_assign_op.eval())
    print ("b:", sess.run (b))
    print ("c:", sess.run (c))
    print ("w:", sess.run (w))
    
    sess.run (random_w.initializer)
    print ("random_w:", random_w.eval())
    
def test6(sess):
    print (a_assign_op.eval())
def test6_2(sess):
    sess.run (a_assign_op)
    print (a_assign_op.eval())
def test6_3(sess):
    sess.run (a.initializer)
    sess.run (a.assign_add (1))
    print (a) #-> <tf.Variable 'scalar_39:0' shape=() dtype=int32_ref>
    print (a.eval()) # -> 3
    
    sess.run (a_copy.initializer) #-> still have to initialize a_copy to 2 initialized value of a.
    print (a_copy.eval())
    
""" Placeholders. Eg. f(x, y) = 2 *x + y => x, y are place holders for the actual values"""
# Create a placeholder of type float 32bit, shape is a vector of 3 elements
a = tf.placeholder (shape = [3], dtype = tf.float32)
b = tf.constant (value = [4, 4, 4], dtype = tf.float32)
c = a + b

def test7(sess):    
    # Feed the placeholder using a dictionary
    print (sess.run (a, {a:[1, 2, 3]}))
    print (sess.run (c, {a:[1, 2, 3]}))
    np.savetxt ("output_basicOperation.txt", sess.run (a, {a:[1, 2, 3]}))
    
 
def test8(sess):
    # create operations, tensors, etc (using the default graph)
    a = tf.add(2, 5)
    b = tf.mul(a, 3)
    # define a dictionary that says to replace the value of 'a' with 15
    replace_dict = {a: 15}
    # Run the session, passing in 'replace_dict' as the value to 'feed_dict'
    sess.run(b, feed_dict=replace_dict) # returns 45


with tf.Session () as sess:
    # to use TensorBoard
    writer = tf.summary.FileWriter ("./graphs", sess.graph)
    test7(sess)
    
    
    