#!/bin/bash

echo "Are you a man? Please answer yes or no, if not, you are either woman, gay, lesian, bisecxual or transgender"
read sex

case $sex in
	#man | Man | MAN ) echo "hi boy.";;
	[Mm] | [Mm][Aa][Nn]) echo "hi boy.";; # inpput: m, M, Man, MAn, MaN, ...
	#w* | W*) echo "you are so pretty.";;
	[wW]*) echo "you are so pretty.";;
	gay) echo "hi gay.";;
	les) echo "hi.";;
	bisecxual) echo "you are bisecxual";; # can't miss ';;'
	transgender) echo "oh, I know";;
	*) echo "sorry, answer not recognized."
	   echo "Please answer again"
	   exit 1
	   #;;                                #  it is ok to miss ';;'
esac

exit 0