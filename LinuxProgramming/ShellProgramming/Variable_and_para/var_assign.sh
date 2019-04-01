#!/bin/bash

# Assignment
a=879
echo "The value of \"a\" is $a."
# Assignment using 'let'
let a=16+5
let "a += 4"
echo "The value of \"a\" is now $a."

# Command substitution
a=`echo Hello!`
b=$(ls -l)

exit 0