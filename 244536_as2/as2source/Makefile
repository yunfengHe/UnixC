
CC=gcc
CFLAGS= -Wall -pedantic


PROGS=DTserver DTclient DTclient_helper

all: $(PROGS)

DTserver: DTserver.c  DTlib.h
	$(CC) $(CFLAGS) -pthread DTserver.c DTlib.h $(LDFLAGS) -o DTserver

DTclient: DTclient.c DTlib.h 
	$(CC) $(CFLAGS) -pthread DTclient.c DTlib.h $(LDFLAGS) -o DTclient

DTclient_helper: DTclient_helper.c DTlib.h
	$(CC) $(CFLAGS) -pthread DTclient_helper.c DTlib.h $(LDFLAGS) -o DTclient_helper

clean:
	rm -f $(PROGS) *COPY LOGFILE
