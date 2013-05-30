
CC=gcc
CFLAGS= -Wall -pedantic


PROGS=DTserver

all: $(PROGS)

DTserver: DTserver.c 
	$(CC) $(CFLAGS) DTserver.c $(LDFLAGS) -o DTserver

clean:
	rm -f $(PROGS) *COPY LOGFILE
