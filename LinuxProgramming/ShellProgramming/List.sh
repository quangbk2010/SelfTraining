#!/bin/bash

if [ -f alist.sh ]; then
	foo="true"
elif [ -f auserInput.sh ]; then
	foo="true"
else
	foo="false"
fi

if [ $foo = "true" ]; then
	echo "One of the files exist."
fi

exit             