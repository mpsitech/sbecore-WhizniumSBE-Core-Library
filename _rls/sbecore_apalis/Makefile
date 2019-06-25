# file Makefile
# make file for Whiznium SBE core library, release sbecore_apalis
# author Alexander Wirthmueller
# date created: 14 Aug 2014
# modified: 25 Jun 2019

SBECORE_PLUS = 1
SBECORE_MAR = 0
SBECORE_MY = 0
SBECORE_PG = 0
SBECORE_LITE = 1

SRCROOT = /home/root/src

CPP = ${TCHROOT}/arm-linux-gnueabihf-g++
CPPFLAGS = --sysroot=${SYSROOT} -std=c++11 -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard -g -Wall -Os -pthread

STATLIB = ${TCHROOT}/arm-linux-gnueabihf-ar
STATLIBFLAGS = rcs
STATLIBEXT = a

DYNLIB = ${TCHROOT}/arm-linux-gnueabihf-g++
DYNLIBFLAGS = -shared
DYNLIBEXT = so

INCINSTPATH = ${SYSROOT}${WHIZROOT}/src
LIBINSTPATH = ${SYSROOT}${WHIZROOT}/lib

INCPATH = -I=${SRCROOT} -I=/usr/include/libxml2
ifeq (${SBECORE_PLUS},1)
	INCPATH += -I=${SRCROOT}/apiwzem -I=${SRCROOT}/dbswzem
endif
ifeq (${SBECORE_MAR},1)
	INCPATH += -I=/usr/include/mariadb
else ifeq (${SBECORE_MY},1)
	INCPATH += -I=/usr/include/mysql
endif
ifeq (${SBECORE_PG},1)
	INCPATH += -I=/usr/include/postgresql
endif
ifeq (${SBECORE_LITE},1)
	INCPATH += -I=/usr/include
endif

OBJS = Types.o Xmlio.o
OBJS_PLUS = Dbs.o Engtypes.o Mttypes.o Stub.o Txtrd.o
OBJS_MON = Mon.o TxtMon.o
OBJS_APIMON = ApiMon.o
OBJS_DBSMON = DbsMon.o
OBJS_MAR = MyDbs.o
OBJS_MY = MyDbs.o
OBJS_PG = PgDbs.o
OBJS_LITE = LiteDbs.o

LIB = libsbecore
LIB_PLUS = libsbecore_plus
LIB_MON = libsbecore_mon
LIB_APIMON = libsbecore_apimon
LIB_DBSMON = libsbecore_dbsmon
LIB_MAR = libsbecore_mar
LIB_MY = libsbecore_my
LIB_PG = libsbecore_pg
LIB_LITE = libsbecore_lite

ALL = ${LIB}
ifeq (${SBECORE_PLUS},1)
	ALL += ${LIB_PLUS} ${LIB_MON} ${LIB_APIMON} ${LIB_DBSMON}
endif
ifeq (${SBECORE_MAR},1)
	ALL += ${LIB_MAR}
else ifeq (${SBECORE_MY},1)
	ALL += ${LIB_MY}
endif
ifeq (${SBECORE_PG},1)
	ALL += ${LIB_PG}
endif
ifeq (${SBECORE_LITE},1)
	ALL += ${LIB_LITE}
endif

.SUFFIXES: .cpp

.cpp.o:
	${CPP} ${CPPFLAGS} ${INCPATH} -o $@ -c $<
all:	${ALL}

${LIB}:	${OBJS}
	${STATLIB} ${STATLIBFLAGS} ${LIB}.${STATLIBEXT} ${OBJS}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS} -o ${LIB}.${DYNLIBEXT}

${LIB_PLUS}:	${OBJS_PLUS}
	${STATLIB} ${STATLIBFLAGS} ${LIB_PLUS}.${STATLIBEXT} ${OBJS_PLUS}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS_PLUS} -o ${LIB_PLUS}.${DYNLIBEXT}

${LIB_MON}:	${OBJS_MON}
	${STATLIB} ${STATLIBFLAGS} ${LIB_MON}.${STATLIBEXT} ${OBJS_MON}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS_MON} -o ${LIB_MON}.${DYNLIBEXT}

${LIB_APIMON}:	${OBJS_APIMON}
	${STATLIB} ${STATLIBFLAGS} ${LIB_APIMON}.${STATLIBEXT} ${OBJS_APIMON}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS_APIMON} -o ${LIB_APIMON}.${DYNLIBEXT}

${LIB_DBSMON}:	${OBJS_DBSMON}
	${STATLIB} ${STATLIBFLAGS} ${LIB_DBSMON}.${STATLIBEXT} ${OBJS_DBSMON}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS_DBSMON} -o ${LIB_DBSMON}.${DYNLIBEXT}

${LIB_MAR}:	${OBJS_MAR}
	${STATLIB} ${STATLIBFLAGS} ${LIB_MAR}.${STATLIBEXT} ${OBJS_MAR}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS_MAR} -o ${LIB_MAR}.${DYNLIBEXT}

${LIB_MY}:	${OBJS_MY}
	${STATLIB} ${STATLIBFLAGS} ${LIB_MY}.${STATLIBEXT} ${OBJS_MY}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS_MY} -o ${LIB_MY}.${DYNLIBEXT}

${LIB_PG}:	${OBJS_PG}
	${STATLIB} ${STATLIBFLAGS} ${LIB_PG}.${STATLIBEXT} ${OBJS_PG}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS_PG} -o ${LIB_PG}.${DYNLIBEXT}

${LIB_LITE}:	${OBJS_LITE}
	${STATLIB} ${STATLIBFLAGS} ${LIB_LITE}.${STATLIBEXT} ${OBJS_LITE}
	${DYNLIB} ${DYNLIBFLAGS} ${OBJS_LITE} -o ${LIB_LITE}.${DYNLIBEXT}

install:
	#-mkdir ${INCINSTPATH}/sbecore
	#chmod 755 ${INCINSTPATH}/sbecore
	#cp ${SYSROOT}${SRCROOT}/sbecore/*.h ${INCINSTPATH}/sbecore/
	#chmod 644 ${INCINSTPATH}/sbecore/*.h
	cp ${LIB}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB}.${DYNLIBEXT} ${LIBINSTPATH}/
ifeq (${SBECORE_PLUS},1)
	cp ${LIB_PLUS}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_PLUS}.${DYNLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_MON}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_MON}.${DYNLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_APIMON}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_APIMON}.${DYNLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_DBSMON}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_DBSMON}.${DYNLIBEXT} ${LIBINSTPATH}/
endif
ifeq (${SBECORE_MAR},1)
	cp ${LIB_MAR}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_MAR}.${DYNLIBEXT} ${LIBINSTPATH}/
else ifeq (${SBECORE_MY},1)
	cp ${LIB_MY}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_MY}.${DYNLIBEXT} ${LIBINSTPATH}/
endif
ifeq (${SBECORE_PG},1)
	cp ${LIB_PG}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_PG}.${DYNLIBEXT} ${LIBINSTPATH}/
endif
ifeq (${SBECORE_LITE},1)
	cp ${LIB_LITE}.${STATLIBEXT} ${LIBINSTPATH}/
	cp ${LIB_LITE}.${DYNLIBEXT} ${LIBINSTPATH}/
endif
	chmod 755 ${LIBINSTPATH}/libsbecore*

clean:
	rm -f *.o
