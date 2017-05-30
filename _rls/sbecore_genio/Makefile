# file Makefile
# make file for Whiznium SBE core library, release sbecore_genio
# author Alexander Wirthmueller
# date created: 14 Aug 2014
# modified: 27 May 2017

SRCROOT = /home/epsi/src
LIBROOT = /home/epsi/lib

CC = g++
CPPFLAGS = -std=c++11 -g -Wall -pthread

INCS = -I$(SRCROOT)
INCS += -I/usr/include/libxml2
INCS += -I/usr/include/mysql -I/usr/include/postgresql
INCS += -I$(SRCROOT)/appwzem -I$(SRCROOT)/dbswzem

OBJECTS = Engtypes.o Mt.o Strmod.o Stub.o Txtrd.o Types.o Xmlio.o
OBJECTS_DBSENG = Dbs.o MyDbs.o PgDbs.o LiteDbs.o
OBJECTS_DBSENG += Mon.o Awem.o ApiMon.o DbsMon.o TxtMon.o

LIBRARY = libsbecore
LIBRARY_DBSENG = libsbedbsengcore

.SUFFIXES: .cpp

.cpp.o:
	$(CC) $(CPPFLAGS) $(INCS) -o $@ -c $<

all:	$(LIBRARY) $(LIBRARY_DBSENG)

$(LIBRARY):	$(OBJECTS)
	ar rcs $(LIBRARY).a $(OBJECTS)

$(LIBRARY_DBSENG):	$(OBJECTS) $(OBJECTS_DBSENG)
	ar rcs $(LIBRARY_DBSENG).a $(OBJECTS) $(OBJECTS_DBSENG)

install:
	cp $(LIBRARY).a $(LIBROOT)/
	cp $(LIBRARY_DBSENG).a $(LIBROOT)/

clean: 
	rm -f *.o
