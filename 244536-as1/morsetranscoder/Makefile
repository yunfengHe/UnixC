
CC=gcc
CFLAGS= -Wall -pedantic


PROGS=morsetranscoder

all: $(PROGS)

morsetranscoder: main.c morse.c morse.h
	$(CC) $(CFLAGS) main.c morse.c morse.h $(LDFLAGS) -o morsetranscoder

clean:
	rm -f $(PROGS) *COPY LOGFILE
