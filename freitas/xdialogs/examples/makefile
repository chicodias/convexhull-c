#
# Makefile for the examples
#

all: wintest imgtest ctltest parallel

CC = gcc
CFLAGS = -g -Wall -I..
LDFLAGS =
LIBS = -L/usr/X11/lib -L.. -lxdialogs -lX11

ctltest: ctltest.o ../libxdialogs.a
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

parallel: parallel.o ../libxdialogs.a
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

wintest: wintest.o ../libxdialogs.a
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

imgtest: imgtest.o ../libxdialogs.a
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
