
CC = gcc

LIBS = -lresolv -lsocket -lnsl -lpthread -lm -lc \
	/home/courses/cse533/Stevens/unpv13e_solaris2.10/libunp.a

FLAGS =  -g -O2
CFLAGS = ${FLAGS} -I/home/courses/cse533/Stevens/unpv13e_solaris2.10/lib

all: server client

server: server_handler.o server.o get_ifi_info_plus.o util.o prifinfo_plus.o receiver_sender.o rtt.o 
	${CC} ${FLAGS} -o server server_handler.o server.o get_ifi_info_plus.o util.o prifinfo_plus.o receiver_sender.o rtt.o ${LIBS}
server.o: server.c
	${CC} ${CFLAGS} -c server.c
	
client: client.o get_ifi_info_plus.o util.o receiver_sender.o rtt.o client_handler.o 
	${CC} ${FLAGS} -o client client.o get_ifi_info_plus.o util.o receiver_sender.o rtt.o client_handler.o ${LIBS}

get_ifi_info_plus.o: get_ifi_info_plus.c
	${CC} ${CFLAGS} -c get_ifi_info_plus.c

prifinfo_plus.o: prifinfo_plus.c
	${CC} ${CFLAGS} -c prifinfo_plus.c

util.o: util.c
	${CC} ${CFLAGS} -c util.c

receiver_sender.o : receiver_sender.c
		${CC} ${CFLAGS} -c receiver_sender.c

rtt.o : rtt.c
	${CC} ${CFLAGS} -c rtt.c

client_handler.o :client_handler.c
	${CC} ${CFLAGS} -c client_handler.c

server_handler.o :server_handler.c
	${CC} ${CFLAGS} -c server_handler.c
clean:
	rm -f server server.o client client.o util.o prifinfo_plus.o get_ifi_info_plus.o receiver_sender.o server_handler.o client_handler.o rtt.o util.o core



