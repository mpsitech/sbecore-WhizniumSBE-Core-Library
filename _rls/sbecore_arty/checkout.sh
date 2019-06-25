#!/bin/bash
# file checkout.sh
# checkout script for Whiznium SBE core library, release sbecore_arty
# author Alexander Wirthmueller
# date created: 14 Aug 2014
# modified: 24 Jun 2019

export set SRCROOT=${WHIZROOT}/src

mkdir ${SYSROOT}${SRCROOT}/sbecore

cp Makefile ${SYSROOT}${SRCROOT}/sbecore/

cp ../../*.h ${SYSROOT}${SRCROOT}/sbecore/
cp ../../*.cpp ${SYSROOT}${SRCROOT}/sbecore/
