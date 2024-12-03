ROOTDIR=$(shell pwd)
SRCDIR=src
BUILDDIR=build
LIBDIR=$(BUILDDIR)/lib
SETDIR=$(SRCDIR)/utils/set
DEPDIRS=$(shell du $(SRCDIR) | cut -f 2)

CC=gcc
CFLAGS=-D_GNU_SOURCE -Wall -g -std=c99
CPPFLAGS=$(addprefix -I, $(DEPDIRS))
LDFLAGS=-L"$(BUILDDIR)/lib"
LDLIBS=-lpthread -lset -lconfig
VALGRIND=valgrind --leak-check=full --show-reachable=yes --track-origins=yes
BIN = $(shell ls -d src/**/*  | tr '\n' '\0' | xargs -0 -n 1 basename | grep .c$ | cut -d'.' -f1)

.PHONY: all
all : $(LIBDIR) server

$(LIBDIR) :
	mkdir -p $@

$(LIBDIR)/libset.a : $(SETDIR)/link.c $(SETDIR)/set.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(SETDIR)/link.c -o $(LIBDIR)/link.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(SETDIR)/set.c -o $(LIBDIR)/set.o
	ar rcs $@ $(LIBDIR)/link.o $(LIBDIR)/set.o
	rm -R $(LIBDIR)/link.o $(LIBDIR)/set.o

$(BUILDDIR)/%.o : $(SRCDIR)/**/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

server : $(LIBDIR)/libset.a $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(BIN))) $(BUILDDIR)/server.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o server $(LDLIBS)

clean :
	rm -R server $(BUILDDIR)/*.o $(SRCDIR)/*.o vgcore.*
