#!/bin/bash

touch file_one
rm -f file_two

echo "AND lists:"
if [ -f file_one ] && echo "file_one is " && [ -f file_two ] && echo "file_two"; then
	echo "in if"
else
	echo "in else"
fi

echo "OR lists:"
if [ -f file_two ] || echo "hello" || echo " there"; then
	echo "in if"
else
	echo "in else"
fi

exit 