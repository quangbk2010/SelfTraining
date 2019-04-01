#!/bin/bash

rm -rf fred*
echo > fred1
echo > fred2
mkdir fred3
echo > fred4

echo
# check break command
echo check break command: 
for file in fred*
do
	if [ -d "$file" ]; then
		break;
	else
		echo $file ": is not a directory"
	fi
done
echo first directory starting fred was $file

echo 
# check continue command
echo check continue command: 
for file in fred*
do
	if [ -d "$file" ]; then
		echo -e "skipping directory $file \t aaaa \n : \c bbb \n ccc "
		continue
	fi
	echo file is $file
done

rm -rf fred*

# do nothing
if [ -f fred1 ]; then
	:
else
	echo file fred1 did not esist
fi
exit 0
