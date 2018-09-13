# file Makefile
# make file for Whiznium SBE core library, release sbecore_jack
# author Alexander Wirthmueller
# date created: 14 Aug 2014
# modified: 27 May 2017

SRCROOT = /home/mpsitech/src

CPP = g++
CPPFLAGS = -std=c++11 -g -Wall -pthread -fpic

STATLIB = ar
STATLIBFLAGS = rcs

DYNLIB = g++
DYNLIBFLAGS = -shared
DYNLIBEXT = so

INCINSTPATH = /usr/local/include
LIBINSTPATH = /usr/local/lib

INCPATH = -I$(SRCROOT)
INCPATH += -I/usr/include/libxml2 -I/usr/include/mysql -I/usr/include/postgresql -I/usr/include
INCPATH += -I$(SRCROOT)/apiwzem -I$(SRCROOT)/dbswzem

OBJS = Dbs.o Engtypes.o Mt.o Strmod.o Stub.o Txtrd.o Types.o Xmlio.o
OBJS_MY = MyDbs.o
OBJS_PG = PgDbs.o
OBJS_LITE = LiteDbs.o
OBJS_MON = Mon.o TxtMon.o
OBJS_APIMON = ApiMon.o
OBJS_DBSMON = DbsMon.o

LIB = libsbecore
LIB_MY = libsbecore_my
LIB_PG = libsbecore_pg
LIB_LITE = libsbecore_lite
LIB_MON = libsbecore_mon
LIB_APIMON = libsbecore_apimon
LIB_DBSMON = libsbecore_dbsmon

.SUFFIXES: .cpp

.cpp.o:
	$(CPP) $(CPPFLAGS) $(INCPATH) -o $@ -c $<

all:	$(LIB) $(LIB_MY) $(LIB_PG) $(LIB_LITE) $(LIB_MON) $(LIB_APIMON) $(LIB_DBSMON)

$(LIB):	$(OBJS)
	$(STATLIB) $(STATLIBFLAGS) $(LIB).a $(OBJS)
	$(DYNLIB) $(DYNLIBFLAGS) $(OBJS) -o $(LIB).$(DYNLIBEXT)

$(LIB_MY):	$(OBJS_MY)
	$(STATLIB) $(STATLIBFLAGS) $(LIB_MY).a $(OBJS_MY)
	$(DYNLIB) $(DYNLIBFLAGS) $(OBJS_MY) -o $(LIB_MY).$(DYNLIBEXT)

$(LIB_PG):	$(OBJS_PG)
	$(STATLIB) $(STATLIBFLAGS) $(LIB_PG).a $(OBJS_PG)
	$(DYNLIB) $(DYNLIBFLAGS) $(OBJS_PG) -o $(LIB_PG).$(DYNLIBEXT)

$(LIB_LITE):	$(OBJS_LITE)
	$(STATLIB) $(STATLIBFLAGS) $(LIB_LITE).a $(OBJS_LITE)
	$(DYNLIB) $(DYNLIBFLAGS) $(OBJS_LITE) -o $(LIB_LITE).$(DYNLIBEXT)

$(LIB_MON):	$(OBJS_MON)
	$(STATLIB) $(STATLIBFLAGS) $(LIB_MON).a $(OBJS_MON)
	$(DYNLIB) $(DYNLIBFLAGS) $(OBJS_MON) -o $(LIB_MON).$(DYNLIBEXT)

$(LIB_APIMON):	$(OBJS_APIMON)
	$(STATLIB) $(STATLIBFLAGS) $(LIB_APIMON).a $(OBJS_APIMON)
	$(DYNLIB) $(DYNLIBFLAGS) $(OBJS_APIMON) -o $(LIB_APIMON).$(DYNLIBEXT)

$(LIB_DBSMON):	$(OBJS_DBSMON)
	$(STATLIB) $(STATLIBFLAGS) $(LIB_DBSMON).a $(OBJS_DBSMON)
	$(DYNLIB) $(DYNLIBFLAGS) $(OBJS_DBSMON) -o $(LIB_DBSMON).$(DYNLIBEXT)

install:
	-mkdir $(INCINSTPATH)/sbecore
	chmod 755 $(INCINSTPATH)/sbecore
	cp $(SRCROOT)/sbecore/*.h $(INCINSTPATH)/sbecore/
	chmod 644 $(INCINSTPATH)/sbecore/*.h
	cp $(LIB).a $(LIBINSTPATH)/
	cp $(LIB).$(DYNLIBEXT) $(LIBINSTPATH)/
	cp $(LIB_MY).a $(LIBINSTPATH)/
	cp $(LIB_MY).$(DYNLIBEXT) $(LIBINSTPATH)/
	cp $(LIB_PG).a $(LIBINSTPATH)/
	cp $(LIB_PG).$(DYNLIBEXT) $(LIBINSTPATH)/
	cp $(LIB_LITE).a $(LIBINSTPATH)/
	cp $(LIB_LITE).$(DYNLIBEXT) $(LIBINSTPATH)/
	cp $(LIB_MON).a $(LIBINSTPATH)/
	cp $(LIB_MON).$(DYNLIBEXT) $(LIBINSTPATH)/
	cp $(LIB_APIMON).a $(LIBINSTPATH)/
	cp $(LIB_APIMON).$(DYNLIBEXT) $(LIBINSTPATH)/
	cp $(LIB_DBSMON).a $(LIBINSTPATH)/
	cp $(LIB_DBSMON).$(DYNLIBEXT) $(LIBINSTPATH)/
	chmod 755 $(LIBINSTPATH)/libsbecore*

clean: 
	rm -f *.o
