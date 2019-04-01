#!/bin/bash

# Variables: assignment and substitution
a=375
hello=$a
#-------------------------------------------------------------------------
# No space permitted on either side of = sign when initializing variables.
# What happens if there is a space?
# "VARIABLE =value"
# ^
#% Script tries to run "VARIABLE" command with one argument, "=value".
# "VARIABLE= value"
# ^
#% Script tries to run "value" command with
#+ the environmental variable "VARIABLE" set to "".

hello="A B C D"
echo $hello # A B C D
echo "$hello" # A B C D
# As we see, echo $hello and echo "$hello" give different results.
# =======================================
# Quoting a variable preserves whitespace.
# =======================================
echo '$hello' # $hello
# ^ ^
# Variable referencing disabled (escaped) by single quotes,
#+ which causes the "$" to be interpreted literally

other_numbers="1 2 3"
# If there is whitespace embedded within a variable,
#+ then quotes are necessary.
mixed_bag=2\ ---\ Whatever
# ^ ^ Space after escape (\).


# difference between uninitialized var vs initialized but unassigned var
if [ -z "$unassigned" ]
then
 echo "\$unassigned is NULL."
fi # $unassigned is NULL.
# Using a variable before assigning a value to it may cause problems. It is nevertheless
#+ possible to perform arithmetic operations on an uninitialized variable.
echo "$uninitialized" # (blank line)
let "uninitialized += 5" # Add 5 to it.
echo "$uninitialized"