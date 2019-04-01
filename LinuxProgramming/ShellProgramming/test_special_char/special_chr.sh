#!/bin/bash

# comment character (#)
echo "Acomment will follow."#comment here

echo "Acomment will follow." #comment here // need a space

echo $(( 2#101011 )) # base conversion, not a comment

# command separator (semicolon ;)
echo hello;echo world

filename="a bc.txt"
if [ -x "$filename" ]; then
    echo "file $filename exists."; cp $filename $filename.bak # should use a space after ;
else
    echo "file $filename not found.";touch "$filename"
fi; echo "File test complete."

# patial quoting (double quote "): "String" preserves most of the special characterswithin string

# full quoting (single quote): 'string' preserves all special characters, stronger than "string", variable substitution ($var) can't be executed.

# comma operator (,): links together a series of arithmetic operations. All are evaluated, but only the last one is returned.
let "t= ((a = 9, 15 / 3))"
echo "a = $a, t = $t"
# another example: for file in /{,usr}bin/ ... blabla -> search file in /bin directory and /usr/bin directory

# variable substitution (contents of a variable: $)

# parameter substitution: (${}): may be used for variable concatenation:
ourpath=${PWD}/test_path/
filename=quang
ourfile=${filename}_file.txt
echo "path1: $ourpath"
echo "file1: $ourfile"
ourpath2=$PWD/test_path2/
ourfile2=$filename_file2.txt
echo "path2: $ourpath2"
echo "file2: $ourfile2"

# quoted string expansion: ($'..')
quote=$'\042'
pound=$'\043'
echo -e "quote: $quote\npound: $pound\n"

# Positional parameters (used when pass arguments from command line)
echo "positional variables: $*" # all of the positional parameters
echo "$@" # the same as $*, 

# exit status variable
echo "Exit status variable: $?"

# process ID variable
echo "Process id: $$"

# Group of commands
(a=hello; echo "Sub shell: a = $a, b = $b")
echo "Main cell: a = $a"

# Array initialization
arr=(e1 e2 e3)
echo "arr(0) = ${arr[1]}"

# Brace expansion ({})
echo "{My, name, is, Arnold}"
echo ""{My, name, is, Arnold}""
echo \"{My, name, is, Arnold}\"
echo "1st file" > file1; echo "2nd file" > file2

# extended brace expansion
echo {a..z}
echo {c..a}
echo {2..7}
echo {A..T}
echo {A..a}
echo {a..A}

# Block of code:
a=12;
b=34;
{
    a=1;
    echo "local a = $a";
    local b;
    b=3;
    echo "local b = $b"
}
echo "global: a = $a, b = $b"

# example using block of code:
File=file1

{
    read line1
    read line2
} < $File

echo "First line in $File: $line1"
echo "Second line in $File: $line2"

# test condition ([[..]])
file=/etc/passwd
cur_file=$PWD/file2
if [ -e $file ]; then
    echo "Password file exists"
fi
if [ -e $file || -e cur_file ]; then  # can not use || operator in this condition test.
    echo "Password file or $file2 exists"
fi
if [ $a -eq $b ]; then  
    echo "compare a vs b using -eq operator"
else
    echo "a != b"
fi

if [[ -f $file ]]; then # using [[..]], rather than [..] can prevent many logic errors in scripts, Ex. $$, ||, <, > operators
    echo "Password file exists"
fi
if [[ -f $file || -f $cur_file ]]; then # using [[..]], rather than [..] can prevent many logic errors in scripts, Ex. $$, ||, <, > operators
    echo "Password file or $file2 exists"
fi

# Note: following an if, neither the test command nor the test brackets ([], [[]]) are strictly neccessary, the if COMMAND struct returns the exit status of COMMAND
# Similarly, a condition whithin test brackets may stand alone without an if.
[$a -eq $b]
    echo "a==b"

# range of characters:
#read var

echo "using case"
case $var in
    [a-z][a-z]) echo "equal";;
    *) echo "not equal";;
esac


echo "using if-else"
#if [ $var == [a-z][a-z] ]; then # can not use this way
#if [[ $var == [a-z][a-z] ]]; then # it is ok
if [[ $var = [a-z][a-z] ]]; then # it is ok
    echo "equal"
else
    echo "not equal"
fi

# integer expansion
a=3
b=7
echo $[$a + $b]
echo $(($a + $b))
echo `expr $a + $b`
echo $(expr $a + $b)

# word boundary
filename="combined_file"
if  grep 'a' $filename ; then
    echo 1st;
fi
if grep '\<a\>' $filename ; then
    echo 2nd;
fi

# pipe: a method used to communicate between processes, send the output (stdout) of one process (command) to another.
# output of a command can pass to a script
echo ls -l | sh
s1="abc"
s2="def"
#s3=$(($s1+$s2))
let "s3=$s1+$s2"
echo "$s1 $s2 $s3"
