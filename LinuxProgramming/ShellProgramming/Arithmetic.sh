#!/bin/bash

x=0
while [ "$x" -ne 10 ]; do
	echo $x
	x=`expr $x + 1` #x=`expr $x+1` ----> false
	#x=$(($x+1))
done

# Parameter proceessing
echo
echo Parameter proceessing:
unset foo
echo ${foo:-bar}

foo=fud
echo ${foo:-bar}

foo=/usr/bin|X11/startx
echo ${foo#*|}
echo ${foo##*|}

bar=/usr/local/etc/local/networks
echo ${bar%local*}
echo ${bar%%local*}
exit 0