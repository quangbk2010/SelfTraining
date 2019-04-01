#CC= gcc
CC := gcc
#CFLAGS =-I.
CFLAGS :=-I.
#OBJS =lib1.o lib2.o main.o 
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
#DEPS =lib1.h lib2.h
DEPS := $(wildcard *.h)
A = a
B := b
A += c
B = d 
A := e
B ?= f
C ?= g

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	$(CC) -o main $(OBJS) $(CFLAGS)
	echo "A = " $(A) 
	echo "B = " $(B) 
	echo "C = " $(C)

clean:
	rm *.o main
	rm -r bin/*

install:
	mkdir -p bin
	cp -rf *.o bin
