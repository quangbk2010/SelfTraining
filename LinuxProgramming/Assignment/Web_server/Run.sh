#!/bin/bash

N=2
./tcp_Server3 &
for (( i = 0; i < $N; i++ ))
do
    ./tcp_Client &
done