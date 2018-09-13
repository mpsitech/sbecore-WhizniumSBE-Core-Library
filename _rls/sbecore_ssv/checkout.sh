#!/bin/bash
# file checkout.sh
# checkout script for Whiznium SBE core library, release sbecore_ssv
# author Alexander Wirthmueller
# date created: 14 Aug 2014
# modified: 12 Jun 2016

export set SRCROOT=/opt/poky/2.1/sysroots/armv7a-neon-poky-linux-gnueabi/home/root/src

mkdir $SRCROOT/sbecore

cp Makefile $SRCROOT/sbecore/

cp ../../*.h $SRCROOT/sbecore/
cp ../../*.cpp $SRCROOT/sbecore/
