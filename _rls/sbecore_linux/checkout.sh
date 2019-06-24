#!/bin/bash
# file checkout.sh
# checkout script for Whiznium SBE core library, release sbecore_linux
# author Alexander Wirthmueller
# date created: 14 Aug 2014
# modified: 24 Jun 2019

export set SRCROOT=${WHIZROOT}/src

mkdir ${SRCROOT}/sbecore

cp Makefile ${SRCROOT}/sbecore/

cp ../../*.h ${SRCROOT}/sbecore/
cp ../../*.cpp ${SRCROOT}/sbecore/
