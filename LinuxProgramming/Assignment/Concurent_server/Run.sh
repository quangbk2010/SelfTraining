#!/bin/bash

N=1
./Server &
for (( i = 0; i < $N; i++ ))
do
    ./Client &
done