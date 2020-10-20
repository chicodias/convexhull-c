#
# Makefile for XDialogs
#

CC = gcc

# Note that some compilers don't like -O (optimization)
# with -g (debug info); with gcc it's OK.
CFLAGS = -g -O -Wall $(DOUBLE_CLICKS) -I/usr/X11R6/include

# Change this if your standard library does not provide gettimeofday():
DOUBLE_CLICKS =
#DOUBLE_CLICKS = -DNO_DCLICK    # Disable double clicks


#####################################################
# You should not have to change anything beyond this

OBJS = window.o dialog.o win_io.o stdctrl.o stddlg.o bgiclone.o

all: libxdialogs.a

clean:
	rm -f *~ *.o *.a

libxdialogs.a: $(OBJS)
	ar rs $@ $(OBJS)

%.o: %.c xdialogs.h internal.h
	$(CC) $(CFLAGS) -o $@ -c $<
