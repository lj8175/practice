CC = gcc
CFLAGS = -g #-O -Wall -Werror
LDFLAGS =
LIBS = -lssl -lcrypto # -lsocket -lnsl


all: cli serv

cli: cli.o
	$(CC) $(LDFLAGS) -o cli cli.o $(LIBS)

serv: serv.o
	$(CC) $(LDFLAGS) -o serv serv.o $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f cli serv *.o *~ core *.core
