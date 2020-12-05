#!/bin/bash
# file checkout.sh
# checkout script for Whiznium SBE core library
# copyright: (C) 2016-2020 MPSI Technologies GmbH
# author: Alexander Wirthmueller
# date created: 14 Aug 2014

export set BUILDROOT=${WHIZSDKROOT}/build

mkdir ${SYSROOT}${BUILDROOT}/sbecore

cp config.h ${SYSROOT}${BUILDROOT}/sbecore/
cp Makefile ${SYSROOT}${BUILDROOT}/sbecore/

cp ../*.h ${SYSROOT}${BUILDROOT}/sbecore/
cp ../*.cpp ${SYSROOT}${BUILDROOT}/sbecore/
