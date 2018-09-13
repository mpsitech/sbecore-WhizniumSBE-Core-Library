#!/bin/bash
# file checkout.sh
# checkout script for Whiznium SBE core library, release sbecore_zedboard
# author Alexander Wirthmueller
# date created: 14 Aug 2014
# modified: 12 Jun 2016

export set SRCROOT=/home/mpsitech/emb/zedboard/avnet-digilent-zedboard-2017.2/build/tmp/sysroots/plnx_arm/home/root/src

mkdir $SRCROOT/sbecore

cp Makefile $SRCROOT/sbecore/

cp ../../*.h $SRCROOT/sbecore/
cp ../../*.cpp $SRCROOT/sbecore/
