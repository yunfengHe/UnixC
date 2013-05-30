CC=gcc
CFLAGS= -Wall -pedantic

PROGS=morsecoder decoder
morsecodersource=morsecoder.c morse.c morse.h
decodersource=decoder.c decoder.h
all: $(PROGS)

morsecoder: $(morsecodersource)
	$(CC) $(CFLAGS) $(morsecodersource) $(LDFLAGS) -o morsecoder

decoder: $(decodersource)
	$(CC) $(CFLAGS) $(decodersource) $(LDFLAGS) -o decoder

.PHONY : clean
clean:
	rm -f $(PROGS) LOGFILE *COPY
