#
# Makefile for CG
#

CC = gcc

CFLAGS = -g -Wall -O -I$(XDLG_DIR) -DUNIX -I/usr/X11R6/include
LDFLAGS =

X11_DIR = /usr/X11R6/lib
XDLG_DIR = ./xdialogs
INSTALL_DIR = /usr/local/bin


##########################################################
# You should not have to change anything beyond this line

OBJS = cg.o fgrafico.o fmouse.o interfac.o qhull.o graham.o triang.o \
       triang3.o mindist.o sort.o erro.o vetor.o pilha.o fio.o dupla.o \
       guloso.o we.o mwt_poly.o aem.o

LIBS = -L$(XDLG_DIR) -lxdialogs -L$(X11_DIR) -lX11 -lm


all: cg

install: cg
	cp cg $(INSTALL_DIR)

clean:
	rm -f *.o *~ cg minusc
	cd $(XDLG_DIR); make clean

minusc: minusc.c
	$(CC) -s -O2 -o minusc minusc.c

cg: $(OBJS) $(XDLG_DIR)/libxdialogs.a
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(XDLG_DIR)/libxdialogs.a:
	cd $(XDLG_DIR); make

%.o: %.c %.h f*.h
	$(CC) $(CFLAGS) -o $@ -c $<
