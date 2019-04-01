#gcc -g -o Server Server.c -I.
#gcc -g -o Client Client.c -I.

# the macro CC is the C compiler to use
CC=gcc

# CFLAGS is the list of flags to pass to the compilation command
CFLAGS=-I.

# creates the macro DEPS, which is the set of .h files on which the .c files depend
# any change in DEPS will change in .c file compilation 
DEPS = Client.h

# all of the object files should be listed as part of the macro OBJ.
OBJ = Server.o Client.o 

# a simple rule used to tell 'make' that all '.c' files depend on certain '.h' files
# define a rule that applies to all files ending in the .o suffix. 
# The rule says that the .o file depends upon the .c version of the file and the .h files included in the DEPS macro. 
# The rule then says that to generate the .o file, make needs to compile the .c file using the compiler defined in the CC macro. 
# The -c flag says to generate the object file, the -o $@ says to put the output of the compilation in the file named on the left side of the :, 
# the $< is the first item in the dependencies list, and the CFLAGS macro is defined as above.
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
Server: Server.o
	$(CC) -o Server Server.o $(CFLAGS)
	
Client: Client.o
	$(CC) -o Client Client.o $(CFLAGS)
	
